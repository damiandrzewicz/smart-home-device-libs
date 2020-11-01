#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



class Task
{
public:

    Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

    ~Task();

    void start();

    void stop();

protected:

    virtual void task() = 0;

    static void taskWrapper(void *param);

    virtual void execute() = 0;

    void deleteTask();

protected:
    xTaskHandle _handle;

    const char *_name;
    unsigned portBASE_TYPE _priority;
    unsigned short _stackDepth;
    bool _running = false;
};