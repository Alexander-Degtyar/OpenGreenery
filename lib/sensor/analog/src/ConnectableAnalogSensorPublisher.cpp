#include "open_greenery/sensor/ConnectableAnalogSensorPublisher.hpp"

namespace open_greenery::sensor
{

ConnectableAnalogSensorPublisher::ConnectableAnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider,
                                                                   std::shared_ptr<open_greenery::gpio::IInputPin> _is_connected,
                                                                   std::chrono::milliseconds _period)
    : AnalogSensorPublisher(_provider, _period),
    m_is_connected(_is_connected)
{
        if (!m_is_connected)
        {
            throw std::logic_error("Is-sensor-connected pin is nullptr");
        }
}


void ConnectableAnalogSensorPublisher::notify(std::int16_t _val) const
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    for (const Notificator & ntf : m_notificators)
    {
        if (m_is_connected->read() == open_greenery::gpio::LogicLevel::HIGH)
        {
            ntf(_val);
        }
    }
}

}
