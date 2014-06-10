<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of UserModel
 *
 * @author tabk30
 */
class UserModel {

    //put your code here
    private $con;

    public function __construct() {
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

    public function addUserInfo($user_profile) {
        $sql = "INSERT INTO user_frofile(uid, name) " .
                "VALUES ('" . $user_profile["id"] . "','" . $user_profile["name"] . "')";
        $retval = mysql_query($sql, $this->con);
        if (!$retval) {
            die('Could not enter data: ' . mysql_error());
        }
    }
    
    public function checkUserExit($user_id){
        $result = mysql_query("SELECT * FROM user_frofile WHERE uid='" . $user_id . "'", $this->con);
        while ($row = mysql_fetch_array($result)) {
            return 1;
        }
        return 0;
    }

    public function __destruct() {
        mysql_close($this->con);
    }

}
