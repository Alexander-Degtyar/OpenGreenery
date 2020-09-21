#include <utility>
#include <iostream>
#include <SQLiteCpp/Exception.h>
#include <open_greenery/irrigation/IrrigationController.hpp>

namespace open_greenery::irrigation
{

IrrigationController::IrrigationController(open_greenery::dataflow::IrrigationConfigRecord _cfg,
                                           std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _sensor_reader,
                                           std::shared_ptr<open_greenery::pump::IPump> _pump)
    :m_cfg(std::move(_cfg)),
    m_sensor_reader(std::move(_sensor_reader)),
    m_state_machine_context(std::make_unique<Context>(m_cfg, std::move(_pump)))
{}

IrrigationController::~IrrigationController()
{
    stop();
}

void IrrigationController::start()
{
    if (!m_irrigation_thr)
    {
        m_irrigation_thr = std::make_unique<open_greenery::tools::LoopThread>(
                std::bind(&IrrigationController::IrrigationThreadFunc, this), m_cfg.watering_period
            );
    }
    m_irrigation_thr->start();
}

void IrrigationController::stop()
{
    if (m_irrigation_thr)
    {
        m_irrigation_thr->stop();
    }
}

void IrrigationController::IrrigationThreadFunc()
{
    std::int16_t soil_moisture;
    try
    {
        soil_moisture = m_sensor_reader->read();
    }
    catch (const SQLite::Exception & _ex)
    {
        std::cerr << "Irrigation controller: SQLite error: " << _ex.getErrorCode() << " "
                  << _ex.getExtendedErrorCode() << " " << _ex.getErrorStr() << " " << _ex.what() << std::endl;
        return;
    }

    m_state_machine_context->handleSoilMoisture(soil_moisture);
}

}
