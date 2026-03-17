#ifndef USBFS_HPP
#define USBFS_HPP

#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "usbd_def.h"

#include <string>
#include <vector>
#include <functional>

class USBFS
{

    public:

        static bool transmit_bytes(const std::vector<uint8_t>& bytes);

        static bool on_receive(uint8_t* buffer, uint32_t* length);

        static bool println(std::string text);

        static bool println(double data);

        static bool print_header(std::string header, std::string text);

        static bool print_header(std::string header, double data);

    private:

        static std::vector<uint8_t> m_read_buffer;

        static std::function<bool(const std::vector<uint8_t>&)> m_receive_callback;


}; // class USBFS


#endif // USBFS_HPP