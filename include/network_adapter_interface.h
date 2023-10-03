#pragma once
#include <Arduino.h>
#include <Client.h>
#include <Udp.h>
#include <network_exception.h>

#include <algorithm>
#include <map>
#include <optional>
#include <string>
enum class connectionState { INIT, CONNECTING, CONNECTED, DISCONNECTING, DISCONNECTED, CLOSED, ERROR };
enum class adapterType { NONE, WIFI, ETHERNET };

using std::string, std::nullopt;
using AdapterTypeMap = std::unordered_map<std::string, adapterType>;
struct AddressConfig {
    IPAddress ip;
    IPAddress dns;
    IPAddress gateway;
    IPAddress subnet;
};
const char* ip_to_string(const IPAddress& address) {
    char* buf = (char*)malloc(20 * sizeof(char));
    sprintf(buf, "%u.%u.%u.%u", address[0], address[1], address[2], address[3]);
    return buf;
}
using OptAddress = std::optional<AddressConfig>;
AdapterTypeMap adapterMap{{"none", adapterType::NONE}, {"wifi", adapterType::WIFI}, {"ethernet", adapterType::ETHERNET}};
class INetworkAdapter {
   public:
    INetworkAdapter(const string& name, uint8_t priority = 0, bool const keep_alive = true, OptAddress conf = nullopt, adapterType interface = adapterType::NONE)
        : m_name{name}, m_priority{priority}, m_keep_alive{keep_alive}, m_interface{interface}, m_static{false}, m_current_net_connection_state{connectionState::INIT} {
        if (conf) {
            m_static = true;
            this->config(conf.value());
        }
    }

    string getName() { return m_name; }
    connectionState getStatus() { return m_current_net_connection_state; }
    void setStatus(connectionState state) { m_current_net_connection_state = state; }
    adapterType getInterface() { return m_interface; }
    uint8_t get_priority() { return m_priority; }
    void connect();
    void disconnect();

    void config(AddressConfig conf) {
        m_ip = conf.ip;
        m_dns = conf.dns;
        m_gateway = conf.gateway;
        m_subnet = conf.subnet;
        m_static = true;
    }
    virtual Client* get_client() = 0;
    virtual UDP* get_udp() = 0;
    virtual int begin() = 0;

   protected:
    bool m_static;
    bool m_keep_alive;
    adapterType m_interface;
    uint8_t m_priority;
    string m_name;
    IPAddress m_ip;
    IPAddress m_dns;
    IPAddress m_gateway;
    IPAddress m_subnet;
    connectionState m_current_net_connection_state;
};
