/*
   This file is part of ArduinoIoTCloud.

   Copyright 2019 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

#ifndef ARDUINO_CONNECTION_HANDLER_H_
#define ARDUINO_CONNECTION_HANDLER_H_

#include <Arduino.h>

#include <Ethernet.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#define SPIWIFI SPI
#define SPIWIFI_SS PA4   // Chip select pin
#define SPIWIFI_ACK PB0  // a.k.a BUSY or READY pin
#define ESP32_RESETN PC1 // Reset pin
#define ESP32_GPIO0 -1   // Not connected

/******************************************************************************
   INCLUDES
 ******************************************************************************/

#if !defined(__AVR__)
#include <debug.h>
#endif

/******************************************************************************
   TYPEDEFS
 ******************************************************************************/

enum class NetworkConnectionState : unsigned int {
    INIT = 0,
    CONNECTING = 1,
    CONNECTED = 2,
    DISCONNECTING = 3,
    DISCONNECTED = 4,
    CLOSED = 5,
    ERROR = 6
};

enum class NetworkConnectionEvent { CONNECTED, DISCONNECTED, ERROR };

enum class NetworkAdapter { WIFI, ETHERNET, NB, GSM, LORA };

typedef void (*OnNetworkEventCallback)();

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static unsigned int const CHECK_INTERVAL_TABLE[] = {
    /* INIT          */ 100,
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
    /* CONNECTING    */ 4000,
#else
    /* CONNECTING    */ 500,
#endif
    /* CONNECTED     */ 10000,
    /* DISCONNECTING */ 100,
    /* DISCONNECTED  */ 1000,
    /* CLOSED        */ 1000,
    /* ERROR         */ 1000};

/******************************************************************************
   CLASS DECLARATION
 ******************************************************************************/

class ConnectionHandler {
  public:
    ConnectionHandler(bool const keep_alive, NetworkAdapter interface);

    NetworkConnectionState check();

#if defined(BOARD_HAS_WIFI) || defined(BOARD_HAS_GSM) || defined(BOARD_HAS_NB) ||                  \
    defined(BOARD_HAS_ETHERNET)
    virtual unsigned long getTime() = 0;
    virtual Client& getClient() = 0;
    virtual UDP& getUDP() = 0;
#endif

    NetworkConnectionState getStatus() __attribute__((deprecated)) {
        return _current_net_connection_state;
    }

    NetworkAdapter getInterface() { return _interface; }

    void connect();
    void disconnect();

    void addCallback(NetworkConnectionEvent const event, OnNetworkEventCallback callback);
    void addConnectCallback(OnNetworkEventCallback callback) __attribute__((deprecated));
    void addDisconnectCallback(OnNetworkEventCallback callback) __attribute__((deprecated));
    void addErrorCallback(OnNetworkEventCallback callback) __attribute__((deprecated));

  protected:
    bool _keep_alive;
    NetworkAdapter _interface;

    virtual NetworkConnectionState update_handleInit() = 0;
    virtual NetworkConnectionState update_handleConnecting() = 0;
    virtual NetworkConnectionState update_handleConnected() = 0;
    virtual NetworkConnectionState update_handleDisconnecting() = 0;
    virtual NetworkConnectionState update_handleDisconnected() = 0;

  private:
    unsigned long _lastConnectionTickTime;
    NetworkConnectionState _current_net_connection_state;
    OnNetworkEventCallback _on_connect_event_callback = NULL, _on_disconnect_event_callback = NULL,
                           _on_error_event_callback = NULL;
};

#if defined(BOARD_HAS_WIFI)
#include "Arduino_WiFiConnectionHandler.h"
#endif

#if defined(BOARD_HAS_ETHERNET)
#include "Arduino_EthernetConnectionHandler.h"
#endif

#endif /* CONNECTION_HANDLER_H_ */
