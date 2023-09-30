#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>
#include <Ethernet.h>
#include <SdFat.h>
#include <Udp.h>
#include <network_exception.h>

#include <algorithm>
#include <map>
#include <string>

using std::string;
void convertFromJson(JsonVariantConst src, IPAddress& dst) { dst.fromString(src.as<const char*>()); }
enum class connectionState { INIT, CONNECTING, CONNECTED, DISCONNECTING, DISCONNECTED, CLOSED, ERROR };
enum class adapterType { NONE, WIFI, ETHERNET };
std::unordered_map<string, adapterType> adapterMap{{"none", adapterType::NONE}, {"wifi", adapterType::WIFI}, {"ethernet", adapterType::ETHERNET}};
class INetworkAdapter {
    protected:
    INetworkAdapter(const string& name, StreamFile<ExFatFile, uint64_t>* file, uint8_t priority, bool const keep_alive, adapterType interface)
        : m_name{name}, m_priority{priority}, m_keep_alive{keep_alive}, m_interface{interface}, m_current_net_connection_state{connectionState::INIT} {
        DeserializationError error = deserializeJson(m_json, *file);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }
    }

   public:
    INetworkAdapter(const string& name, StreamFile<ExFatFile, uint64_t>* file, uint8_t priority) : INetworkAdapter{name, file, priority, true, adapterType::NONE} {
        m_static = m_json["static"];
        if (m_static) {
            m_ip = m_json["address"]["ip"].as<IPAddress>();
            m_dns = m_json["address"]["dns"].as<IPAddress>();
            m_gateway = m_json["address"]["gateway"].as<IPAddress>();
            m_subnet = m_json["address"]["subnet"].as<IPAddress>();
        }
        m_keep_alive = m_json["keep-alive"];
        m_sinterface = m_json["adapterType"].as<string>();
        std::transform(m_sinterface.begin(), m_sinterface.end(), m_sinterface.begin(), ::tolower);
        m_interface = adapterMap[m_sinterface.c_str()];
    }
    string getName() { return m_name; }
    connectionState getStatus() { return m_current_net_connection_state; }
    void setStatus(connectionState state) { m_current_net_connection_state = state; }
    adapterType getInterface() { return m_interface; }
    uint8_t get_priority() { return m_priority; }
    void connect();
    void disconnect();

    virtual Client* get_client() = 0;
    virtual UDP* get_udp() = 0;
    virtual int begin() = 0;

   protected:
    bool m_static;
    bool m_keep_alive;
    adapterType m_interface;
    uint8_t m_priority;
    string m_name;
    string m_sinterface;
    IPAddress m_ip;
    IPAddress m_dns;
    IPAddress m_gateway;
    IPAddress m_subnet;
    StaticJsonDocument<512> m_json;
    connectionState m_current_net_connection_state;
};
