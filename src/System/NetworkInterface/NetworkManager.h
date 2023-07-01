#ifndef NetworkManager_h
#define NetworkManager_h
#include <Arduino.h>

class NetworkInterface {
   private:
    inline static NetworkInterface* instance{nullptr};
    NetworkInterface() = default;
    ~NetworkInterface() = default;

   public:
    NetworkInterface(const NetworkInterface&) = delete;

    NetworkInterface& operator=(const NetworkInterface&) = delete;

    static NetworkInterface* getInstance() {
        if (!instance) {
            instance = new NetworkInterface();
        }
        return instance;
    }
    int connect();
    int disconnect();
    int set_dhcp(bool dhcp);
    int get_connection_status();

    const char* get_mac_addres();
    int set_mac_addres(uint8_t mac_addr, uint8_t addr_len);

    const char* get_ip_addres();
    int set_ip_addres(uint8_t mac_addr, uint8_t addr_len);
};

#endif  // NetworkManager