#include "open_greenery/sensor/AnalogSensorPublisher.hpp"

#include <utility>

namespace open_greenery::sensor
{

AnalogSensorPublisher::AnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider,
                                             std::chrono::milliseconds period)
    : m_sensor_provider(std::move(_provider)),
    m_period(period)
{}

void AnalogSensorPublisher::subscribe(Notificator _notificator)
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.push_back(_notificator);
}

void AnalogSensorPublisher::unsubscribe(Notificator _notificator)
{
    auto comp_lambda = [](Notificator _n1, Notificator _n2)
        {
            return _n1.target<NotificatorFuncPtr>() == _n2.target<NotificatorFuncPtr>();
        };
    auto predicate = [_notificator, comp_lambda](const Notificator & _n){return comp_lambda(_notificator, _n);};

    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.remove_if(predicate);
}

void AnalogSensorPublisher::start()
{
    if (!m_reading_thr)
    {
        m_reading_thr.emplace([this]{notify(m_sensor_provider->read());}, m_period);
    }
    m_reading_thr->start();
}

void AnalogSensorPublisher::stop()
{
    if (m_reading_thr)
    {
        m_reading_thr->stop();
    }
}

void AnalogSensorPublisher::notify(std::int16_t _val) const
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    for (const Notificator & ntf : m_notificators)
    {
        ntf(_val);
    }
}

}
