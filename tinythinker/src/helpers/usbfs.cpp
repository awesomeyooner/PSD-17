#include "helpers/usbfs.hpp"


bool USBFS::println(std::string text)
{
    // Add carriage return and newline
    text += "\r\n";

    // Send the data over
    // Return true if trasmit was USBD_OK
    return CDC_Transmit_FS((uint8_t*)text.c_str(), text.length()) == USBD_OK;

} // end of println(std::string)


bool USBFS::println(double data)
{
    // Convert data to string and send it over
    return println(std::to_string(data));
    
} // end of println(double)


bool USBFS::print_header(std::string header, std::string text)
{
    // [ HEADER ]: My Text
    std::string formatted = "[ " + header + " ]: " + text;

    // Print the statement
    return println(formatted);

}; // end of print_header(std::string, std::string)


bool USBFS::print_header(std::string header, double data)
{
    // [ HEADER ] data
    std::string formatted = "[ " + header + " ] " + std::to_string(data);

    // Print the statement
    return println(formatted);

}; // end of print_header(std::string, double)