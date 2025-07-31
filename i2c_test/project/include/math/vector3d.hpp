#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include <string>

namespace math_util{
    class Vector3D{

        public:
            double x, y, z;

            Vector3D(double _x, double _y, double _z){
                x = _x;
                y = _y;
                z = _z;
            }

            Vector3D() : Vector3D(0, 0, 0){}

            void copy_from(Vector3D other){
                x = other.x;
                y = other.y;
                z = other.z;
            }

            void refresh(double _x, double _y, double _z){
                x = _x;
                y = _y;
                z = _z;
            }

            std::string to_string(){
                std::string output = "";

                output += "x: " + std::to_string(x) + "\t";
                output += "y: " + std::to_string(y) + "\t";
                output += "z: " + std::to_string(z) + "\t";

                return output;
            }

        private:
        
    }; // class Vector3D

} // namespace math_util

#endif // VECTOR3D_HPP