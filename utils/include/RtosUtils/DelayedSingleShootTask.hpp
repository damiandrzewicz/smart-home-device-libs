#pragma once

#include "Task.hpp"

class DelayedSingleShootTask : public Task
{
public:
    DelayedSingleShootTask(const char *name, unsigned portBASE_TYPE priority, unsigned int delay, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

    void setDelay(unsigned int delay);

protected:
    virtual void execute() override;

private:
    unsigned int _delay = 0;
};