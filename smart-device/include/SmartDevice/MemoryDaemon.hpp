#pragma once

#include "RtosUtils/RoutineTask.hpp"

namespace SmartDevice
{
    class MemoryDaemon : public RoutineTask
    {
    public:
        MemoryDaemon();

    protected:
        virtual void task() override;

    private:

    private:
        unsigned short _maxHeapSize = 520; //KB
    };
};