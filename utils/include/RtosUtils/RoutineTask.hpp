#pragma once

#include "RtosUtils/Task.hpp"

class RoutineTask : public Task
{
public:
    RoutineTask(const char *name, unsigned portBASE_TYPE priority, unsigned short delay, unsigned short stackDepth = configMINIMAL_STACK_SIZE);

protected:
    virtual void execute() override;

    void wait(unsigned int delay);

private:
    unsigned short _delay = 0;
};