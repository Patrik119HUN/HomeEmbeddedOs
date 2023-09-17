#pragma once
enum class NetworkConnectionState {
    INIT,
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED,
    CLOSED,
    ERROR
};

enum class NetworkConnectionEvent { CONNECTED, DISCONNECTED, ERROR };

enum class NetworkAdapter { WIFI, ETHERNET, NB, GSM, LORA };
