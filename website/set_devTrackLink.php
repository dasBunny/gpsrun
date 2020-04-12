<?php
    require 'connect_db.php';
    $trackname="betatrack";
     if(isset($_POST['trackname'])){
        $trackname = $_POST['trackname']; 
    }
    $device="Beta";
    if(isset($_POST['device'])){
       $device = $_POST['device']; 
   }
    $status="fail";
    if($mysqli->connect_error) {
        echo json_encode($status);
        exit('Could not connect');
    }
    $stmt = mysqli_stmt_init($mysqli);
    $query="UPDATE devices SET trackname=? WHERE human_readable=?";
    if(!mysqli_stmt_prepare($stmt, $query)){
        echo "Prepare failed";
    } else {
      mysqli_stmt_bind_param($stmt,"ss",$trackname,$device);
      if (mysqli_stmt_execute($stmt)) {
        $result=mysqli_stmt_get_result($stmt);
        $status="sucess";
        }
    }
echo json_encode($status);

mysqli_close($mysqli);

?> 