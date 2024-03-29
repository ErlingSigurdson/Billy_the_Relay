/****************** DESCRIPTION *****************/

/**
 * Filename: ESP32_Bluetooth.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: Bluetooth Classic-related functions for ESP32 modules.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef ESP32_BLUETOOTH_H
#define ESP32_BLUETOOTH_H


/*--- Includes ---*/

// General Arduino library.
#include <Arduino.h>

// Project configs.
#include "config_general.h"


/*--- Misc ---*/

// Conditional compilation.
#if defined ESP32 && defined BT_CLASSIC_PROVIDED


/************** FUNCTION PROTOTYPES *************/

void ESP32_BT_start(char *dev_name);
bool ESP32_BT_check_connection();
uint32_t ESP32_BT_read_line(char *buf, uint32_t str_max_len, uint32_t conn_timeout);
void ESP32_BT_send_msg(const char *msg);
void ESP32_BT_disconnect(uint32_t shutdown_downtime);
void ESP32_BT_stop(uint32_t shutdown_downtime);


#endif  // Conditional compilation.
#endif  // Include guards.
