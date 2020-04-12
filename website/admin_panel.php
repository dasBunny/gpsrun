<?php
session_start();
if(isset($_SESSION['username'])){
}
else{
    header("Location: login.php");
    exit();
}
?>
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <script type="text/javascript" src="https://code.jquery.com/jquery-3.4.1.min.js"></script>
    <style>
                    select { 
                appearance: none; 
                outline: 0; 
                background: rgb(231, 231, 231); 
                background-image: none; 
                width: 200px; 
                height: 50px; 
                color: black; 
                cursor: pointer; 
                border:1px solid rgb(192, 192, 192); 
            } 
            .select { 
                position: relative; 
                display: block; 
                width: 15em; 
                height: 2em; 
                line-height: 3; 
                overflow: hidden; 
                border-radius: .25em; 
                padding-bottom:10px; 
                  
            }

        .button {
          position: relative;
          background-color: #4CAF50;
          border: none;
          font-size: 15px;
          color: #FFFFFF;
          height: 50px;
          width: 80px;
          text-align: center;
          -webkit-transition-duration: 0.4s; /* Safari */
          transition-duration: 0.4s;
          text-decoration: none;
          overflow: hidden;
          cursor: pointer;
        }
        
        .button:after {
          content: "";
          background: #90EE90;
          display: block;
          position: absolute;
          padding-top: 300%;
          padding-left: 350%;
          margin-left: -20px!important;
          margin-top: -120%;
          opacity: 0;
          transition: all 0.8s
        }
        
        .button:active:after {
          padding: 0;
          margin: 0;
          opacity: 1;
          transition: 0s
        }
        body {
            margin-left: 10px;
            padding: 0;
        }
        input{
            width: 200px;
            height: 45px;
            font-size: 20px;
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
        </style>
</head>
<body bgcolor="#D1D1D1">
    <ul class="top">
        <li class="top"><a href="map.html">Map</a></li>
        <li class="top"><a class="active" href="admin_panel.php">Admin panel</a></li>
        <li class="top"><a href="about.html">About</a></li>
        <li class="top" style="float:right"><a>
            <form action="logout.php" method="post">
                <button class="logoutbutton" type="submit" name="logout-submit">Logout</button>
            </form>
        </a></li>
      </ul>
    <div style="margin-top: 10vh">
        <h1>Standardtrack auswählen</h1>
        <p>Hier kannst du den Track auswählen, der jedem Besucher standardmäßig auf der Karte angezeigt wird.</p>
        <table style="width:300px">
            <tr>
                <th> 
                    <select id="select1" name="select1">
                        <option>Options not loaded yet...</option>
                    </select>
                </th>
                <th>
                    <button class="button" onclick="setDefaultTrack()">SET TRACK</button>
                </th>
            </tr>
        </table>        
        <p id="return1"> </p>
    </div>
    <div>
        <h1>Vorschautrack auswählen</h1>
        <p>Hier kannst du den Track auswählen, der jedem Besucher gestrichelt im Hintergrund auf der Karte angezeigt wird.</p>
        <table style="width:300px">
            <tr>
                <th> 
                    <select id="select_prev" name="select_prev">
                        <option>Options not loaded yet...</option>
                    </select>
                </th>
                <th>
                    <button class="button" onclick="setPreviewTrack()">SET TRACK</button>
                </th>
            </tr>
        </table>        
        <p id="return_prev"> </p>
    </div>
    <div>
        <h1>Gerät mit Track verknüpfen</h1>
        <p>Hier kannst du auswählen, auf welchen Track ein Gerät speichern soll.</p>
        <h2>Bestehenden Track verwenden</h2>
        <table style="width:500px">
            <tr>
                <th>
                    Gerätename
                </th>
                <th>
                    Trackname
                </th>
                <th>
                </th>
            </tr>
            <tr>
                <th> 
                    <select id="device2" name="device2">
                        <option>Options not loaded yet...</option>
                    </select>
                </th>
                <th>
                    <select id="track2" name="track2">
                        <option>Options not loaded yet...</option>
                    </select>
                </th>
                <th>
                    <button class="button" onclick="linkOldTrack()">SET TRACK</button>
                </th>
            </tr>
        </table>
        <p id="return2"> </p>
        <h2>Neuen Track anlegen</h2>
    </div>
    <div>
        <table style="width:300px">
            <tr>
                <th>
                    Gerätename
                </th>
                <th>
                    Trackname
                </th>
                <th>
                </th>
            </tr>
            <tr>
                <td> 
                    <select id="device1" name="device1">
                        <option>Options not loaded yet...</option>
                    </select>
                </td>
                <td>
                    <input type="text" id="fname" name="fname">
                </td>
                <td>
                    <button class="button" onclick="linkNewTrack()">SET TRACK</button>
                </td>
            </tr>
        </table>    
        <p id="return3"> </p>
    <script>
        function linkOldTrack(){
            var e = document.getElementById("track2");
            trackname1 = e.options[e.selectedIndex].value;
            linkDevice("device2",trackname1);
            console.log(trackname1);
        }
        function linkDevice(device_selector,trackname){
            var e = document.getElementById(device_selector);
            devicename = e.options[e.selectedIndex].value;
            console.log(devicename);
            console.log(trackname1);
            $.ajax({
                type: "POST",
                url: "set_devTrackLink.php",
                data: {trackname: trackname, device: devicename},
                dataType:'JSON', 
                success: function(response){
                    if(response="sucess"){
                        document.getElementById("return2").style.color = "green";
                        document.getElementById("return2").innerHTML="Sucess!";
                    }
                }       
            });
        }
        function setDefaultTrack(){
            var e = document.getElementById("select1");
            trackname1 = e.options[e.selectedIndex].value;
            console.log(trackname1);
            $.ajax({
                type: "POST",
                url: "set_default.php",
                data: {trackname: trackname1},
                dataType:'JSON', 
                success: function(){
                    document.getElementById("return1").style.color = "green";
                    document.getElementById("return1").innerHTML="Sucess!";
                },
                error: function(error){
                    document.getElementById("return1").style.color = "red";
                    document.getElementById("return1").innerHTML="Failure!";
                }           
            });
        }
        function setPreviewTrack(){
            var e = document.getElementById("select_prev");
            trackname1 = e.options[e.selectedIndex].value;
            console.log(trackname1);
            $.ajax({
                type: "POST",
                url: "set_preview.php",
                data: {trackname: trackname1},
                dataType:'JSON', 
                success: function(){
                    document.getElementById("return_prev").style.color = "green";
                    document.getElementById("return_prev").innerHTML="Sucess!";
                },
                error: function(error){
                    document.getElementById("return_prev").style.color = "red";
                    document.getElementById("return_prev").innerHTML="Failure!";
                }    
            });
        }
        function populateSelectMenu() {
            var selects = ['select1','track2','select_prev'];
            console.log("populating");
            $.ajax({
                type: "POST",
                url: "get_tracknames.php",
                data: {},
                dataType:'JSON', 
                success: function(response){
                    console.log(response);
                    selects.forEach(element => {
                        console.log(element);
                        var select = document.getElementById(element), i = 0, il=response.length, length = select.options.length;
                        for (j = length-1; j >= 0; j--) {
                            select.options[j] = null;
                        }   
                        for(;i<il;i++){
                            console.log(response[i]);
                            var opt = document.createElement("option");
                            opt.value=response[i];
                            opt.innerHTML = response[i];
                            select.appendChild(opt);
                        }
                    })
                }
            });
        }

        function populateDeviceMenu() {
            var selects = ['device1','device2'];
            console.log("populating");
            $.ajax({
                type: "POST",
                url: "get_devices.php",
                data: {},
                dataType:'JSON', 
                success: function(response){
                    console.log(response);
                    selects.forEach(element => {
                        console.log(element);
                        var select = document.getElementById(element), i = 0, il=response.length, length = select.options.length;
                        for (j = length-1; j >= 0; j--) {
                            select.options[j] = null;
                        }   
                        for(;i<il;i++){
                            console.log(response[i]);
                            var opt = document.createElement("option");
                            opt.value=response[i];
                            opt.innerHTML = response[i];
                            select.appendChild(opt);
                        }
                    })
                }
            });
        }
        populateSelectMenu();
        populateDeviceMenu();
    </script>
</body>