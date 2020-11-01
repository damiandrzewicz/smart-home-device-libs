#pragma once

#include "RtosUtils/RoutineTask.hpp"
#include "SmartDevice/MemoryDaemon.hpp"

namespace SmartDevice
{
    class Kernel : public RoutineTask
    {
    public:
        Kernel();


    protected:
        virtual void task() override;

    private:
        MemoryDaemon _memoryDaemon;
    };
};

