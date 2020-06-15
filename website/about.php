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
    <div style="margin-top: 10vh">
        <h1>Über diese Seite</h1>
        <p>Im Rahmen der Städtepartnerschaft mit La Garenne-Colombes veranstaltet die Stadt Wangen im Allgäu einen Staffellauf. </p>
        <p>Diese Website wurde, gemeinsam mit der Trackinghardware, die uns die Daten liefert, im Rahmen eines Semesterprojekts an der FH Vorarlberg entwickelt. </br></p>
        Hintergrund bla bla bla
    </div>
    <div>
        <h1>Impressum & Datenschutz</h1>
        Unsere Datenschutzerklärung befindet sich <a href="privacy.php">hier</a>
        <h2>Verantwortlich nach §5 Telemediengesetz</h2>
        Max Mustermann </br>
        Beispielweg 10 </br>
        31337 Astadt
    </div>
    <div>
        <h1>Weitere Informationen</h1>
        <h2>Wangen im Allgäu</h2>
        <a href="https://www.wangen.de/buerger/stadt-ortschaften/partnerstaedte/la-garenne-colombes.html">Partnerschaft mit La Garenne-Colombes</a>
        <h2>Projekt</h2>
        <a href="fhv.at">Fachhochschule Vorarlberg</a></br>
        <a href="fhv.at">Projektdokumentation</a>
    </div>
</body>