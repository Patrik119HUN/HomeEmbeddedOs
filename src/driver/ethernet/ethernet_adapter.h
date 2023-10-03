#pragma once
#include <Ethernet.h>
#include <network_adapter_interface.h>

#include <stdexcept>
using std::string;
class EthernetAdapter : public INetworkAdapter {
   public:
    EthernetAdapter(const string& name, bool keep_alive = true, OptAddress conf = nullopt) : INetworkAdapter{name, 0, keep_alive, conf, adapterType::ETHERNET} {}

    int begin() {
        Ethernet.init(PA8);
        if (m_static) {
            Ethernet.begin(mac, m_ip, m_dns, m_gateway, m_subnet);
            if (Ethernet.linkStatus() == Unknown) return 2;
        } else {
            if (Ethernet.linkStatus() != LinkON || Ethernet.begin(mac) == 0) return 3;
        }
        if (Ethernet.hardwareStatus() == EthernetNoHardware) return 1;
        return 0;
    }
    Client* get_client() { return &_eth_client; }
    UDP* get_udp() { return &_eth_udp; }

   private:
    byte mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetUDP _eth_udp;
    EthernetClient _eth_client;
};