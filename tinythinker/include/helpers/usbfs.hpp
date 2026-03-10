#ifndef USBFS_HPP
#define USBFS_HPP

#include "usb_device.h"
#include "usbd_cdc_if.h"

#include <string>


class USBFS
{

    public:

        static bool println(std::string text);

        static bool println(double data);

        static bool print_header(std::string header, std::string text);

        static bool print_header(std::string header, double data);


    private:


}; // class USBFS


#endif // USBFS_HPP