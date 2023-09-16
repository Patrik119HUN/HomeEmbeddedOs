#include "process_manager.h"
#include "exception.h"

void ProcessManager::startProcess(
    std::string t_name, ProcessPriority t_priority, function func, const char** params
) {
    TaskHandle_t* xHandle;
    portBASE_TYPE process =
        xTaskCreate(func, t_name.c_str(), configMINIMAL_STACK_SIZE + 800, NULL, 1, xHandle);
    m_task_map.insert(std::pair{t_name, xHandle});
}

void ProcessManager::stop(const std::string task) {
    auto res = m_task_map.find(task);
    if (res != m_task_map.end()) {
        auto process = res->second;
    }
}
portBASE_TYPE* ProcessManager::generateProcess(ProcessPriority t_priority, ProcessWrapper* proc) {
    portBASE_TYPE* task;
    return task;
}

ProcessManager processManager;