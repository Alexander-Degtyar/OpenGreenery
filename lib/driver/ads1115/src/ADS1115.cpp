#include "open_greenery/driver/ads1115/ADS1115.hpp"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>


namespace open_greenery
{
namespace driver
{

ADS1115::Config::Config(const Address _adr)
    :adr(_adr)
{}

ADS1115::ADS1115(const Config _cfg)
    :m_cfg(_cfg)
{
    m_i2c_dev = open("/dev/i2c-1", O_RDWR);
    if (m_i2c_dev < 0)
    {
        throw std::runtime_error("Failed to open the I2C bus.");
    }

    if (ioctl(m_i2c_dev, I2C_SLAVE, std::uint8_t(m_cfg.adr)) < 0)
    {
        throw std::runtime_error("Failed to acquire bus access and/or talk to slave.");
    }
}

ADS1115::ADS1115(const Address _adr)
    :ADS1115(Config{_adr})
{}

std::int16_t ADS1115::read(const Channel _ch)
{
    // Prepare configuration
    const std::uint16_t adc_cfg = cfgRegs()
        | channelMask(_ch)
        | std::uint16_t(OS_W::SINGLE);

    std::uint8_t reg_cfg [3] {std::uint8_t(Register::CONFIG),
                              std::uint8_t((adc_cfg >> 8) & 0x00FF),
                              std::uint8_t(adc_cfg & 0x00FF)};

    // Write configuration
    if(::write(m_i2c_dev, reg_cfg, 3) != 3)
    {
        throw std::runtime_error("Failed to write the I2C bus.");
    }


    // Wait for conversion finish
    std::this_thread::sleep_for(conversionDuration(m_cfg.dr));// Sleep for average conversion duration
    std::uint8_t data [2] {};
    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (::read(m_i2c_dev, &data, 1) != 1)
        {
            throw std::runtime_error("Failed to read the I2C bus.");
        }
    }
    while ((data[0] & std::uint8_t(OS_R::MASK)) == std::uint8_t(OS_R::BUSY));

    // Switch register to read
    reg_cfg[0] = std::uint8_t(Register::CONVERSION);
    if (::write(m_i2c_dev, reg_cfg, 1) != 1)
    {
        throw std::runtime_error("Failed to write the I2C bus.");
    }

    // Read conversion result
    if (::read(m_i2c_dev, &data, 2) != 2)
    {
        throw std::runtime_error("Failed to read the I2C bus.");
    }

    return (data[0] << 8) | data[1];
}

ADS1115::Config ADS1115::cfg() const
{
    return m_cfg;
}

std::uint16_t ADS1115::cfgRegs() const
{
    return  std::uint16_t(m_cfg.mux)   |
            std::uint16_t(m_cfg.pga)   |
            std::uint16_t(m_cfg.mode)  |
            std::uint16_t(m_cfg.dr)    |
            std::uint16_t(m_cfg.cm)    |
            std::uint16_t(m_cfg.cp)    |
            std::uint16_t(m_cfg.cl)    |
            std::uint16_t(m_cfg.cq);
}

std::uint16_t ADS1115::channelMask(const Channel _ch) const
{
    switch (_ch)
    {
        case Channel::A0:
            return std::uint16_t(MUX::SINGLE_0);
        case Channel::A1:
            return std::uint16_t(MUX::SINGLE_1);
        case Channel::A2:
            return std::uint16_t(MUX::SINGLE_2);
        case Channel::A3:
            return std::uint16_t(MUX::SINGLE_3);
        default:
            throw std::logic_error("Invalid channel.");
    }
}

std::chrono::milliseconds ADS1115::conversionDuration(const DR _dr) const
{
    switch (_dr)
    {
        case DR::SPS_8: return std::chrono::milliseconds(128);
        case DR::SPS_16: return std::chrono::milliseconds(64);
        case DR::SPS_32: return std::chrono::milliseconds(32);
        case DR::SPS_64: return std::chrono::milliseconds(16);
        case DR::SPS_128: return std::chrono::milliseconds(8);
        case DR::SPS_250: return std::chrono::milliseconds(4);
        case DR::SPS_475: return std::chrono::milliseconds(2);
        case DR::SPS_860: return std::chrono::milliseconds(1);
        default: throw std::logic_error("Invalid data rate.");
    }
}

}
}
