#pragma once
#include "includes.h"
#include <memory>
#include <tuple>
class ProcessManager {
  public:
    ProcessManager();
    ~ProcessManager() = default;

  public:
    void
    startProcess(processSettings* settings, parametered_function function, const char** params);
    inline void loop() { lowPriority->execute(); }
    void stop(const std::string task);
    void signal(const std::string_view task, int status);

  private:
    std::unique_ptr<Task> generateProcess(bool is_high_priority, ProcessWrapper* proc);
    Scheduler* lowPriority;
    Scheduler* basePriority;
    Scheduler* highPriority;
    std::map<std::string, std::unique_ptr<Task>> m_task_map;
};
extern ProcessManager processManager;

/*
TODO: Refactor it to unique pointers
Todo: no need for destructor
Todo: make Schedulers an array
Todo: use std::optional
Todo: use tuple and std::move
*/