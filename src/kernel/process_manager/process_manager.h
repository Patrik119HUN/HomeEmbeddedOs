#ifndef process_manager_h
#define process_manager_h

#define _TASK_PRIORITY
#define _TASK_STD_FUNCTION
#define _TASK_SLEEP_ON_IDLE_RUN
#include "process_wrapper.h"
#include <Arduino.h>
#include <TaskScheduler.h>
#include <map>

typedef int (*parametered_function)(int, char**);
typedef int (*function)();
class ProcessManager {
  public:
    void startProcess(
        const char* t_name, bool t_is_high_priority, parametered_function t_func, int n, ...
    ) {
        if (m_task_map.find(t_name) != m_task_map.end()) {
            DEBUG_ERROR("%s is already running!!", t_name);
            return;
        }
        Task* process;
        ProcessWrapper* proc = new ProcessWrapper(t_func);

        va_list vl;
        va_start(vl, n);
        proc->setArgv(m_generate_params(vl, t_name, n));
        va_end(vl);

        proc->setArgc(n + 1);

        auto f1 = std::bind(&ProcessWrapper::func, proc);
        process = t_is_high_priority ? new Task(1000, TASK_ONCE, f1, highPriority, true)
                                     : new Task(1*TASK_HOUR, TASK_ONCE, f1, lowPriority, true);
        m_task_map.insert(std::pair{t_name, process});
        lowPriority->addTask(*process);
    }
    inline void loop() { lowPriority->execute(); }
    void stopProcess();

  private:
    char** m_generate_params(va_list t_vl, const char* t_name, int n) {
        char** fparam = (char**)malloc(sizeof(char*) * (n + 2));
        fparam[0] = (char*)t_name;
        fparam[n + 2] = nullptr;
        for (int i = 0; i < n; i++) {
            fparam[i + 1] = va_arg(t_vl, char*);
        }
        return fparam;
    }
    Scheduler* lowPriority;
    Scheduler* highPriority;
    std::map<const char*, Task*> m_task_map;
    ProcessManager() {
        lowPriority = new Scheduler;
        highPriority = new Scheduler;
        lowPriority->allowSleep(true);
        highPriority->allowSleep(true);
        lowPriority->setHighPriorityScheduler(highPriority);
    }
    ~ProcessManager() = default;

  private:
    inline static ProcessManager* m_instance{nullptr};

  public:
    ProcessManager(const ProcessManager&) = delete;

    ProcessManager& operator=(const ProcessManager&) = delete;

    static ProcessManager* getInstance() {
        if (!m_instance) {
            m_instance = new ProcessManager();
        }
        return m_instance;
    }
};

#endif // process_manager