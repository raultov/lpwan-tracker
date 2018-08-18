#ifndef _GLOBAL_
#define _GLOBAL_1

#define DEBUG false

typedef struct __attribute__ ((packed)) sigfox_message {
  float latitude;
  char nsIndicator;
  float longitude;
  char ewIndicator;
} SigfoxMessage;

#endif
