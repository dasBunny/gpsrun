<?php
$password="1234";
?>
<!DOCTYPE HTML>
<html>
<head>

</head>
<body>
    <p>
<?php
    echo password_hash("1234",PASSWORD_DEFAULT);
?>
</p>
</html>
