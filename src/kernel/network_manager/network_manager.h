#pragma once

#include <Arduino.h>
#include <memory.h>
#include <network_adapter_interface.h>
#include <types.h>

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
using std::string, std::pair;
using NetworkAdapterPtr = std::shared_ptr<INetworkAdapter>;
using NetworkAdapterMap = std::unordered_map<std::string, NetworkAdapterPtr>;
class NetworkManager {
   public:
    NetworkManager() : m_client(nullptr), m_udp(nullptr) {}
    ~NetworkManager() = default;

   public:
    UDP* get_udp() { return this->m_udp; }
    Client* get_client() { return this->m_client; }

    void set_stack(string t_name) {
        this->m_udp = m_handler.at(t_name)->get_udp();
        this->m_client = m_handler.at(t_name)->get_client();
    }
    void remove_adapter(string t_name) { m_handler.erase(t_name); }

    NetworkAdapterPtr get_adapter(string t_name) { return m_handler.at(t_name); }
    void add_adapter(NetworkAdapterPtr t_handler) {
        m_adapter_names.push_back(t_handler->get_name());
        m_handler.insert(pair{t_handler->get_name(), t_handler});
    };

    const StringVector& get_adapters() const { return m_adapter_names; }

   private:
    UDP* m_udp;
    Client* m_client;
    NetworkAdapterMap m_handler;
    StringVector m_adapter_names;
};
NetworkManager networkManager;