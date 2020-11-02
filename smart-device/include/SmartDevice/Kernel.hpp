#pragma once

#include "RtosUtils/RoutineTask.hpp"
#include "SmartDevice/MemoryDaemon.hpp"
#include "Network/WifiStation.hpp"

namespace SmartDevice
{
    class Kernel : public RoutineTask
    {
    public:
        Kernel();


    protected:
        virtual void initTask() override;

        virtual void task() override;

    private:
        void printSystemInfo();

        void initMemoryDaemon();

        void initNetwork();

        void performOta();

    private:
        MemoryDaemon _memoryDaemon;
        WifiStation _station;
        //Ota _ota;
    };
};

