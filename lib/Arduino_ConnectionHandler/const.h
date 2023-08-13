#ifndef const_h
#define const_h

#define SPIWIFI SPI
#define SPIWIFI_SS PA4   // Chip select pin
#define SPIWIFI_ACK PB0  // a.k.a BUSY or READY pin
#define ESP32_RESETN PC1 // Reset pin
#define ESP32_GPIO0 -1   // Not connected
static unsigned int const CHECK_INTERVAL_TABLE[] = {
    /* INIT          */ 100,
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
    /* CONNECTING    */ 4000,
#else
    /* CONNECTING    */ 500,
#endif
    /* CONNECTED     */ 10000,
    /* DISCONNECTING */ 100,
    /* DISCONNECTED  */ 1000,
    /* CLOSED        */ 1000,
    /* ERROR         */ 1000};

#endif // const