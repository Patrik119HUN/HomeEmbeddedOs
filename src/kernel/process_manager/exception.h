#include <exception>
#include <string>
class UnableToStartProcess : public std::exception {
  public:
    UnableToStartProcess(std::string& process_name) { m_process_name = process_name; }
    const char* what() {
        std::string message = "Failed to start the process: ";
        std::string ret = message + m_process_name;
        return ret.c_str();
    }

  private:
    std::string m_process_name;
};