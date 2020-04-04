<?php
require 'connect_db.php';
if(isset($_POST['submit'])){
    $username = $_POST['username'];
    $password = $_POST['password'];
    $password_repeat = $_POST['password_repeat'];
    
    if(empty($password) || empty($username) || empty($password_repeat)) {
        header("Location: login.php?error=emptyfield");
        exit();
    }
    else{
        if($password_repeat == $password) {
            $pw_hashed = password_hash($password,PASSWORD_DEFAULT);
            $stmt = mysqli_stmt_init($mysqli);
            $query="INSERT INTO users (username,password) VALUES (?,?)";
            if(!mysqli_stmt_prepare($stmt, $query)){
                header("Location: login.php?error=dberror");
                exit();
            } else {
                mysqli_stmt_bind_param($stmt,"ss",$username,$pw_hashed);
                mysqli_stmt_execute($stmt);
                header("Location: login.php");
            }
        }
        else {
            header("Location: signup.php?error=passwordmismatch");
        }   
    }
}
else{
    header("Location: login.php?error=submitnotset");
}
?>
