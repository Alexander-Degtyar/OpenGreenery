#ifndef WET_STATE_HPP
#define WET_STATE_HPP

#include "State.hpp"

namespace open_greenery::irrigation
{

class WetState final : public State
{
public:
    WetState(const open_greenery::dataflow::IrrigationConfigRecord & _config);

    StateName handleSoilMoisture(std::int16_t _moisture) override;
    StateName state() override;
};

}

#endif //WET_STATE_HPP
