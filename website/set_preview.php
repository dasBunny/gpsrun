<?php
    require 'connect_db.php';
    $trackname="riefensbeek";
     if(isset($_POST['trackname'])){
        $trackname = $_POST['trackname']; 
    }
    $status="fail";
    if($mysqli->connect_error) {
        echo json_encode($status);
        exit('Could not connect');
    }
    $stmt = mysqli_stmt_init($mysqli);
    $query="UPDATE config SET value=? where property='preview_track'";
    if(!mysqli_stmt_prepare($stmt, $query)){
    } else {
      mysqli_stmt_bind_param($stmt,"s",$trackname);
      if (mysqli_stmt_execute($stmt)) {
        $result=mysqli_stmt_get_result($stmt);
        $status="sucess";
        }
    }
echo json_encode($status);

mysqli_close($mysqli);

?> 