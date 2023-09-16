#pragma once
#include <Arduino.h>

#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "const.h"
#include "typedefs.h"
#include <string>
class INetworkAdapter {
  public:
    INetworkAdapter(std::string name, uint8_t priority, bool const keep_alive, NetworkAdapter interface)
        : _name{name}, _priority{priority}, _keep_alive{keep_alive}, _interface{interface},
          _current_net_connection_state{NetworkConnectionState::INIT} {}

    virtual Client* getClient() = 0;
    virtual UDP* getUDP() = 0;
    std::string getName() { return this->_name; }
    NetworkConnectionState getStatus() { return _current_net_connection_state; }
    void setStatus(NetworkConnectionState state) { this->_current_net_connection_state = state; }
    NetworkAdapter getInterface() { return _interface; }
    virtual int begin() = 0;
    void connect();
    void disconnect();

  protected:
    bool _keep_alive;
    NetworkAdapter _interface;
    std::string _name;
    uint8_t _priority;

  private:
    NetworkConnectionState _current_net_connection_state;
};
