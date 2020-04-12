<?php
require 'connect_db.php';
$names=array();
if ($result = $mysqli -> query("SELECT human_readable FROM devices")) {
while($rows = $result->fetch_assoc()){
  array_push($names,$rows['human_readable']);
}
echo json_encode($names);
}
$result -> free_result();
mysqli_close($mysqli);

?> 