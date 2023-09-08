#include "process_manager.h"
#include "exception.h"
#include <TaskScheduler.h>
ProcessManager::ProcessManager() {
    lowPriority = new Scheduler;
    basePriority = new Scheduler;

    lowPriority->allowSleep(true);
    basePriority->allowSleep(true);

    lowPriority->setHighPriorityScheduler(basePriority);
}

void ProcessManager::startProcess(
    std::string t_name, ProcessPriority t_priority, parametered_function function,
    const char** params
) {
    ProcessWrapper* proc = new ProcessWrapper(true);
    proc->setTask(function);
    proc->setArgv(const_cast<char**>(params));
    proc->setArgc(sizeof(params) / sizeof(char*));
    Task* process = generateProcess(t_priority, proc);
    m_task_map.insert(std::pair{t_name, process});
    lowPriority->addTask(*process);
}

void ProcessManager::stop(const std::string task) {
    auto res = m_task_map.find(task);
    if (res != m_task_map.end()) {
        auto process = res->second;
        process->abort();
    }
}
Task* ProcessManager::generateProcess(ProcessPriority t_priority, ProcessWrapper* proc) {
    Task* process;
    auto f1 = std::bind(&ProcessWrapper::func, proc);
    process = new Task(10 * TASK_MILLISECOND, TASK_ONCE, f1, basePriority, true);
    return process;
}

ProcessManager processManager;