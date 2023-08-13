#include "ethernet_handler.h"
#include <debug.h>
NetworkConnectionState EthernetConnectionHandler::update_handleInit() {
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        DEBUG_ERROR("Error, ethernet shield was not found.");
        return NetworkConnectionState::ERROR;
    }
    return NetworkConnectionState::CONNECTING;
}

NetworkConnectionState EthernetConnectionHandler::update_handleConnecting() {
    if (_ip != INADDR_NONE) {
        Ethernet.begin(nullptr, _ip, _dns, _gateway, _netmask);
        if (Ethernet.linkStatus() == Unknown) {
            DEBUG_ERROR("Failed to configure Ethernet, check cable connection");
            return NetworkConnectionState::CONNECTING;
        }
    } else {
        if (Ethernet.begin(nullptr, 15000, 4000) == 0) {
            DEBUG_ERROR("Waiting Ethernet configuration from DHCP server, check cable connection");
            return NetworkConnectionState::CONNECTING;
        }
    }

    return NetworkConnectionState::CONNECTED;
}

NetworkConnectionState EthernetConnectionHandler::update_handleConnected() {
    if (Ethernet.linkStatus() == LinkOFF) {
        DEBUG_ERROR("Ethernet link OFF, connection lost.");
        if (_keep_alive) {
            DEBUG_ERROR("Attempting reconnection");
        }
        return NetworkConnectionState::DISCONNECTED;
    }
    return NetworkConnectionState::CONNECTED;
}

NetworkConnectionState EthernetConnectionHandler::update_handleDisconnecting() {
    return NetworkConnectionState::DISCONNECTED;
}

NetworkConnectionState EthernetConnectionHandler::update_handleDisconnected() {
    NetworkConnectionState state = (_keep_alive) ? NetworkConnectionState::INIT : NetworkConnectionState::CLOSED;
    return state;
}
