<?php
require 'connect_db.php';
$trackname="beta";
if(isset($_POST['trackname'])){
  $trackname = $_POST['trackname']; 
}
if (!isset($coords)) 
    $coords = new stdClass();
if (!isset($marker)) 
    $marker = new stdClass();
if (!isset($result)) 
    $result = new stdClass();
if($mysqli->connect_error) {
    echo "fail";
    exit('Could not connect');
}

$preview_trackname = "beta";
$stmt = mysqli_stmt_init($mysqli);
$query="SELECT value FROM config where property='preview_track' LIMIT 1";
if(!mysqli_stmt_prepare($stmt, $query)){
  echo "fail";
} else {
  if (mysqli_stmt_execute($stmt)) {
    $result=mysqli_stmt_get_result($stmt);
    $num=$result->num_rows;
    if ($num == 1) {
      $row = $result->fetch_assoc();
      $preview_trackname = $row['value'];
    }
    else echo "fail";
  }
  else echo "fail";
}
$query="SELECT * FROM coords where name=? order by timestamp asc LIMIT 1000";
if(!mysqli_stmt_prepare($stmt, $query)){
  echo "fail";
} else {
  mysqli_stmt_bind_param($stmt,"s",$preview_trackname);
  if (mysqli_stmt_execute($stmt)) {
    $result=mysqli_stmt_get_result($stmt);
    $num=$result->num_rows;
    if ($num > 0) {
      // output data of each row
      $track = array();
      for($i=0; $i<$num; $i++) {
        $row = $result->fetch_assoc();
        $pos = array($row["lat"],$row["lng"]);
        array_push($track,$pos);
      }
      $coords->preview=$track;
    }
  }
}
$query="SELECT * FROM coords where name=? order by timestamp asc LIMIT 5000";
if(!mysqli_stmt_prepare($stmt, $query)){
  echo "fail";
} else {
  mysqli_stmt_bind_param($stmt,"s",$trackname);
  if (mysqli_stmt_execute($stmt)) {
    $result=mysqli_stmt_get_result($stmt);
    $num=$result->num_rows;
    if ($num > 0) {
      // output data of each row
      $track = array();
      for($i=0; $i<$num-1; $i++) {
        $row = $result->fetch_assoc();
        $pos = array($row["lat"],$row["lng"],$row["timestamp"]);
        array_push($track,$pos);
      }
      $row = $result->fetch_assoc();
      $marker->lat=$row["lat"];
      $marker->long=$row["lng"];
      $marker->timestamp=$row["timestamp"];
      $coords->marker=$marker;
      array_push($track,array($row["lat"],$row["lng"]));
      $coords->track=$track;
      $myJSON = json_encode($coords);
      echo $myJSON;
    }
}
  // Free result set
  $result -> free_result();
}

mysqli_close($mysqli);

?> 