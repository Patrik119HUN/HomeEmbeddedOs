#pragma once
#include "../kernel/network_manager/network_manager.h"

int network_handler_deamon(int argc, char** argv) {
    //ConnectionHandler* current = networkManager.getAdapter(0);
    Serial.println("Network handler");
        // while (current->getStatus() != NetworkConnectionState::CONNECTED) {
        //     for (uint8_t i = 0; i < networkManager->getHandlerCount(); i++) {
        //         ch = networkManager->getHandler(i);
        //         if (ch->getStatus() == NetworkConnectionState::CONNECTED) {
        //         }
        //     }
        //     networkManager->setClient(current->getClient());
        //     networkManager->setUDP(current->getUDP());
        //}
        //current->check();

    return 0;
}