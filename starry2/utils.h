/**
Miscellaneous stuff used throughout the code.

*/

#ifndef _STARRY_UTILS_H_
#define _STARRY_UTILS_H_

#include <Eigen/Core>
#include <unsupported/Eigen/AutoDiff>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>
#include <limits>
#include "errors.h"

namespace utils {

    // Multiprecision datatype
    typedef boost::multiprecision::cpp_dec_float<STARRY_NMULTI> mp_backend;
    typedef boost::multiprecision::number<mp_backend, boost::multiprecision::et_off> Multi;
    #if STARRY_NMULTI > 150
    #error "Currently, PI is computed to a maximum of 150 digits of precision. "
           "If you **really** need `STARRY_NMULTI` > 150, you will need to re-define PI in `utils.h`."
    #endif

    // Some frequently used constants, computed
    // at both double and multi precision
    static const double PI_DOUBLE = M_PI;
    static const Multi PI_MULTI = Multi("3.14159265358979323846264338327950288419716939937510582097"
                                        "4944592307816406286208998628034825342117067982148086513282"
                                        "306647093844609550582231725359408128");
    template <typename T> inline T PI(){ return T(PI_DOUBLE); }
    template <> inline Multi PI(){ return PI_MULTI; }

    static const double SQRT_PI_DOUBLE = sqrt(PI<double>());
    static const Multi SQRT_PI_MULTI = sqrt(PI<Multi>());
    template <typename T> inline T SQRT_PI(){ return T(SQRT_PI_DOUBLE); }
    template <> inline Multi SQRT_PI(){ return SQRT_PI_MULTI; }

    static const double TWO_OVER_SQRT_PI_DOUBLE = 2.0 / sqrt(PI<double>());
    static const Multi TWO_OVER_SQRT_PI_MULTI = 2.0 / sqrt(PI<Multi>());
    template <typename T> inline T TWO_OVER_SQRT_PI(){ return T(TWO_OVER_SQRT_PI_DOUBLE); }
    template <> inline Multi TWO_OVER_SQRT_PI(){ return TWO_OVER_SQRT_PI_MULTI; }

    // Our custom vector types
    template <typename T>
    using Vector = Eigen::Matrix<T, Eigen::Dynamic, 1>;
    template <typename T>
    using VectorT = Eigen::Matrix<T, 1, Eigen::Dynamic>;
    template <typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    template <typename T>
    using UnitVector = Eigen::Matrix<T, 3, 1>;

    // Some useful unit vectors
    static const UnitVector<double> xhat_double({1, 0, 0});
    static const UnitVector<double> yhat_double({0, 1, 0});
    static const UnitVector<double> zhat_double({0, 0, 1});
    template <typename T> inline UnitVector<T> xhat(){ return xhat_double.template cast<T>(); }
    template <typename T> inline UnitVector<T> yhat(){ return xhat_double.template cast<T>(); }
    template <typename T> inline UnitVector<T> zhat(){ return xhat_double.template cast<T>(); }

    // Machine precision at current type
    // We need to be careful with AutoDiffScalar specialization.
    // See https://stackoverflow.com/a/36209847
    template<class T> struct tag{};
    template<class T> T mach_eps(tag<T>) { return std::numeric_limits<T>::epsilon(); }
    template<class T> Eigen::AutoDiffScalar<T> mach_eps(tag<Eigen::AutoDiffScalar<T>>) {
        return std::numeric_limits<typename T::Scalar>::epsilon();
    }
    template<class T> T mach_eps() { return mach_eps(tag<T>()); }

    // Check if number is even (or doubly, triply, quadruply... even)
    inline bool is_even(int n, int ntimes=1) {
        for (int i = 0; i < ntimes; i++) {
            if ((n % 2) != 0) return false;
            n /= 2;
        }
        return true;
    }

}; // namespace utils

#endif
