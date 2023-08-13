#ifndef network_handler_deamon_h
#define network_handler_deamon_h
#include "../kernel/network_manager/network_manager.h"

int network_handler(int argc, char** argv) {
    NetworkManager* networkManager = NetworkManager::getInstance();
    ConnectionHandler* current = networkManager->getHandler(0);
    ConnectionHandler* ch;
    current->check();

    while (true) {
        while (current->getStatus() != NetworkConnectionState::CONNECTED) {
            for (uint8_t i = 0; i < networkManager->getHandlerCount(); i++) {
                ch = networkManager->getHandler(i);
                if (ch->getStatus() == NetworkConnectionState::CONNECTED) {
                }
            }
            networkManager->setClient(current->getClient());
            networkManager->setUDP(current->getUDP());
        }
    }

    return 0;
}
#endif // network_handler_deamon