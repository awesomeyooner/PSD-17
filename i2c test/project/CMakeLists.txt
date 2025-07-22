cmake_minimum_required(VERSION 3.12)

# CHANGE ME
project( i2c_test )

set(MAIN_EXEC Main)
set(TEST_EXEC Test)

# Create list of dependencies
set(THIS_PACKAGE_INCLUDE_DEPENDS
  
)

# Find each dependency
find_package(ament_cmake REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# GTest
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

find_library(I2C_LIB NAMES i2c PATHS /usr/local/lib)
find_path(I2C_INCLUDE_DIR NAMES i2c/i2c.h PATHS /usr/local/include)


include_directories(include ${I2C_INCLUDE_DIR})

add_executable( ${MAIN_EXEC} src/main.cpp )

target_link_libraries( ${MAIN_EXEC} ${THIS_PACKAGE_INCLUDE_DEPENDS} ${I2C_LIB})

# TESTING
enable_testing()
add_executable( ${TEST_EXEC} test/test.cpp)
target_link_libraries(${TEST_EXEC} GTest::gtest_main)

# automatic
gtest_discover_tests(${TEST_EXEC})

# manual
# add_test(NAME my_test_case COMMAND my_test)