#ifndef ANALOG_SENSOR_HPP
#define ANALOG_SENSOR_HPP

#include <atomic>
#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include "open_greenery/adc/IADCReader.hpp"

namespace open_greenery
{
namespace sensor
{

class AnalogSensor
{
public:
    using Notificator = std::function<void(std::int16_t)>;

    AnalogSensor(std::shared_ptr<open_greenery::adc::IADCReader> _reader);
    AnalogSensor(std::shared_ptr<open_greenery::adc::IADCReader> _reader,
                 const std::chrono::milliseconds period);
    AnalogSensor(const AnalogSensor &) = delete;
    AnalogSensor & operator =(const AnalogSensor &) = delete;
    ~AnalogSensor();

    void subscribe(Notificator _notificator);
    void unsubscribe(Notificator _notificator);

    void enable(const std::chrono::milliseconds period);
    void disable();
private:
    using NotificatorFuncPtr = void(*)(std::int16_t);
    void notify(const std::int16_t _val) const;
    void update(const std::chrono::milliseconds period) const;

    std::shared_ptr<open_greenery::adc::IADCReader> m_reader;
    std::thread m_thread;
    std::atomic_bool m_thread_continue;
    std::list<Notificator> m_notificators;
    mutable std::mutex m_notificators_mutex;
};

}
}

#endif //ANALOG_SENSOR_HPP
