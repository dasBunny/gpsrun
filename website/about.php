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
        Diese Seite wurde im Rahmen eines Semesterprojekts im Studiengang Mechatronik an der FH Vorarlberg umgesetzt. 
        <h2>Die Idee</h2>
        2018 feierte die Stadt Wangen im Allgäu das 30. Jubiläum der Städtepartnerschaft mit der italienischen Stadt Prato. Dazu wurde unter anderem ein Staffellauf von Wangen nach in die Toskana veranstaltet.
        2020 jährt sich die Partnerschaft mit La Garenne-Colombes in der Metropolregion Paris zum 40. Mal und auch hier soll ein Lauf veranstaltet werden. Um die Staffelübergabe zu erleichtern soll die Position der Läufer online abrufbar sein und dadurch Orientierungsprobleme verhindert werden.
        Der Staffelstab ist der einzige Gegenstand, der dauerhaft bei den aktuell aktiven Läufern ist. Daher kam die Idee, darin einen GPS-Sender einzubauen.
        <h2>Die Umsetzung</h2>
        In diesem Projekt wurde ein Staffelstab mit Tracking Funktion hergestellt. Dazu wurde eine Schaltung mit einem Mikroprozessor, einem GPS- und einem LTE-Modul entworfen. Der Mikroprozessor ist ein ESP8266 und dafür zuständig, die Position über das GPS-Modul auszulesen und über das LTE-Modul zu versenden.
        Der Stab wurde mit einem 3D Drucker hergestellt, da so eine Schnellwechselfunktion für die Batterien und eine genaue Anpassung auf die Elektronik sehr gut möglich sind.
        Die Daten werden an einen Server übertragen und dort gespeichert. Auf einer Website können die GPS-Tracks angesehen werden. Für die Organisatoren gibt es ein Adminpanel, in dem Einstellungen geändert werden können.
    </div>
    <div>
        <h1>Impressum & Datenschutz</h1>
            <h2>Verantwortlich für den Inhalt</h2>
            Tim Strutzberg </br>
            Müllerstraße 32 </br>
            6850 Dornbirn </br>
            Österreich </br>
            Kontakt-Email: tim.strutzberg@gmail.com</br>
            <h2>Datenverarbeitung</h2>
            Wir speichern ausschließlich die Logindaten von Nutzern, die sich für das Adminpanel anmelden. Dabei werden Nutzername und Passwort gespeichert. Die Daten werden für keine weiteren Zwecke genutzt. Eine Löschung dieser Daten ist jederzeit per Anfrage an die oben genannte Kontakt-Email möglich. </br>
            Wir verwenden keine Cookies. Die Website bittet um die Freigabe des Standorts, um ihn auf der Karte zu markieren. Der gesamte Prozess findet auf dem Endgerät des Nutzers statt, es werden keine Daten an uns oder Dritte versendet. </br>
    </div>
    <div>
        <h1>Weitere Informationen</h1>
        <h2>Städtepartnerschaft</h2>
        <a href="https://www.wangen.de/buerger/stadt-ortschaften/partnerstaedte/la-garenne-colombes.html">Partnerschaft mit La Garenne-Colombes</a>
        <h2>Projekt</h2>
        <a href="https://fhv.at">Fachhochschule Vorarlberg</a></br>
    </div>
</body>