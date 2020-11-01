#pragma once

#include "Task.hpp"

class SingleShootTask : public Task
{
public:
    SingleShootTask(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

protected:
    virtual void execute() override;
};