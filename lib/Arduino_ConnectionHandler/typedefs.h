#ifndef typedefs_h
#define typedefs_h

enum class NetworkConnectionState : unsigned int {
    INIT = 0,
    CONNECTING = 1,
    CONNECTED = 2,
    DISCONNECTING = 3,
    DISCONNECTED = 4,
    CLOSED = 5,
    ERROR = 6
};

enum class NetworkConnectionEvent { CONNECTED, DISCONNECTED, ERROR };

enum class NetworkAdapter { WIFI, ETHERNET, NB, GSM, LORA };

typedef void (*OnNetworkEventCallback)();
#endif // typedefs