<?php
require 'connect_db.php';
$names=array();
if ($result = $mysqli -> query("SELECT name FROM coords GROUP BY name")) {
while($rows = $result->fetch_assoc()){
  array_push($names,$rows['name']);
}
echo json_encode($names);
}
$result -> free_result();
mysqli_close($mysqli);

?> 