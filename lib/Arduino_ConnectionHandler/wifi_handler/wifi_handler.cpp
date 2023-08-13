#include "wifi_handler.h"

NetworkConnectionState WiFiConnectionHandler::update_handleInit() {
    DEBUG_INFO("WiFi.status(): %d", WiFi.status());

    WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);

    if (WiFi.status() == WL_NO_MODULE) {
        DEBUG_ERROR("WiFi Hardware failure.\nMake sure you are using a WiFi enabled board/shield.");
        DEBUG_ERROR("Then reset and retry.");
        return NetworkConnectionState::ERROR;
    }
    DEBUG_INFO("Current WiFi Firmware: %s", WiFi.firmwareVersion());

    return NetworkConnectionState::CONNECTING;
}

NetworkConnectionState WiFiConnectionHandler::update_handleConnecting() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(_ssid, _pass);
    }

    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_ERROR("Connection to %s failed", _ssid);
        DEBUG_INFO(
            "Retrying in  %d milliseconds",
            CHECK_INTERVAL_TABLE[static_cast<unsigned int>(NetworkConnectionState::CONNECTING)]
        );
        return NetworkConnectionState::CONNECTING;
    } else {
        DEBUG_INFO("Connected to %s", _ssid);
        return NetworkConnectionState::CONNECTED;
    }
}

NetworkConnectionState WiFiConnectionHandler::update_handleConnected() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_VERBOSE("WiFi.status(): %d", WiFi.status());
        DEBUG_ERROR("Connection to \"%s\" lost.", _ssid);
        if (_keep_alive) {
            DEBUG_INFO("Attempting reconnection");
        }

        return NetworkConnectionState::DISCONNECTED;
    }
    return NetworkConnectionState::CONNECTED;
}

NetworkConnectionState WiFiConnectionHandler::update_handleDisconnecting() {
    WiFi.disconnect();
    return NetworkConnectionState::DISCONNECTED;
}

NetworkConnectionState WiFiConnectionHandler::update_handleDisconnected() {
    WiFi.end();
    NetworkConnectionState state = (_keep_alive) ? NetworkConnectionState::INIT : NetworkConnectionState::CLOSED;
    return state;
}

