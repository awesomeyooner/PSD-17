#include "helpers/usbfs_api.hpp"
#include "helpers/usbfs.hpp"


bool USBFS_on_receive(uint8_t* buffer, uint32_t* length)
{
    // Call the C++ version
    return USBFS::on_receive(buffer, length);

} // end of "USBFS_on_receive(uint8_t*, uint32_t*)"
