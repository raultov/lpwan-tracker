#ifndef _GLOBAL_
#define _GLOBAL_1

#define SERIAL_DEBUG 1

#ifdef SERIAL_DEBUG
#define SERIAL_DEBUG_PRINT(X) Serial.print(X)
#else
#define SERIAL_DEBUG_PRINT(X) { // nothing to do }
#endif

#endif
