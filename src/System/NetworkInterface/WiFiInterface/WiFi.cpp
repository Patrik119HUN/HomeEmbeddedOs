#include "WiFi.h"

int WiFi::connect() {}
int WiFi::disconnect() {}
int WiFi::set_network(IPAddress ip, IPAddress gateway, IPAddress netmask) {}

int WiFi::set_dhcp(bool dhcp) {}
int WiFi::get_connection_status() {}

const char* WiFi::get_mac_addres() {}
int WiFi::set_mac_addres(uint8_t mac_addr, uint8_t addr_len) {}

const char* WiFi::get_ip_addres() {}
int WiFi::set_ip_addres(uint8_t mac_addr, uint8_t addr_len) {}

int WiFi::link_status(){};
int WiFi::hardware_status() {}