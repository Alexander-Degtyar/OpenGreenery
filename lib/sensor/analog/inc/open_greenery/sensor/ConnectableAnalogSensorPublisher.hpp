#ifndef CONNECTABLE_ANALOG_SENSOR_HPP
#define CONNECTABLE_ANALOG_SENSOR_HPP

#include <open_greenery/gpio/IInputPin.hpp>
#include "AnalogSensorPublisher.hpp"

namespace open_greenery::sensor
{

class ConnectableAnalogSensorPublisher : public AnalogSensorPublisher
{
public:
    ConnectableAnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider,
                                     std::shared_ptr<open_greenery::gpio::IInputPin> _is_connected,
                                     std::chrono::milliseconds _period);

protected:
    void notify(std::int16_t _val) const override;

private:
    std::shared_ptr<open_greenery::gpio::IInputPin> m_is_connected;
};

}

#endif //CONNECTABLE_ANALOG_SENSOR_HPP
