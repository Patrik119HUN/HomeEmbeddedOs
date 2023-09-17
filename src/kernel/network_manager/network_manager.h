#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <map>
#include <network_adapter_interface.h>
#include <string>
class NetworkManager {
  public:
    NetworkManager() = default;
    ~NetworkManager() = default;

  public:
    UDP* getUDP() { return this->m_udp; }
    Client* getClient() { return this->m_client; }

    void setStack(std::string t_name) {
        this->m_udp = m_handler.at(t_name)->getUDP();
        this->m_client = m_handler.at(t_name)->getClient();
    }
    void removeAdapter(std::string t_name) { m_handler.erase(t_name); }
    INetworkAdapter* getAdapter(std::string t_name) { return m_handler.at(t_name); }
    void addAdapter(INetworkAdapter* t_handler) {
        m_handler.insert(std::pair{t_handler->getName(), t_handler});
    };

    uint8_t getAdapterCount() { return m_handler.size(); }
    void printAdapter(std::string t_name) {
        Serial << "Link type: " << NetworkTypeString(m_handler.at(t_name)->getInterface()) << endl;
    }

  private:
    UDP* m_udp;
    Client* m_client;
    std::unordered_map<std::string, INetworkAdapter*> m_handler;
    const char* NetworkTypeString(adapterType elem) {
        switch (elem) {
        case adapterType::WIFI:
            return "WiFi";
        case adapterType::ETHERNET:
            return "Ethernet";
        default:
            break;
        }
        return nullptr;
    };
};
extern NetworkManager networkManager;