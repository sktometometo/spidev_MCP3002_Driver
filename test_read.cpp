#include "MCP3002_Driver.h"

#include <iostream>
#include <cstdlib>

#include <getopt.h>
#include <unistd.h>

int channel = 0;
std::string device_name( "/dev/spidev0.0" );

void print_usage( const char *prog )
{
    std::cout << "Usage: " << prog << std::endl;
    std::cout << "  -D --device   device to use (default /dev/spidev0.0)" << std::endl
              << "  -c --channel  ADC channel to use (default 0)" << std::endl;
    std::exit(1);
}

void parse_opts( int argc, char **argv )
{
    while (1) {
        static const struct option lopts[] = {
            { "device",  1, 0, 'D' },
            { "channel", 1, 0, 'c' },
        };
        int c;
        
        std::cout << "hoge" << std::endl;

        c = getopt_long( argc, argv, "D:c:", lopts, NULL );

        if ( c == -1 )
            break;

        switch (c) {
            case 'D':
                device_name = std::string( optarg );
                break;
            case 'c':
                channel = std::atoi( optarg );
                break;
            default:
                print_usage( argv[0] );
                break;
        }
    }
}

int main( int argc, char **argv )
{
    parse_opts( argc, argv );

    MCP3002_Driver hoge;

    bool ret = hoge.init( 1000000, device_name );

    if ( ret == false ) {
        std::cerr << "device initialization error." << std::endl;
        std::exit(-1);
    }

    int value;
    while (true) {
      usleep(500000);
      value = hoge.read( channel );
      if ( value < 0 ) {
          std::cerr << "read error :" << value << std::endl;
          std::exit(-1);
      }
      std::cout << value << std::endl;
    }
}
