<?php
session_start();
?>
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <title>GPS Staffelstab</title>
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
        <li class="top"><a href="map.html">Karte</a></li>
        <li class="top"><a class="active" href="admin_panel.php">Adminpanel</a></li>
        <li class="top"><a href="about.php">Über diese Seite</a></li>
        <li class="top"><a href="manual.php">Bedienungsanleitung</a></li>
      </ul>
    <div style="margin-top: 7vh">
    Diese Bedienungsanleitung ist noch nicht vollständig und wird regelmäßig erweitert.
    <h1>Website</h1>
    <h2>Icons</h2>
    <table>
        <tr>
            <td> <img src="laeufer_black.png" width="50" height="50" alt="Läufer Icon"> </td>
            <td> Der Läufer markiert die letzte bekannte Position des Stabs auf dem ausgewählten Track.
        </tr>
        <tr>
            <td> <img src="cursor.png" width="50" height="50" alt="Standort Marker"> </td>
            <td> Der Pfeil markiert den Standort des Website-Nutzers. Je nachdem, welche Ortungsmethoden zur Verfügung stehen, kann der Standort deutlich vom tatsächlichen Standort abweichen. Bei Mobilgeräten ist er in der Regel aber sehr genau. </td>
    </tr>
    </table>
    </div>
    <div>
        <h2>Adminpanel</h1>
        <h3>Zugang</h2>
        Zugang zum Adminpanel haben nur vorher bestätigte Nutzer, die sich mit ihren Logindaten anmelden müssen. Es wird nur Personen, die an der Organisation des Laufs oder dem Betrieb des Systems beteiligt sind, Zugang gewährt. 
        Nachdem ein Nutzer sich regristiert hat muss er noch manuell freigeschaltet werden. 
    </div>
    <h1>Staffelstab</h1>
    <h2>Verhalten</h2>
    Wenn der Stab eingeschaltet ist, sendet er alle 120 Sekunden seinen Standort. 
    <h2>Knopf</h2>
    Mit dem Knopf kann die Status-LED ausgelöst und ein Sendevorgang gestartet werden. Da das Senden Energie kostet sollte der Knopf möglichst wenig gedrückt werden. Der Stab sendet auch ohne Aufforderung alle 120 Sekunden!
    <h2>Status-LED</h2>
    Der Stab gibt über die Farbe und Blinken/Dauersignal der LED an, wie voll der Akku ist. Dies geschieht immer kurz bevor auch ein Sendevorgang stattfindet.
</body>