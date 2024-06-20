/****************** DESCRIPTION *****************/

/**
 * Filename: cmd.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Purpose: text commands processing.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CMD_H
#define CMD_H


/*--- Includes ---*/

// Essential Arduino library.
#include <Arduino.h>

// Local modules.
#include "stored_configs.h"


/*--- Misc ---*/

/* To print or not to print a new value of the changed config.
 * OFF for password, ON for other cases.
 */
#define ECHO_VAL_ON 1
#define ECHO_VAL_OFF 0


/************** FUNCTION PROTOTYPES *************/


/*--- Buffer contents check ---*/

// Look up for a valid prefix and for valid commands from a text commands list.
int32_t cmd_check(const char *buf, const char *prefix, const char *cmd_list[], uint32_t cmd_list_len);


/*--- Auxiliary functions (helper functions, accessories) ---*/

/* A generic accessory called by the others.
 * Prints a message over a UART and sends it over the wireless connections.
 */
void cmd_aux_output(const char *msg);

// Accessories for handler functions.
void cmd_aux_set_digital_output(uint8_t pin, uint8_t state, const char *topic);
void cmd_aux_set_PWM(uint8_t pin, uint32_t val, const char *topic);
void cmd_aux_set_config(const char *cmd, uint32_t addr, const char *topic, bool echo_val, bool *refresh_flag);
void cmd_aux_output_config(uint32_t addr, const char *topic);


/*--- Handler functions ---*/

/* Functions called in a response to received commands
 * and command processing errors.
 */

// Incoming byte string is too long.
void cmd_handler_err_len();

// Invalid/absent command prefix.
void cmd_handler_err_prefix();

// No valid command after a prefix.
void cmd_handler_err_cmd();

// No valid value after a command which requires it.
void cmd_handler_err_val();

/* Command #1:
 * turn a digital (two-state) load ON or OFF. A main workhorse.
 * A prescribed state of the load is NOT stored in the inbuilt storage.
 */
void cmd_handler_set_load_digital(const char *cmd);

/* Command #2:
 * drive a load using PWM. Another main workhorse.
 * Valid values of a duty cycle are 0 to 255.
 * A prescribed duty cycle value is NOT stored in the inbuilt storage.
 */
void cmd_handler_set_load_PWM(const char *cmd);

/* Command #3:
 * print and send to a client/master the current state of a digital (two-state) load.
 * The actual output is based on a return value of the digitalRead().
 */
void cmd_handler_output_load_digital();

/* Command #4:
 * change an SSID of a Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_WiFi_SSID(const char *cmd, bool *refresh_flag);

/* Command #5:
 * print and send to a client/master an SSID of a Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_output_WiFi_SSID();

/* Command #6:
 * change a password for a Wi-Fi access point
 * stored in the inbuilt storage.
 */
void cmd_handler_set_WiFi_pswd(const char *cmd, bool *refresh_flag);

/* Command #7:
 * turn periodical printing and sending of a current RSSI value ON or OFF.
 */
void cmd_handler_set_WiFi_RSSI_output_flag(const char *cmd, bool *refresh_flag);

/* Command #8:
 * turn periodical attempts to reconnect to a Wi-Fi network ON or OFF.
 */
void cmd_handler_set_WiFi_autoreconnect_flag(const char *cmd, bool *refresh_flag);

/* Command #9:
 * print and send to a client/master the device's IP in a Wi-Fi network.
 */
void cmd_handler_output_local_server_IP();

/* Command #10:
 * change a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_set_local_server_port(const char *cmd, bool *refresh_flag);

/* Command #11:
 * print and send to a client/master a local TCP server port number
 * stored in the inbuilt storage.
 */
void cmd_handler_output_local_server_port();

/* Command #12:
 * set the IoT mode (attempts to connect to a remote server) ON or OFF.
 */
void cmd_handler_set_IoT_flag(const char *cmd, bool *refresh_flag);

/* Command #13:
 * change an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_IP(const char *cmd, bool *refresh_flag);

/* Command #14:
 * print and send to a client/master an IP address of a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_IP();

/* Command #15:
 * change a port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_server_port(const char *cmd, bool *refresh_flag);

/* Command #16:
 * print and send to a client/master a port number used for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_server_port();

/* Command #17:
 * change a request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_msg(const char *cmd, bool *refresh_flag);

/* Command #18:
 * print and send to a client/master a request message to be sent to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_output_IoT_req_msg();

/* Command #19:
 * change the interval (in ms) for sending requests to a remote server
 * stored in the inbuilt storage.
 */
void cmd_handler_set_IoT_req_period(const char *cmd, bool *refresh_flag);

/* Command #20:
 * set Bluetooth Classic functionality ON or OFF.
 */
void cmd_handler_set_BTClassic_flag(const char *cmd,
                                    void (*setup_BTClassic_ptr)(stored_configs_t *stored_configs),
                                    stored_configs_t *stored_configs,
                                    bool *refresh_flag);

/* Command #21:
 * change a name of the ESP as a Bluetooth Classic slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_set_BTClassic_dev_name(const char *cmd,
                                        void (*setup_BTClassic_ptr)(stored_configs_t *stored_configs),
                                        stored_configs_t *stored_configs,
                                        bool *refresh_flag);

/* Command #22:
 * print and send to a client/master a name of the ESP as a Bluetooth Classic slave device
 * stored in the inbuilt storage.
 */
void cmd_handler_output_BTClassic_dev_name();

/* Command #23:
 * reset all wireless connections and restart the wireless connectivity.
 */
void cmd_handler_all_conn_rst(void (*setup_WiFi_ptr)(stored_configs_t *stored_configs),
                              void (*setup_BTClassic_ptr)(stored_configs_t *stored_configs),
                              stored_configs_t *stored_configs);


#endif  // Include guards.
