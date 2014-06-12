<?php
require_once ('fb/facebook.php');
require_once ('model/PhotoTagModel.php');
require_once ('model/UserModel.php');

$fbconfig['appUrl'] = "The full url of your app on Facebook goes here";

//Create An instance of our Facebook Application .
$facebook = new Facebook(array(
    'appId' => '692878360771056',
    'secret' => 'cdc0b3e178feea514ec217d84c8a2395',
    'cookies' => 'true',
        ));
$user_id = $facebook->getUser();
if ($user_id) {
    try {
        $user_profile = $facebook->api('/me');
        $user_model = new UserModel();
        if ($user_model->checkUserExit($user_profile["id"]) == 0) {
            $user_model->addUserInfo($user_profile);

//Create folder to save image data:
            mkdir("train/" . $user_profile["id"]);

            $facebook->setExtendedAccessToken();
            $access_token = $facebook->getAccessToken();
            $photo_tag_model = new PhotoTagModel($access_token);
            $photo_tag_model->getPhotoTagOfUser($user_profile);
            exec('./faceDetect/face_detect');
            exec('chmod -R 777 train/*');
            $photo_tag_model->cleanData();
        }

//Get foto and save to server
    } catch (FacebookApiException $e) {
        error_log($e);
        $user = null;
    }
} else {
    echo '<h1>Not get user info</h1>';
    error_log($e->getType());
    error_log($e->getMessage());
}

//process iamge upload:
$image_detect = NULL;
$image_recognition = NULL;
$result = NULL;
if (($_FILES["image_recognition"]["error"] > 0)) {
    echo "Error: " . $_FILES["image_recognition"]["error"] . "<br>";
} else {
    if (file_exists($_FILES['image_recognition']['name'])) {
        $info = pathinfo($_FILES['image_recognition']['name']);
        $ext = $info['extension'];
        $name = $info['basename'];
        //$newname = $name . $ext;

        $target = 'images/' . $name;
        move_uploaded_file($_FILES['image_recognition']['tmp_name'], $target);
        echo exec('chmod -R 755 images/*');
        echo './FaceRecognition/recognition ' . $target . ' ' . $_FILES['image_recognition']['name'];
        echo exec('./FaceRecognition/recognition ' . $target . ' ' . $_FILES['image_recognition']['name']);
        $image_detect = 'images/' . $_FILES['image_recognition']['name'] . '_detection.jpg';
        $image_recognition = 'images/' . $_FILES['image_recognition']['name'] . '_recognition.jpg';
        $result = 'images/' . $_FILES['image_recognition']['name'] . '_result.txt';
        //read result:
        $fh = fopen('filename.txt', 'r');
        $result_display = '<ol>';
        while ($line = fgets($fh)) {
            $result_display = '<li>';
            $result_display = $line;
            $result_display = '</li>';
        }
        fclose($fh);
        $result_display = $result_display + '</ol>';
    }
}
?> 

<html>
    <body>
        <form action="index.php" method="post"
              enctype="multipart/form-data">
            <label for="file">Upload Image:</label>
            <input type="file" name="image_recognition" id="file"><br>
            <input type="submit" name="submit" value="Submit">
        </form>
        <?php
        if ($image_detect != null) {
            echo '<h1> Detect Result </h1>';
            echo '<img src="' . $target . '" alt="Detect Result">';
        }
        if ($image_recognition != NULL) {
            echo '<h1> Recognition Result </h1>';
            echo '<img src="' . $result . '" alt="Recognition Result">';
        }
        if ($result != NULL) {
            echo '<h1>' . $result_display . '</h1>';
        }
        ?>
    </body>
</html> 