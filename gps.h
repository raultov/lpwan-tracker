#ifndef _GPS_
#define _GPS_1

/**
 * Library to handle NEO-6M GPS module
 */

struct Point {
  String ggaLatitude = String("");
  char northSouthIndicator = '\0';
  String ggaLongitude = String("");
  char eastWestIndicator = '\0';
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
 * Fills a Point
 */
void gpsFillPoint(Point & point);

/**
 * Returns true if proper coordinates are still not set in point.
 */
boolean areCoordinatesStillNotFetched(Point point);

#endif
