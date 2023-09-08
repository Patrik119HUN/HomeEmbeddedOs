#pragma once
#include "includes.h"
class ProcessManager {
  public:
    ProcessManager();
    ~ProcessManager() = default;

  public:
    void startProcess(
        std::string t_name, ProcessPriority t_priority, parametered_function function,
        const char** params
    );
    inline void loop() { lowPriority->execute(); }
    void stop(const std::string task);
    void signal(const std::string_view task, int status);
    Task* get(std::string name){
      return m_task_map.at(name);
    }
  private:
    Task* generateProcess(ProcessPriority t_priority, ProcessWrapper* proc);
    Scheduler* lowPriority;
    Scheduler* basePriority;
    std::map<std::string, Task*> m_task_map;
};
extern ProcessManager processManager;

/*
TODO: Refactor it to unique pointers
Todo: no need for destructor
Todo: make Schedulers an array
Todo: use std::optional
Todo: use tuple and std::move
*/