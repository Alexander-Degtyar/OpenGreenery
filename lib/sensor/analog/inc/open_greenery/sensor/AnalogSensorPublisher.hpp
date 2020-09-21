#ifndef ANALOG_SENSOR_HPP
#define ANALOG_SENSOR_HPP

#include <atomic>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <open_greenery/dataflow/ISensorReadProvider.hpp>
#include <open_greenery/tools/LoopThread.hpp>
#include "IAnalogSensorPublisher.hpp"

namespace open_greenery::sensor
{

class AnalogSensorPublisher : public IAnalogSensorPublisher
{
public:
    AnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider,
                          std::chrono::milliseconds period);

    /**
     * @note Internal LoopThread will be stopped in the own destructor
     */
    ~AnalogSensorPublisher() override = default;

    void subscribe(Notificator _notificator) override;
    void unsubscribe(Notificator _notificator) override;

    void start() override;
    void stop() override;
private:
    using NotificatorFuncPtr = void(*)(std::int16_t);
    void notify(std::int16_t _val) const;

    std::list<Notificator> m_notificators;
    mutable std::mutex m_notificators_mutex;

    std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> m_sensor_provider;
    const std::chrono::milliseconds m_period;
    std::optional<open_greenery::tools::LoopThread> m_reading_thr;
};

}

#endif //ANALOG_SENSOR_HPP
