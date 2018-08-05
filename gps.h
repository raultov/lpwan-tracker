#ifndef _GPS_
#define _GPS_1

#define GPS_DEBUG 1

/**
 * Library to handle NEO-6M GPS module
 */

struct Point {
  String ggaLatitude = String("");
  String ggaLongitude = String("");
  String accuracy = String("");
  String altitude = String("");
};

/**
 * Sets NEO-6M GPS module up
 * 
 * MUST be called in the setup() method
 */
void gpsSetup();

/**
 * Deactivate default standard NMEA messages that NEO-6M module
 * outputs once a second.
 */
void gpsDeactivateStandardNMEAMessages();

/**
 * Returns a Point
 */
Point gpsGetPoint();

#endif
