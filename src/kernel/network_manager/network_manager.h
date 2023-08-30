#pragma once
#include <Arduino.h>
#include <Streaming.h>
#include <connection_handler.h>
#include <vector>

class NetworkManager {
  public:
    NetworkManager() = default;
    ~NetworkManager() = default;

  public:
    UDP* getUDP() { return this->m_udp; }
    Client* getClient() { return this->m_client; }

    void setUDP(UDP* t_udp) { m_udp = t_udp; }
    void setClient(Client* t_client) { m_client = t_client; }

    void removeAdapter(int t_n) { m_handler.erase(m_handler.begin() + t_n); }
    ConnectionHandler* getAdapter(int t_n) { return m_handler.at(t_n); }
    void addAdapter(ConnectionHandler* t_handler) { m_handler.push_back(t_handler); };

    uint8_t getAdapterCount() { return m_handler.size(); }
    void printAdapter(int t_elem) {
        Serial << "Link type: " << NetworkTypeString(m_handler.at(t_elem)->getInterface()) << endl;
    }

  private:
    UDP* m_udp;
    Client* m_client;
    std::vector<ConnectionHandler*> m_handler;
    const char* NetworkTypeString(NetworkAdapter elem) {
        switch (elem) {
        case NetworkAdapter::WIFI:
            return "WiFi";
        case NetworkAdapter::ETHERNET:
            return "Ethernet";
        default:
            break;
        }
        return nullptr;
    };
};
extern NetworkManager networkManager;