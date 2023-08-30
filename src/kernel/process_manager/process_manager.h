#pragma once
#include "includes.h"
#include <memory>
class ProcessManager {
  public:
    ProcessManager();
    ~ProcessManager();

  public:
    void
    startProcess(processSettings* settings, parametered_function function, const char** params);
    inline void loop() { lowPriority->execute(); }
    void stop(const char* task);
    void signal(const char* task, int status);

  private:
    std::unique_ptr<Task> generateProcess(bool is_high_priority, ProcessWrapper* proc);
    bool isMapped(const char* name);
    Scheduler* lowPriority;
    Scheduler* basePriority;
    Scheduler* highPriority;
    std::map<const char*, std::unique_ptr<Task>> m_task_map;
};
extern ProcessManager processManager;

/*
TODO: Refactor it to unique pointers
Todo: no need for destructor
Todo: make Schedulers an array
Todo: use std::optional
Todo: use tuple and std::move
*/