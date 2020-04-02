<?php
  require 'connect_db.php';
  if (!isset($config)) 
    $config = new stdClass();


  if ($result = $mysqli -> query("SELECT property,value FROM config")) {
    while($row=$result->fetch_assoc()){
      $property = $row["property"];
      $value = $row["value"];
      $config->$property=$value;
    }
    $myJSON = json_encode($config);
    echo $myJSON;
  }
  // Free result set
  $result -> free_result();


mysqli_close($mysqli);

?> 