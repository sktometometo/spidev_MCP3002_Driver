#include <cstdint>
#include <cstring>

#include <iostream>
#include <string>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <fcntl.h>

#include "MCP3002_Driver.h"

MCP3002_Driver::MCP3002_Driver()
{
    this->spiBPW = 8;
    this->spibus_speed = 0;
    this->spi_device_name = std::string( "" );
    this->file_descriptor = -1;
}

MCP3002_Driver::MCP3002_Driver( int speed, std::string device_name )
{
    this->init( speed, device_name );
}

MCP3002_Driver::~MCP3002_Driver()
{
    if( this->file_descriptor >= 0 ) {
        close( this->file_descriptor );
    }
}

bool MCP3002_Driver::init( int speed, std::string device_name )
{
    this->spiBPW = 8;
    this->spibus_speed = speed;
    this->spi_device_name = device_name;
    this->file_descriptor = -1;
    this->spibus_mode = SPI_MODE_0;

    if ( ( this->file_descriptor = open( this->spi_device_name.c_str(), O_RDWR | O_SYNC ) ) < 0 ) {
        std::cerr << "device open error. device name : " << this->spi_device_name.c_str() << std::endl;
        return false;
    } else {
        std::cout << "device descriptor : " << this->file_descriptor << std::endl;
        std::cout << "device name : " << this->spi_device_name.c_str() << std::endl;
    }

    int ret = 0;

    if ( ( ret = ioctl( this->file_descriptor, SPI_IOC_WR_MODE, &(this->spibus_mode) ) ) == -1 ) {
        std::cerr << "SPI_IOC_WR_MODE setting error. ret = " << ret << std::endl;
        return false;
    } else {
        std::cout << "SPI_IOC_WR_MODE setting ret = " << ret << std::endl;
    }

    if ( ( ret = ioctl( this->file_descriptor, SPI_IOC_WR_BITS_PER_WORD, &(this->spiBPW) ) ) == -1 ) {
        std::cerr << "SPI_IOC_WR_BITS_PER_WORD setting error. ret = " << ret << std::endl;
        return false;
    } else {
        std::cout << "SPI_IOC_WR_BITS_PER_WORD setting ret = " << ret << std::endl;
    }

    if ( ( ret = ioctl( this->file_descriptor, SPI_IOC_WR_MAX_SPEED_HZ, &(this->spibus_speed) ) ) == -1 ) {
        std::cerr << "SPI_IOC_WR_MAX_SPEED_HZ setting error. ret = " << ret << std::endl;
        return false;
    } else {
        std::cout << "SPI_IOC_WR_MAX_SPEED_HZ setting ret = " << ret << std::endl;
    }

    return true;
}

int MCP3002_Driver::read( uint8_t adc_channel )
{
    uint8_t tx_data[2];
    uint8_t rx_data[2];

    memset( tx_data, 0, sizeof( tx_data ) );
    memset( rx_data, 0, sizeof( rx_data ) );

    if ( adc_channel == 0 ) {
        tx_data[0] = 0b11010000;
    } else {
        tx_data[0] = 0b11110000;
    }
    tx_data[1] = 0;

    struct spi_ioc_transfer spi;
    memset( &spi, 0, sizeof(spi) );
    spi.tx_buf        = (unsigned long) tx_data;
    spi.rx_buf        = (unsigned long) rx_data;
    spi.len           = 2;
    spi.delay_usecs   = 0;
    spi.speed_hz      = this->spibus_speed;
    spi.bits_per_word = this->spiBPW;

    int ret = ioctl( this->file_descriptor, SPI_IOC_MESSAGE(1), &spi );

    if ( ret == -1 ) {
        return -1;
    }

    return ( ( rx_data[0] << 8 ) | ( rx_data[1] >> 1) ) & 0x3FF;
}
