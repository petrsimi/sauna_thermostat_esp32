#pragma once

#include <Arduino.h>

#include <TFT_eSPI.h>

#include "FreeSerif8pt7b.h"

#include "FreeSans7pt7b.h"
#include "FreeSansBold72pt7b.h"
#include "FreeMonoBold76pt7b.h"


#define FM18 &FreeMono18pt7b
#define FMB18 &FreeMonoBold18pt7b
#define FMB76 &FreeMonoBold76pt7b



#define FS8 &FreeSerif8pt7b
#define FS9 &FreeSerif9pt7b
#define FS12 &FreeSerif12pt7b
#define FS18 &FreeSerif18pt7b
#define FS24 &FreeSerif24pt7b

#define FSB9 &FreeSerifBold9pt7b
#define FSB12 &FreeSerifBold12pt7b
#define FSB18 &FreeSerifBold18pt7b
#define FSB24 &FreeSerifBold24pt7b

#define FSS6 &FreeSans6pt7b
#define FSS7 &FreeSans7pt7b
#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b

#define FSSB9 &FreeSansBold9pt7b
#define FSSB12 &FreeSansBold12pt7b
#define FSSB18 &FreeSansBold18pt7b
#define FSSB24 &FreeSansBold24pt7b
#define FSSB72 &FreeSansBold72pt7b


typedef enum {
    ON,
    OFF,
    HEATING,
    WAITING,
} state_t;

typedef enum {
    SCREEN_STATUS,
    SCREEN_CONFIG,
    SCREEN_CONFIG_SSID,
    SCREEN_CONFIG_PWD
} screen_t;
