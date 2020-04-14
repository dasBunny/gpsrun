bool wifi_send(float flat, float flon, const char* host, const int httpPort) {
/*
 * Parameter: -float:      flat Latitude of GPS-Position
 *            -float:      flon Longitude of GPS-Position
 *            -char:       host Hostname of server
 *            -httpPort:   port of server
 *            
 *            
 *  Return values:bool           
 *            -false: communication to server failed
 *            -true:  communication succeded
 */

Serial.print("connecting to ");
    Serial.println(host); // Use WiFiClient class to create TCP connections
    WiFiClient client;
   
    if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false; }

//String url =  "/lat="+(String)flat+"/lng="+(String)flon+"/id="+DEV_ID/"TOKEN="+TOKEN; pfusch!

    String url = "/";   //building url for http-get message
    url += "lat=";
    url += String(flat,6);
    url += "/lng=";
    url += String(flon,6);
    url += "/id=";
    url += dev_id;
    url += "/token=";
    url += token;

    Serial.print("Requesting URL: ");
    Serial.println(url); // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 5000)
    { Serial.println(F(">>> Client Timeout !"));
    client.stop(); return false; } } // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    { String line = client.readStringUntil('\r'); Serial.print(line);
    }
    Serial.println();
    Serial.println(F("closing connection"));
    return true;

  
}
