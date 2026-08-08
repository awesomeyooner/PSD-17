#include <iostream>

#include "plib/util/system.hpp"
#include "plib/util/logger.hpp"

#include "CommiFaceLib/protocols/serial.hpp"

int main(int argc, char* argv[])
{

    SerialInterface serial;

    serial.init_field("product", "STM32 Virtual ComPort");
    

    return 0;

} // end of "main(int, char*)"