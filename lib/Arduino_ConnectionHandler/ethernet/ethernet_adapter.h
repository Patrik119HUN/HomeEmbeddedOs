#pragma once
#include "../connection_handler.h"

#include <Ethernet.h>

class EthernetAdapter : public INetworkAdapter {
  public:
    EthernetAdapter(std::string name, bool const keep_alive = true)
        : INetworkAdapter{name, 0, keep_alive, NetworkAdapter::ETHERNET}, _ip{INADDR_NONE},
          _dns{INADDR_NONE}, _gateway{INADDR_NONE}, _netmask{INADDR_NONE} {}

    EthernetAdapter(
        std::string name, const IPAddress ip, const IPAddress dns, const IPAddress gateway,
        const IPAddress netmask, bool const keep_alive = true
    )
        : INetworkAdapter{name, 0, keep_alive, NetworkAdapter::ETHERNET}, _ip{ip}, _dns{dns},
          _gateway{gateway}, _netmask{netmask} {}

    int begin() override {
        Ethernet.init(PA9);
        if (_ip != INADDR_NONE) {
            Ethernet.begin(mac, _ip, _dns, _gateway, _netmask);
            if (Ethernet.linkStatus() == Unknown) return 2;
        } else {
            if (Ethernet.begin(mac) == 0) return 3;
        }
        if (Ethernet.hardwareStatus() == EthernetNoHardware) return 1;
        return 0;
    }
    virtual Client* getClient() override { return &_eth_client; }
    virtual UDP* getUDP() override { return &_eth_udp; }

  private:
    byte mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    IPAddress _ip;
    IPAddress _dns;
    IPAddress _gateway;
    IPAddress _netmask;

    EthernetUDP _eth_udp;
    EthernetClient _eth_client;
};