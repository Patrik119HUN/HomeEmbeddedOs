#pragma once
#include "includes.h"
class ProcessManager {
  public:
    ProcessManager() = default;
    ~ProcessManager() = default;

  public:
    void startProcess(
        std::string t_name, ProcessPriority t_priority, function func,
        const char** params
    );
    inline void loop() {
        vTaskStartScheduler();
        Serial.println("Insufficient RAM");
        while (1)
            ;
    }
    void stop(const std::string task);
    void signal(const std::string_view task, int status);

  private:
    portBASE_TYPE* generateProcess(ProcessPriority t_priority, ProcessWrapper* proc);
    std::map<std::string, TaskHandle_t*> m_task_map;
};
extern ProcessManager processManager;

/*
TODO: Refactor it to unique pointers
Todo: no need for destructor
Todo: make Schedulers an array
Todo: use std::optional
Todo: use tuple and std::move
*/