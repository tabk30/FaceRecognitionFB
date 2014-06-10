<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of PhotoTagModel
 *
 * @author tabk30
 */
//require_once ('../fb/facebook.php');
require_once ('PhotoModel.php');

class PhotoTagModel {

    //put your code here
    private $config;
    private $facebook;
    private $access_token;
    private $con;

    public function __construct($access_token) {
        $this->access_token = $access_token;
        $this->config = array(
            'appId' => '692878360771056',
            'secret' => 'cdc0b3e178feea514ec217d84c8a2395',
            'cookie' => true
        );
        $this->facebook = new Facebook($this->config);
        //connect database:
        $dbhost = 'localhost:3036';
        $dbuser = 'root';
        $dbpass = 'root';
        $this->con = mysql_connect($dbhost, $dbuser, $dbpass);
        mysql_select_db('PhotoInfo');
// Check connection
        if (mysqli_connect_errno()) {
            echo "Failed to connect to MySQL: " . mysqli_connect_error();
        }
    }

    public function getPhotoTagOfUser($user_frofile) {
        if ($user_frofile) {

            // We have a user ID, so probably a logged in user.
            // If not, we'll get an exception, which we handle below.
            try {
                $fql = "SELECT pid, text, xcoord, ycoord, subject FROM photo_tag WHERE subject=" . $user_frofile["id"] . " LIMIT 30 OFFSET 0";
                $ret_obj = $this->facebook->api(array(
                    'method' => 'fql.query',
                    'query' => $fql,
                    'access_token' => $this->access_token
                ));
                $this->savePosision($ret_obj);
                return $ret_obj;
            } catch (FacebookApiException $e) {
                // If the user is logged out, you can have a 
                // user ID even though the access token is invalid.
                // In this case, we'll get an exception, so we'll
                // just ask the user to login again here.
                $login_url = $this->facebook->getLoginUrl();
                echo 'Request photo tag of user, Please <a href="' . $login_url . '">login.</a>';
                error_log($e->getType());
                error_log($e->getMessage());
            }
        } else {
            // No user, so print a link for the user to login
            //$login_url = $this->facebook->getLoginUrl();
            echo 'Request photo tag of user, Please <a href="' . $login_url . '">login.</a>';
        }
        return array(0);
    }

    public function savePosision($data) {
        $photo_model = new PhotoModel($this->access_token);

        foreach ($data as $row) {
            if ($this->checkExitPosision($row["pid"])) {
                continue 1;
            }
            $sql = "INSERT INTO photo_tag(pid, text, subject, xcoord, ycoord) " .
                    "VALUES ('" . $row["pid"] . "','" . $row["text"] . "','" . $row["subject"] . "','" . $row["xcoord"] . "','" . $row["ycoord"] . "')";
            $photo_model->getPhotoLink($row["pid"], $row["subject"]);
            $retval = mysql_query($sql, $this->con);
            if (!$retval) {
                die('Could not enter data: ' . mysql_error());
            }
        }
    }

    public function checkExitPosision($pid) {
        $result = mysql_query("SELECT * FROM photo_tag WHERE pid='" . $pid . "'", $this->con);
        while ($row = mysql_fetch_array($result)) {
            return 1;
        }
        return 0;
    }
}
