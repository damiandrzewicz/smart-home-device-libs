#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

class Task
{
public:

    enum TaskBit{
        Executed = BIT0,
        Initialized = BIT1
    };

    Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

    virtual ~Task();

    void start();

    void stop();

    bool isExecuted();

    bool isInitialized();

    void waitForInitialized();

    void waitForExecuted();

protected:

    virtual void task() = 0;

    virtual void initTask();

    static void taskWrapper(void *param);

    virtual void execute() = 0;

    void deleteTask();

    bool checkInitialized(bool block);

    bool checkExecuted(bool block);

    void sleep(unsigned int delay);

protected:
    xTaskHandle _handle;

    const char *_name;
    unsigned portBASE_TYPE _priority;
    unsigned short _stackDepth;
    bool _running = false;

    EventGroupHandle_t _events;
};