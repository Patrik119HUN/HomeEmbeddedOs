#pragma once
#include <Arduino.h>
#include <Client.h>
#include <Udp.h>
#include <network_exception.h>
#include <string>

using std::string;

enum class connectionState {
    INIT,
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED,
    CLOSED,
    ERROR
};

enum class adapterType { WIFI, ETHERNET, NB, GSM, LORA };

class INetworkAdapter {
  public:
    INetworkAdapter(const string& name, uint8_t priority, bool const keep_alive, adapterType interface)
        : _name{name}, _priority{priority}, _keep_alive{keep_alive}, _interface{interface},
          _current_net_connection_state{connectionState::INIT} {}

    string getName() { return this->_name; }
    connectionState getStatus() { return _current_net_connection_state; }
    void setStatus(connectionState state) { this->_current_net_connection_state = state; }
    adapterType getInterface() { return _interface; }
    void connect();
    void disconnect();

    virtual Client* get_client() = 0;
    virtual UDP* get_udp() = 0;
    virtual int begin() = 0;

  protected:
    bool _keep_alive;
    adapterType _interface;
    string _name;
    uint8_t _priority;

  private:
    connectionState _current_net_connection_state;
};
