<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of PhotoModel
 *
 * @author tabk30
 */
//require_once('../fb/facebook.php');

class PhotoModel {

    //put your code here
    //put your code here
    private $config;
    private $facebook;
    private $access_token;

    public function __construct($access_token) {
        $this->access_token = $access_token;
        $this->config = array(
            'appId' => '692878360771056',
            'secret' => 'cdc0b3e178feea514ec217d84c8a2395',
            'cookie' => true
        );
        $this->facebook = new Facebook($this->config);
    }

    public function getPhotoLink($photo_id, $subject) {
        $user_id = $this->facebook->getUser();
        if ($user_id) {

            // We have a user ID, so probably a logged in user.
            // If not, we'll get an exception, which we handle below.
            try {
                $fql = "SELECT src_big FROM photo WHERE pid= '" . $photo_id . "'";
                $ret_obj = $this->facebook->api(array(
                    'method' => 'fql.query',
                    'query' => $fql,
                    'access_token' => $this->access_token
                ));
                
                foreach ($ret_obj as $row) {
                    $this->savePhotoToClient($row["src_big"], $photo_id, $subject);
                }
            } catch (FacebookApiException $e) {
                // If the user is logged out, you can have a 
                // user ID even though the access token is invalid.
                // In this case, we'll get an exception, so we'll
                // just ask the user to login again here.
                $login_url = $this->facebook->getLoginUrl();
                echo 'Please <a href="' . $login_url . '">login.</a>';
                error_log($e->getType());
                error_log($e->getMessage());
            }
        } else {
            // No user, so print a link for the user to login
            $login_url = $this->facebook->getLoginUrl();
            echo 'Please <a href="' . $login_url . '">login.</a>';
        }
        return array(0);
    }

    public function savePhotoToClient($src, $photo_id, $subject) {
        file_put_contents("train/".$subject."/" . $photo_id . ".jpg", file_get_contents($src));
        chmod ( "train/".$subject."/" . $photo_id . ".jpg" ,  777 );
    }

}
