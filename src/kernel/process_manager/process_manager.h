#pragma once
#include "process_priority.h"
#include "typedef.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <map>
#include <sys/log.h>
#define INFO(str) syslog(&Serial, Debug_level::INFO, "Starting %s process", str);
using std::string, std::pair, std::string_view;
class ProcessManager {
  public:
    ProcessManager() = default;
    ~ProcessManager() = default;

  public:
    void startProcess(
        const string& t_name, function func, void* params = NULL, ProcessPriority t_priority = BASE,
        uint16_t stack_size = 900
    ) {
        INFO(t_name.c_str());
        TaskHandle_t* xHandle = new TaskHandle_t;
        portBASE_TYPE process =
            xTaskCreate(func, t_name.c_str(), stack_size, params, t_priority, xHandle);
        m_task_map.insert(pair{t_name, xHandle});
    }
    inline void loop() {
        vTaskStartScheduler();
        Serial.println("Insufficient RAM");
        while (1)
            ;
    }
    void stop(const string& task) {
        auto res = m_task_map.find(task);
        if (res != m_task_map.end()) {
            auto process = res->second;
        }
    }
    void signal(const string_view task, int status);

  private:
    std::map<string, TaskHandle_t*> m_task_map;
};
ProcessManager processManager;
