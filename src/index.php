<?php

require_once ('fb/facebook.php');
require_once ('model/PhotoTagModel.php');
require_once ('model/UserModel.php');

$fbconfig['appUrl'] = "The full url of your app on Facebook goes here";

echo "<h1>Face Recognition</h1>";

//Create An instance of our Facebook Application .
$facebook = new Facebook(array(
    'appId' => '692878360771056',
    'secret' => 'cdc0b3e178feea514ec217d84c8a2395',
    'cookies' => 'true',
        ));
$user_id = $facebook->getUser();

if ($user_id) {
    echo "<h1>User login success</h1>";
    try {
        $user_profile = $facebook->api('/me');
        $user_model = new UserModel();
        //exec("./faceDetect/face_detect");
        //echo "<h1> " . exec('./faceDetect/face_detect') . "<h1> ";
        if ($user_model->checkUserExit($user_profile["id"]) == 0) {
            $user_model->addUserInfo($user_profile);

            //Create folder to save image data:
            mkdir("train/" . $user_profile["id"]);

            $facebook->setExtendedAccessToken();
            $access_token = $facebook->getAccessToken();
            $photo_tag_model = new PhotoTagModel($access_token);
            $photo_tag_model->getPhotoTagOfUser($user_profile);
            echo exec('./faceDetect/face_detect');
            echo exec('chmod -R 777 train/*');
            $photo_tag_model->cleanData();
            //echo exec('chmod 777 train/*.jpg');
        } else {
            
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

echo "	<h1>End program</h1>";
?>