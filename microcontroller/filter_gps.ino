

bool filter_gps(float flat, float flon, float lat_old, float lng_old) {

 // double distanceM = TinyGPSPlus.distanceBetween(
 double distanceM = gps.distanceBetween(
    (double)flat,
    (double)flon,
    (double)lat_old,
    (double)lng_old);
  Serial.print("distance between old and new waypoint in meters:");
  Serial.println(distanceM);

  if(distanceM > 50.0)
    {
     return true;
    }

  else
  {
   return false; 
  }
  
}

//a filter algorithm, which sets output to 1 when points are at least 50m distant
