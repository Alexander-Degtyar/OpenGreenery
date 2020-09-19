#include <open_greenery/irrigation/state_machine/WetState.hpp>
#include <chrono>

namespace open_greenery::irrigation
{

WetState::WetState(const open_greenery::dataflow::IrrigationConfigRecord & _config)
    :State(_config)
{}

StateName WetState::state()
{
    return StateName::WET;
}

StateName WetState::handleSoilMoisture(std::int16_t _moisture)
{
    if (_moisture >= m_config.dry)// If soil is to dry
    {
        // Change machine state to DRY
        return StateName::DRY;
    }

    return StateName::WET;
}

}
