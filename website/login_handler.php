<?php
require 'connect_db.php';
if(isset($_POST['submit'])){
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    if(empty($password) || empty($username)) {
        header("Location: login.php?error=emptyuserpw");
        exit();
    }
    else{
        $stmt = mysqli_stmt_init($mysqli);
        $query="SELECT password,status FROM users where username=?";
        if(!mysqli_stmt_prepare($stmt, $query)){
            header("Location: login.php?error=dberror");
            exit();
        } else {
            mysqli_stmt_bind_param($stmt,"s",$username);
            mysqli_stmt_execute($stmt);
            $result = mysqli_stmt_get_result($stmt);
            if($row  = mysqli_fetch_assoc($result)){
                if($row['status']!=1){
                    header("Location: login.php?error=notactivated");
                    exit();
                }
                $pwdCheck = password_verify($password, $row['password']);
                if($pwdCheck==false){
                    header("Location: login.php?error=pwerror");
                    exit();
                }
                else if($pwdCheck == true){
                    session_start();
                    $_SESSION['username']=$username;
                    header("Location: admin_panel.php");
                }
                else {
                    header("Location: login.php?error=pwerror");
                    exit();
                }
            }
            else {
                header("Location: login.php?error=nodb");
                exit();
            }
        }
    }
}
else{
    header("Location: login.php?error=submitnotset");
}
?>
