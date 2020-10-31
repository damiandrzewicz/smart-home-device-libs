#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

class Task
{
public:

    enum class Type{
        SingleShoot,
        Routine
    };

    Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth = configMINIMAL_STACK_SIZE)
    {
        _name = name;
        _priority = priority;
        _stackDepth = stackDepth;
    }

    void singleShoot()
    {
        createTask(Type::SingleShoot);
    }

    void startRoutine(unsigned short delay)
    {
        _delay = delay;
        createTask(Type::Routine);
    }

    void stop()
    {
        _running = false;
    }

protected:

    virtual void task() = 0;

    static void taskfun(void *param)
    {
        static_cast<Task*>(param)->execute();
    }

    void execute()
    {
        _running = true;
        if(_type == Type::SingleShoot){ single(); }
        else { routine(); }
        _running = false;
    }

    void single()
    {
        task();
        deleteTask();
    }


    void routine()
    {
        while(_running)
        {
            task();
            wait(_delay);
        }
        deleteTask();
    }

    void wait(unsigned int delay)
    {
        vTaskDelay(pdMS_TO_TICKS(delay));
    }

    void createTask(Type type)
    {
        _type = type;
        xTaskCreate(&taskfun, _name, _stackDepth, this, _priority, &_handle);
    }

    void deleteTask()
    {
        vTaskDelete(NULL);
    }

protected:
    xTaskHandle _handle;

    const char *_name;
    unsigned portBASE_TYPE _priority;
    unsigned short _stackDepth;
    unsigned short _delay;
    Type _type;
    bool _running;
};