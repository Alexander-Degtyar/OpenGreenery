#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <map>
#include <memory>
#include <open_greenery/pump/IPump.hpp>
#include "State.hpp"
#include "StateName.hpp"

namespace open_greenery::irrigation
{

class Context final : IState
{
public:
    Context(const open_greenery::dataflow::IrrigationConfigRecord & _config,
            std::shared_ptr<open_greenery::pump::IPump> _pump);
    StateName state() override;
    StateName handleSoilMoisture(std::int16_t _moisture) override;

private:
    std::map<StateName, std::shared_ptr<State>> m_states;
    std::shared_ptr<State> m_current_state;
};

}

#endif //CONTEXT_HPP
