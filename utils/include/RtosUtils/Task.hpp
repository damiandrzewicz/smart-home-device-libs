#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

class Task
{
public:

    enum TaskBit{
        Executed = BIT0
    };

    Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

    virtual ~Task();

    void start();

    void stop();

    bool isExecuted();

    void wait();

protected:

    virtual void task() = 0;

    static void taskWrapper(void *param);

    virtual void execute() = 0;

    void deleteTask();

    bool checkExecuted(bool block);

protected:
    xTaskHandle _handle;

    const char *_name;
    unsigned portBASE_TYPE _priority;
    unsigned short _stackDepth;
    bool _running = false;

    EventGroupHandle_t _events;
};