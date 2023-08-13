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

#ifndef CONNECTION_HANDLER_H_
#define CONNECTION_HANDLER_H_

#include <Arduino.h>

#include <Ethernet.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "const.h"
#include "typedefs.h"
#include <debug.h>

class ConnectionHandler {
  public:
    ConnectionHandler(bool const keep_alive, NetworkAdapter interface)
        : _keep_alive{keep_alive}, _interface{interface}, _lastConnectionTickTime{millis()},
          _current_net_connection_state{NetworkConnectionState::INIT} {}

    NetworkConnectionState check();

    virtual Client& getClient() = 0;
    virtual UDP& getUDP() = 0;

    NetworkConnectionState getStatus() { return _current_net_connection_state; }

    NetworkAdapter getInterface() { return _interface; }

    void connect();
    void disconnect();

    void addCallback(NetworkConnectionEvent const event, OnNetworkEventCallback callback);
    void addConnectCallback(OnNetworkEventCallback callback);
    void addDisconnectCallback(OnNetworkEventCallback callback);
    void addErrorCallback(OnNetworkEventCallback callback);

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

#include "ethernet_handler/ethernet_handler.h"
#include "wifi_handler/wifi_handler.h"
#endif /* CONNECTION_HANDLER_H_ */
