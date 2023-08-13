#include "network_manager.h"

NetworkManager* NetworkManager::getInstance() {
    if (!m_instance) {
        m_instance = new NetworkManager();
    }
    return m_instance;
}