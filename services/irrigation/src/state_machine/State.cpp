#include <open_greenery/irrigation/state_machine/State.hpp>

namespace open_greenery::irrigation
{

State::State(const open_greenery::dataflow::IrrigationConfigRecord & _config)
    :m_config(_config)
{}

}
