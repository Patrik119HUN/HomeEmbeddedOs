#ifndef NetworkManager_h
#define NetworkManager_h
#include <Arduino.h>
#include <Arduino_EthernetConnectionHandler.h>
#include <Arduino_WiFiConnectionHandler.h>

#include "../sysvar.h"
class NetworkManager {
   private:
    inline static NetworkManager* instance{nullptr};
    EthernetConnectionHandler* ethCon;
    WiFiConnectionHandler* wifiCon;
    UDP* udp;
    Client* client;
    NetworkConnectionState state[2];
    template <typename T>
    void setInterface(int num, ConnectionHandler* con) {
        T* connection = static_cast<T*>(con);
        if (this->getState(num) != NetworkConnectionState::CONNECTED) return;
        this->setUDP(&connection->getUDP());
        this->setClient(&connection->getClient());
    }
    NetworkManager() {
        wifiCon = new WiFiConnectionHandler(WIFI::ssid, WIFI::psw);
        ethCon = new EthernetConnectionHandler;
        this->udp = &ethCon->getUDP();
        this->client = &ethCon->getClient();
    }
    ~NetworkManager() = default;

   public:
    NetworkManager(const NetworkManager&) = delete;

    NetworkManager& operator=(const NetworkManager&) = delete;

    static NetworkManager* getInstance() {
        if (!instance) {
            instance = new NetworkManager();
        }
        return instance;
    }

    void loop(void) {
        this->state[0] = wifiCon->check();
        this->state[1] = ethCon->check();
        if (this->state[0] == NetworkConnectionState::DISCONNECTED ||
            this->state[1] == NetworkConnectionState::CONNECTED) {
            this->setInterface<EthernetConnectionHandler>(1,ethCon);
        } else if (this->state[1] == NetworkConnectionState::DISCONNECTED) {
            this->setInterface<WiFiConnectionHandler>(0,wifiCon);
        }
    }
    void setUDP(UDP* udp) { this->udp = udp; }
    void setClient(Client* client) { this->client = client; }
    UDP* getUDP() { return this->udp; }
    Client* getClient() { return this->client; }
    NetworkConnectionState getState(int i) { return this->state[i]; }
};

#endif  // NetworkManager