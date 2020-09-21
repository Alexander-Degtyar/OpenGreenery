#include <open_greenery/tools/LoopThread.hpp>

namespace open_greenery::tools
{

LoopThread::LoopThread(std::function<void()> _func, std::chrono::milliseconds _period)
    :m_stop_flag(true),
    m_periodic_func([_func, _period, this]{
        while (!m_stop_flag)
        {
            const auto end = std::chrono::system_clock::now() + _period;
            if (m_stop_flag)
            {
                break;
            }
            _func();
            if (m_stop_flag)
            {
                break;
            }
            std::this_thread::sleep_until(end);
        }
    }),
    m_thr()
{}

LoopThread::~LoopThread()
{
    stop();
    if (m_thr.joinable())
    {
        m_thr.join();
    }
}

bool LoopThread::start()
{
    if (!m_thr.joinable() && m_stop_flag)
    {
        m_stop_flag = false;
        m_thr = std::thread(m_periodic_func);
        return true;
    }
    else
    {
        return false;
    }
}

void LoopThread::stop()
{
    m_stop_flag = true;
}

}
