#include "RtosUtils/Task.hpp"

#include "esp_log.h"

static const char *TAG = "Task";


Task::Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth)
{
    _name = name;
    _priority = priority;
    _stackDepth = stackDepth;
}

Task::~Task()
{
    deleteTask();
}

void Task::start()
{
    xTaskCreate(&taskWrapper, _name, _stackDepth, this, _priority, &_handle);
}

void Task::stop()
{
    _running = false;
}
    
void Task::taskWrapper(void *param)
{
    auto task = static_cast<Task*>(param);


    task->_running = true;
    task->execute();
}



void Task::deleteTask()
{
    ESP_LOGD(TAG, "Deleting task: [%s]", _name);
    vTaskDelete(NULL);
    _running = false;
}