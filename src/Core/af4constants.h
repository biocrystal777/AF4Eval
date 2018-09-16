#ifndef FFFCONSTANTS_H
#define FFFCONSTANTS_H
#include <af4datatypes.h>

#define NUMBER_OF_DETECTORS 8                     //
#define PI                  3.141592653589793     //
#define NA                  6.02214129e+23         // [1/mol]
#define KB                  1.3806488e-23         // [m^2*kg*s^-2*K^-1]
//#define RHO                 0.998234
//#define ETA                 0.0100194

constexpr arrD8 anglesDeg = {
   21.47,
   34.91,
   53.13,
   67.70,
   88.90,
   109.80,
   131.30,
   149.31
};

constexpr arrD8 angles = {
   21.47 * PI / 360.0,
   34.91 * PI / 360.0,
   53.13 * PI / 360.0,
   67.70 * PI / 360.0,
   88.90 * PI / 360.0,
   109.88 * PI / 360.0,
   131.30 * PI / 360.0,
   149.31 * PI / 360.0
};

//static const std::vector < double > __slsAngles  {10.78, 17.46, 26.56, 33.83, 44.43, 55.1, 65.65, 74.66};

#endif // FFFCONSTANTS_H
