#include "helpers/usbfs.hpp"


// Initialize the members
std::vector<uint8_t> USBFS::m_read_buffer;


bool USBFS::transmit_bytes(const std::vector<uint8_t>& bytes)
{
    return CDC_Transmit_FS(const_cast<uint8_t*>(bytes.data()), bytes.size()) == USBD_OK;

} // end of "transmit(bytes(const std::vector<uint8_t>&))"


bool USBFS::on_receive(uint8_t* buffer, uint32_t* length)
{
    // Fill the read buffer
    m_read_buffer.assign(buffer, buffer + *length);

    // Call the User-Defined callback
    return m_receive_callback(m_read_buffer);

} // end of "on_receive(uint8_t*, uint32_t*)"


bool USBFS::println(std::string text)
{
    // Add carriage return and newline
    text += "\r\n";

    // Convert to byte vector
    std::vector<uint8_t> buffer(text.begin(), text.end());

    // Send the data over
    // Return true if trasmit was USBD_OK
    return transmit_bytes(buffer);

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