#ifndef I_OPTIONAL_SENSOR_READ_PROVIDER_HPP
#define I_OPTIONAL_SENSOR_READ_PROVIDER_HPP

#include <optional>
#include "SensorRecord.hpp"

namespace open_greenery::dataflow
{

class IOptionalSensorReadProvider
{
public:
    virtual std::optional<std::int16_t> read() const = 0;

    IOptionalSensorReadProvider() = default;
    virtual ~IOptionalSensorReadProvider() = default;
    IOptionalSensorReadProvider(const IOptionalSensorReadProvider &) = delete;
    IOptionalSensorReadProvider & operator=(const IOptionalSensorReadProvider &) = delete;
};

}

#endif //I_OPTIONAL_SENSOR_READ_PROVIDER_HPP
