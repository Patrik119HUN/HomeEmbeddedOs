#ifndef NetworkInterface_h
#define NetworkInterface_h

#include <Arduino.h>

class NetworkInterface {
   private:

   public:
    NetworkInterface() = default;
    ~NetworkInterface() = default;

    virtual int connect() = 0;
    virtual int disconnect() = 0;

    virtual int set_network(IPAddress ip,IPAddress gateway, IPAddress netmask) = 0;

    virtual int set_dhcp(bool dhcp) = 0;
    virtual int get_connection_status() = 0;

    virtual const char* get_mac_addres();
    virtual int set_mac_addres(uint8_t mac_addr, uint8_t addr_len);

    virtual const char* get_ip_addres();
    virtual int set_ip_addres(uint8_t mac_addr, uint8_t addr_len);

    virtual int link_status();
    virtual int hardware_status();

};

#endif  // NetworkInterface