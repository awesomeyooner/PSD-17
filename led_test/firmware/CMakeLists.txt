cmake_minimum_required(VERSION 3.22)

project(firmware)

add_subdirectory(cube)

add_library(shared_library INTERFACE)

target_include_directories(shared_library INTERFACE
    ${CMAKE_SOURCE_DIR}/cube/Core/Inc
    ${CMAKE_SOURCE_DIR}/cube/USB_DEVICE/App
    ${CMAKE_SOURCE_DIR}/cube/USB_DEVICE/Target
    ${CMAKE_SOURCE_DIR}/cube/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/cube/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_SOURCE_DIR}/cube/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_SOURCE_DIR}/cube/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${CMAKE_SOURCE_DIR}/cube/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/cube/Drivers/CMSIS/Include
)

target_compile_definitions(shared_library INTERFACE
    STM32F446xx
)

# Add User Libraries
add_subdirectory(lib/EmbeddedLib)
target_link_libraries(EmbeddedLib PRIVATE shared_library)

add_subdirectory(lib/WireLib)
target_link_libraries(WireLib PRIVATE shared_library)

add_subdirectory(lib/ActionLib)
target_link_libraries(ActionLib PRIVATE shared_library)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    shared_library

    EmbeddedLib
    WireLib
    ActionLib
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    src/core.cpp

    src/devices/as5047.cpp
    
    # src/devices/l298n.cpp
    # src/devices/servo.cpp

    # src/hardware_manager.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    # Add user defined include paths
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
    $<INSTALL_INTERFACE:lib>
)