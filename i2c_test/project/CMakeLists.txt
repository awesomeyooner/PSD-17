cmake_minimum_required(VERSION 3.12)

# CHANGE ME
project( i2c_test LANGUAGES C CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAIN_EXEC Main)
set(TEST_EXEC Test)

# plib
add_subdirectory(lib/plib)

# Create list of dependencies
set(THIS_PACKAGE_INCLUDE_DEPENDS
  
)

# Find each dependency
find_package(ament_cmake REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Create main executable
add_executable( ${MAIN_EXEC} src/main.cpp )

# Link the dependencies / libraries
target_link_libraries( ${MAIN_EXEC} 
  ${THIS_PACKAGE_INCLUDE_DEPENDS} 
  plib
)

target_include_directories( ${MAIN_EXEC}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:lib>
)

# GTest
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# TESTING
enable_testing()

add_executable( ${TEST_EXEC} test/test.cpp)

target_link_libraries(${TEST_EXEC} 
  GTest::gtest_main
  ${THIS_PACKAGE_INCLUDE_DEPENDS} 
  plib
)

target_include_directories( ${TEST_EXEC}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:lib>
)

# automatic
gtest_discover_tests(${TEST_EXEC})

# manual
# add_test(NAME my_test_case COMMAND my_test)