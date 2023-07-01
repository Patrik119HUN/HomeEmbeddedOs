#ifndef WiFi_h
#define WiFi_h

#include <Arduino.h>
#include <SPI.h>

#include "../NetworkInterface.h"
#include "IPAddress.h"
#include "wl_definitions.h"
#include "wl_types.h"

class WiFi : public NetworkInterface {
   private:
    static void init();

    int connect();
    int disconnect();

    int set_network(IPAddress ip, IPAddress gateway, IPAddress netmask);

    int set_dhcp(bool dhcp);
    int get_connection_status();

    const char* get_mac_addres();
    int set_mac_addres(uint8_t mac_addr, uint8_t addr_len);

    const char* get_ip_addres();
    int set_ip_addres(uint8_t mac_addr, uint8_t addr_len);

    int link_status();
    int hardware_status();

   public:
    WiFi();

    void setPins(
        int8_t cs = 10, int8_t ready = 7, int8_t reset = 5, int8_t gpio0 = 6, SPIClass* spi = &SPI
    );
};

#endif