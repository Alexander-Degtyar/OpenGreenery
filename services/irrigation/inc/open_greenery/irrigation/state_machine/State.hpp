#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <open_greenery/dataflow/IrrigationConfigRecord.hpp>
#include "IState.hpp"

namespace open_greenery::irrigation
{

class State : IState
{
public:
    StateName handleSoilMoisture(std::int16_t _moisture) override = 0;
    StateName state() override = 0;

protected:
    State(const open_greenery::dataflow::IrrigationConfigRecord & _config);

    const open_greenery::dataflow::IrrigationConfigRecord & m_config;
};

}

#endif //STATE_HPP
