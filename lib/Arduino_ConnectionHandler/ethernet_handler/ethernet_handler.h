#ifndef ETHERNET_HANDLER_H_
#define ETHERNET_HANDLER_H_

#include "../connection_handler.h"

class EthernetConnectionHandler : public ConnectionHandler {
  public:
    EthernetConnectionHandler(bool const keep_alive = true)
        : ConnectionHandler{keep_alive, NetworkAdapter::ETHERNET}, _ip{INADDR_NONE},
          _dns{INADDR_NONE}, _gateway{INADDR_NONE}, _netmask{INADDR_NONE} {}

    EthernetConnectionHandler(
        const IPAddress ip, const IPAddress dns, const IPAddress gateway, const IPAddress netmask,
        bool const keep_alive = true
    )
        : ConnectionHandler{keep_alive, NetworkAdapter::ETHERNET}, _ip{ip}, _dns{dns},
          _gateway{gateway}, _netmask{netmask} {}
    EthernetConnectionHandler(
        const char* ip, const char* dns, const char* gateway, const char* netmask,
        bool const keep_alive = true
    )
        : ConnectionHandler{keep_alive, NetworkAdapter::ETHERNET}, _ip{INADDR_NONE},
          _dns{INADDR_NONE}, _gateway{INADDR_NONE}, _netmask{INADDR_NONE} {
        if (!_ip.fromString(ip)) {
            _ip = INADDR_NONE;
        }
        if (!_dns.fromString(dns)) {
            _dns = INADDR_NONE;
        }
        if (!_gateway.fromString(gateway)) {
            _gateway = INADDR_NONE;
        }
        if (!_netmask.fromString(netmask)) {
            _netmask = INADDR_NONE;
        }
    }

    virtual Client& getClient() override { return _eth_client; }
    virtual UDP& getUDP() override { return _eth_udp; }

  protected:
    virtual NetworkConnectionState update_handleInit() override;
    virtual NetworkConnectionState update_handleConnecting() override;
    virtual NetworkConnectionState update_handleConnected() override;
    virtual NetworkConnectionState update_handleDisconnecting() override;
    virtual NetworkConnectionState update_handleDisconnected() override;

  private:
    IPAddress _ip;
    IPAddress _dns;
    IPAddress _gateway;
    IPAddress _netmask;

    EthernetUDP _eth_udp;
    EthernetClient _eth_client;
};

#endif /* ARDUINO_ETHERNET_CONNECTION_HANDLER_H_ */
