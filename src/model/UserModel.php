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

    public function checkUserExit($user_id) {
        $result = mysql_query("SELECT * FROM user_frofile WHERE uid='" . $user_id . "'", $this->con);
        while ($row = mysql_fetch_array($result)) {
            return 1;
        }
        return 0;
    }
    
    public function getAllUser(){
        $result = mysql_query("SELECT * FROM user_frofile", $this->con);
        $all_user = array();
        while ($row = mysql_fetch_array($result)) {
            $all_user[] = $row;
        }
        var_dump($all_user);
        return $all_user;
    }

    public function getUserName($user_id) {
        $result = mysql_query("SELECT * FROM user_frofile WHERE uid='" . $user_id . "'", $this->con);
        $row = mysql_fetch_array($result);
        var_dump($row);
        return $row['name'];
    }
    
    public function closeConnect(){
        mysql_close($this->con);
    }

    public function __destruct() {
        mysql_close($this->con);
    }

}
