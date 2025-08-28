#include <gtest/gtest.h>

#include "i2c/i2c.hpp"

TEST(Testing, BasicTest)
{
  float input = 1234.5;
  std::vector<uint8_t> bytes = I2C::float_to_bytes(input);
  float output = I2C::bytes_to_float(bytes);

  ASSERT_EQ(input, output);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}