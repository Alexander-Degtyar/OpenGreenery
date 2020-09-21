#ifndef I_ANALOG_SENSOR_PUBLISHER_HPP
#define I_ANALOG_SENSOR_PUBLISHER_HPP

#include <cstdint>
#include <functional>

namespace open_greenery::sensor
{

class IAnalogSensorPublisher
{
public:
    using Notificator = std::function<void(std::int16_t)>;

    virtual void subscribe(Notificator _notificator) = 0;
    virtual void unsubscribe(Notificator _notificator) = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    IAnalogSensorPublisher() = default;
    virtual ~IAnalogSensorPublisher() = default;

    IAnalogSensorPublisher(const IAnalogSensorPublisher &) = delete;
    IAnalogSensorPublisher & operator=(const IAnalogSensorPublisher &) = delete;
};

}

#endif //I_ANALOG_SENSOR_PUBLISHER_HPP
