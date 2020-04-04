<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  font-family: Arial, 
  Helvetica, sans-serif;
}
ul.top {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
  background-color: rgb(41, 45, 48);
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 5vh;
  z-index: 999
}
.top li a {
  float: left;
  display: block;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
  font-family:georgia,garamond,serif;
  font-size:16px;
  font-style:italic;
}
li a:hover:not(.active) {
   background-color: #9E9E9E;
}
form {
  border: 3px solid #9C9C9C;
  width: 30vw;
  margin-left: 35vw;
  margin-top: 20vh;
  }

input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #9C9C9C;
  box-sizing: border-box;
}

button {
  background-color: #4CAF50;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  cursor: pointer;
  width: 100%;                     
}

button:hover {
  opacity: 0.8;
}

.cancelbtn {
  width: auto;
  padding: 10px 18px;
  background-color: #f44336;
}


.container {
  padding: 16px;
}

span.psw {
  float: right;
  padding-top: 16px;
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
  span.psw {
     display: block;
     float: none;
  }
  .cancelbtn {
     width: 100%;
  }
}
</style>
</head>
<body bgcolor="#D1D1D1">
  <ul class="top">
    <li class="top"><a href="map.html">Map</a></li>
    <li class="top"><a href="admin_panel.php">Admin panel</a></li>
    <li class="top"><a href="about.html">About</a></li>
</ul>

<form action="signup_handler.php" method="post">
  <div class="container">
    <h2>Log in to make changes</h2>
        <label for="username"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="username" required>
        <label for="psw"><b>Password</b></label>
        <input type="password" placeholder="Enter Password" name="password" required>
        <label for="psw"><b>Repeat Password</b></label>
        <input type="password" placeholder="Repeat Password" name="password_repeat" required>
        <button type="submit" name="submit">Sign up</button>
        <label>
          <input type="checkbox" checked="checked" name="remember"> Remember me
        </label>
      </div>
    
      <div class="container" style="background-color:#f1f1f1">
        <button type="button" class="cancelbtn" href="map.html">Cancel</button>
        <span class="psw"><a href="#">Privacy policy</a></span>
      </div>
    </form>
    
</body>
</html>