#include "process_manager.h"
#include <TaskScheduler.h>
ProcessManager::ProcessManager() {
    lowPriority = new Scheduler;
    basePriority = new Scheduler;
    highPriority = new Scheduler;
    lowPriority->setHighPriorityScheduler(basePriority);
    basePriority->setHighPriorityScheduler(highPriority);
}
ProcessManager::~ProcessManager() {
    free(lowPriority);
    free(highPriority);
}
void ProcessManager::startProcess(
    processSettings* settings, parametered_function function, const char** params
) {
    ProcessWrapper* proc = new ProcessWrapper(true);

    proc->setTask(function);
    proc->setArgv(const_cast<char**>(params));
    proc->setArgc(sizeof(params) / sizeof(char*));

    auto process = generateProcess(true, proc);
    m_task_map.insert(std::pair{settings->name, std::move(process)});
    lowPriority->addTask(*process.get());
}
void ProcessManager::stop(const char* task) {
    auto res = m_task_map.find(task);
    if (res != m_task_map.end()) {
        auto process = std::move(res->second);
        process->abort();
    }
}
std::unique_ptr<Task> ProcessManager::generateProcess(bool is_high_priority, ProcessWrapper* proc) {
    std::unique_ptr<Task> process;
    auto f1 = std::bind(&ProcessWrapper::func, proc);
    process = is_high_priority
                  ? std::make_unique<Task>(1000, TASK_ONCE, f1, highPriority, true)
                  : std::make_unique<Task>(1 * TASK_HOUR, TASK_ONCE, f1, lowPriority, true);
    return process;
}
bool ProcessManager::isMapped(const char* name) {
    if (m_task_map.find(name) != m_task_map.end()) {
        return true;
    }
    return false;
}
ProcessManager processManager;