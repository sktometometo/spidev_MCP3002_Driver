#include <iostream>
#include <string>

const std::string spi_device_name_0( "/dev/spidev0.0" );
const std::string spi_device_name_1( "/dev/spidev0.1" );

class MCP3002_Driver
{
    public:

        MCP3002_Driver();
        MCP3002_Driver( int, std::string );
        ~MCP3002_Driver();
        bool init( int, std::string );
        int read( uint8_t );

    private:

        int spiBPW;
        int spibus_speed;
        int spibus_mode;
        std::string spi_device_name;
        int file_descriptor;
};
