#ifndef USBFS_API_HPP
#define USBFS_API_HPP

#include <stdio.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

    bool USBFS_on_receive(uint8_t* buffer, uint32_t* length);

#ifdef __cplusplus
}
#endif


#endif // USBFS_API_HPP
