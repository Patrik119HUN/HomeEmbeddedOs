#pragma once
#include <Arduino.h>
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

    void removeHandler(int t_n) { m_handler.erase(m_handler.begin() + t_n); }
    ConnectionHandler* getHandler(int t_n) { return m_handler.at(t_n); }
    void addHandler(ConnectionHandler* t_handler) { m_handler.push_back(t_handler); };

    uint8_t getHandlerCount() { return m_handler.size(); }

  private:
    UDP* m_udp;
    Client* m_client;
    std::vector<ConnectionHandler*> m_handler;
};
