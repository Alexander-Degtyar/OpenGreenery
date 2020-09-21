#ifndef IRRIGATION_IMPL_H
#define IRRIGATION_IMPL_H

#include <memory>
#include <mutex>
#include <thread>
#include <open_greenery/dataflow/IIrrigationConfigDataProvider.hpp>
#include <open_greenery/dataflow/ISensorReadProvider.hpp>
#include <open_greenery/pump/IPump.hpp>
#include <open_greenery/tools/LoopThread.hpp>
#include "state_machine/Context.hpp"

namespace open_greenery::irrigation
{

class IrrigationController
{
public:
    IrrigationController(open_greenery::dataflow::IrrigationConfigRecord _cfg,
                         std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _sensor_reader,
                         std::shared_ptr<open_greenery::pump::IPump> _pump);
    ~IrrigationController();
    void start();
    void stop();

private:
    void IrrigationThreadFunc();

    open_greenery::dataflow::IrrigationConfigRecord m_cfg;
    std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> m_sensor_reader;

    std::unique_ptr<open_greenery::tools::LoopThread> m_irrigation_thr;
    std::unique_ptr<Context> m_state_machine_context;
};

}

#endif //IRRIGATION_IMPL_H
