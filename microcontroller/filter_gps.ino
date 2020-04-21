

bool filter_gps(float flat, float flon) {

  EEPROM.begin(512);                                        //Initialize EEPROM
  double lat_old;
  double lng_old;
  EEPROM.get(eeAddress, lat_old);                           //read old latitude from EEPROM
  EEPROM.get((eeAddress + sizeof(double)),lng_old);         //read old longitude from EEPROM

  
    double distanceM = gps.distanceBetween(                                           //calculate the distance between new and old waypoint
    (double)flat,
    (double)flon,
    lat_old,
    lng_old);
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
