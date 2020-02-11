#include "open_greenery/sensor/AnalogSensor.hpp"

namespace open_greenery
{
namespace sensor
{

AnalogSensor::AnalogSensor(std::shared_ptr<open_greenery::adc::IADCReader> _reader)
    : m_reader(_reader),
      m_thread_continue(false)
{}

AnalogSensor::AnalogSensor(std::shared_ptr<open_greenery::adc::IADCReader> _reader,
                           const std::chrono::milliseconds period)
    :AnalogSensor(_reader)
{
    enable(period);
}

AnalogSensor::~AnalogSensor()
{
    m_thread_continue = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void AnalogSensor::subscribe(Notificator _notificator)
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.push_back(_notificator);
}

void AnalogSensor::unsubscribe(Notificator _notificator)
{
    auto comp_lambda = [](Notificator _n1, Notificator _n2)
        {
            return _n1.target<NotificatorFuncPtr>() == _n2.target<NotificatorFuncPtr>();
        };
    auto predicate = [_notificator, comp_lambda](const Notificator & _n){return comp_lambda(_notificator, _n);};

    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.remove_if(predicate);
}

void AnalogSensor::enable(const std::chrono::milliseconds period)
{
    m_thread_continue = true;
    m_thread = std::thread(&AnalogSensor::update, this, period);
}

void AnalogSensor::disable()
{
    m_thread_continue = false;
}

void AnalogSensor::notify(const std::int16_t _val) const
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    for (Notificator ntf : m_notificators)
    {
        ntf(_val);
    }
}

void AnalogSensor::update(const std::chrono::milliseconds period) const
{
    auto iteration_start = std::chrono::system_clock::now();
    while (m_thread_continue)
    {
        notify(m_reader->read());

        iteration_start += period;
        std::this_thread::sleep_until(iteration_start);
    }
}

}
}
