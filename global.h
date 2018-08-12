#ifndef _GLOBAL_
#define _GLOBAL_1

#define DEBUG 1

#ifdef DEBUG
#define SERIAL_DEBUG_PRINT(X) Serial.print(X)
#else
#define SERIAL_DEBUG_PRINT(X) { // nothing to do }
#endif

typedef struct __attribute__ ((packed)) sigfox_message {
  float latitude;
  char nsIndicator;
  float longitude;
  char ewIndicator;
} SigfoxMessage;

#endif
