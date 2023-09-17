#pragma once
#include <exception>

class dhcp_failed : public std::exception {
  public:
    const char* what() { return "Failed to connect via dhcp."; }
};
class no_hardware : public std::exception {
  public:
    const char* what() { return "There is no harware connected."; }
};