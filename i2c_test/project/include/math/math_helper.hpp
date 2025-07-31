#ifndef MATH_HELPER_HPP
#define MATH_HELPER_HPP

#include <cmath>

namespace conversions{

    double mg_to_mps2(double mg){
        return mg * 0.00980665;
    }

    double degrees_to_radians(double degrees){
        return (degrees / 180) * M_PI;
    }

    int seconds_to_micros(double seconds){
        return (int)(seconds * 1000000);
    }

} // namespace math_util

#endif // MATH_HELPER_HPP