

bool filter_gps(float flat, float flon) {

  EEPROM_STRUCT readData = {0.0,0.0,""};
  
  EEPROM.begin(512);                                        //Initialize EEPROM
  EEPROM.get(eeAddress, readData);                           //read old waypoint from EEPROM
  EEPROM.end();
    
    double distanceM = gps.distanceBetween(                                           //calculate the distance between new and old waypoint
    (double)flat,
    (double)flon,
    readData.Elat,
    readData.Elon);
    Serial.print("distance between old and new waypoint in meters:");
    Serial.println(distanceM);

    if(distanceM > 50.0)                                                              //check if distance is greater than 50m
      {
      return true;
      }

    else
      {
      return false; 
      }
  
}

//a filter algorithm, which sets output to true when points are at least 50m distant

/*
 * Parameter: -float:      flat Latitude of GPS-Position
 *            -float:      flon Longitude of GPS-Position
 *
 *            
 *            
 *  Return values:bool           
 *            -false: waypoints are less than 50m distant
 *            -true:  waypoints are at least 50m distant
 */
