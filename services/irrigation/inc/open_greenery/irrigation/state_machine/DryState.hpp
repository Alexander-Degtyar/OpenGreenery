#ifndef DRY_STATE_HPP
#define DRY_STATE_HPP

#include "State.hpp"
#include <open_greenery/pump/IPump.hpp>

namespace open_greenery::irrigation
{

class DryState final : public State
{
public:
    DryState(const open_greenery::dataflow::IrrigationConfigRecord & _config,
             std::shared_ptr<open_greenery::pump::IPump> _pump);
    StateName handleSoilMoisture(std::int16_t _moisture) override;
    StateName state() override;

private:
    std::shared_ptr<open_greenery::pump::IPump> m_pump;
};

}

#endif //DRY_STATE_HPP
