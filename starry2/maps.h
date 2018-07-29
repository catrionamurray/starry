/**
Defines the surface map class.

*/

#ifndef _STARRY_MAPS_H_
#define _STARRY_MAPS_H_

#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include "rotation.h"
#include "basis.h"
#include "errors.h"
#include "utils.h"
#include "vectorize.h"

namespace maps {

    using namespace utils;
    using namespace vectorize;
    using std::abs;
    using std::string;
    using rotation::Wigner;
    using basis::Basis;

    // ****************************
    // ----------------------------
    //
    //    The surface map class
    //
    // ----------------------------
    // ****************************
    template <class T>
    class Map {

        public:

            const int lmax;                             /**< The highest degree of the map */
            const int N;                                /**< The number of map coefficients */

        private:

            Vector<T> y;                                /**< The map coefficients in the spherical harmonic basis */
            Vector<T> p;                                /**< The map coefficients in the polynomial basis */
            Vector<T> g;                                /**< The map coefficients in the Green's basis */
            UnitVector<T> axis;                         /**< The axis of rotation for the map */
            std::map<string, Vector<double>> gradient;  /**< Dictionary of derivatives */
            Vector<T> dFdy;                             /**< Derivative of the flux w/ respect to the map coeffs */
            Basis<T> B;                                 /**< Basis transform stuff */
            Wigner<T> W;                                /**< The class controlling rotations */
            bool Y00_is_unity;                          /**< Flag: are we fixing the constant map coeff at unity? */
            Vector<T> tmp_vec;                          /**< A temporary surface map vector. */
            Vector<T>* tmp_vec_ptr;                     /**< A temporary pointer to a surface map vector. */

        public:

            /**
            Instantiate a `Map`.

            */
            Map(int lmax=2, bool Y00_is_unity=false) :
                lmax(lmax),
                N((lmax + 1) * (lmax + 1)),
                y(Vector<T>::Zero(N)),
                axis(yhat<T>()),
                B(lmax),
                W(lmax, (*this).y, (*this).axis),
                Y00_is_unity(Y00_is_unity) {

                // Initialize
                dFdy = Vector<T>::Zero(N);
                update();

            }

            // Housekeeping functions
            void update();
            void reset();

            // I/O functions
            void setCoeff(int l, int m, T coeff);
            void setCoeff(const Vector<int>& inds, const Vector<T>& coeffs);
            const T& getCoeff(int l, int m) const;
            const Vector<T> getCoeff(const Vector<int>& inds);
            void setAxis(const UnitVector<T>& new_axis);
            const UnitVector<T>& getAxis() const;
            const Vector<T>& getY() const;
            const Vector<T>& getP() const;
            const Vector<T>& getG() const;
            const VectorT<T>& getR() const;
            std::string __repr__();

            // Rotate the base map
            void rotate(const T& theta_deg);

            // Get the intensity of the map at a point
            inline T evaluate(const T& theta_deg=0, const T& x0=0, const T& y0=0);

    };


    /* ---------------- */
    /*   HOUSEKEEPING   */
    /* ---------------- */


    // Update the maps after the coefficients changed
    template <class T>
    void Map<T>::update() {

        // Update the polynomial and Green's map coefficients
        p = B.A1 * y;
        g = B.A * y;

        // Update the rotation matrix
        W.update();

    }

    // Reset the map
    template <class T>
    void Map<T>::reset() {
        y.setZero(N);
        if (Y00_is_unity) y(0) = 1;
        axis = yhat<T>();
        update();
    }


    /* ---------------- */
    /*        I/O       */
    /* ---------------- */


    // Set the (l, m) coefficient
    template <class T>
    void Map<T>::setCoeff(int l, int m, T coeff) {
        if ((l == 0) && (Y00_is_unity) && (coeff != 1))
            throw errors::ValueError("The Y_{0,0} coefficient is fixed at unity. "
                                     "You probably want to change the body's "
                                     "luminosity instead.");
        if ((0 <= l) && (l <= lmax) && (-l <= m) && (m <= l)) {
            int n = l * l + l + m;
            y(n) = coeff;
            update();
        } else {
            throw errors::IndexError("Invalid value for `l` and/or `m`.");
        }
    }

    // Set several coefficients at once using a single index
    template <class T>
    void Map<T>::setCoeff(const Vector<int>& inds, const Vector<T>& coeffs) {

        // Ensure sizes match
        if (inds.size() != coeffs.size())
            throw errors::IndexError("Size mismatch between `inds` and `coeffs`.");

        // Loop through and set each coeff
        for (int i = 0; i < inds.size(); i++) {
            if ((inds(i) == 0) && (Y00_is_unity) && (coeffs(i) != 1))
                throw errors::ValueError("The Y_{0,0} coefficient is fixed at unity. "
                                         "You probably want to change the body's "
                                         "luminosity instead.");
            else if ((inds(i) < 0) || (inds(i) > N))
                throw errors::IndexError("Invalid index.");
            else
                y(inds(i)) = coeffs(i);
        }

        // Update stuff
        update();

    }

    // Get the (l, m) coefficient
    template <class T>
    const T& Map<T>::getCoeff(int l, int m) const {
        if ((0 <= l) && (l <= lmax) && (-l <= m) && (m <= l))
            return y(l * l + l + m);
        else
            throw errors::IndexError("Invalid value for `l` and/or `m`.");
    }

    // Get several coefficients at once using a single index
    template <class T>
    const Vector<T> Map<T>::getCoeff(const Vector<int>& inds) {
        Vector<T> coeffs(inds.size());
        for (int i = 0; i < inds.size(); i++) {
            if ((inds(i) < 0) || (inds(i) > N))
                throw errors::IndexError("Invalid index.");
            else
                coeffs(i) = y(inds(i));
        }
        return coeffs;
    }

    // Set the axis
    template <class T>
    void Map<T>::setAxis(const UnitVector<T>& new_axis) {

        // Normalize it
        axis = new_axis / sqrt(new_axis(0) * new_axis(0) +
                               new_axis(1) * new_axis(1) +
                               new_axis(2) * new_axis(2));

        // Update the rotation matrix
        W.update();

    }

    // Return a copy of the axis
    template <class T>
    const UnitVector<T>& Map<T>::getAxis() const {
        return axis;
    }

    // Get the spherical harmonic vector
    template <class T>
    const Vector<T>& Map<T>::getY() const {
        return y;
    }

    // Get the polynomial vector
    template <class T>
    const Vector<T>& Map<T>::getP() const {
        return p;
    }

    // Get the Green's vector
    template <class T>
    const Vector<T>& Map<T>::getG() const {
        return g;
    }

    // Get the rotation solution vector
    template <class T>
    const VectorT<T>& Map<T>::getR() const {
        return B.rT;
    }

    // Return a human-readable map string
    template <class T>
    std::string Map<T>::__repr__() {
        int n = 0;
        int nterms = 0;
        char buf[30];
        std::ostringstream os;
        os << "<STARRY Map: ";
        for (int l = 0; l < lmax + 1; l++) {
            for (int m = -l; m < l + 1; m++) {
                if (abs(y(n)) > 10 * mach_eps<T>()){
                    // Separator
                    if ((nterms > 0) && (y(n) > 0)) {
                        os << " + ";
                    } else if ((nterms > 0) && (y(n) < 0)){
                        os << " - ";
                    } else if ((nterms == 0) && (y(n) < 0)){
                        os << "-";
                    }
                    // Term
                    if ((y(n) == 1) || (y(n) == -1)) {
                        sprintf(buf, "Y_{%d,%d}", l, m);
                        os << buf;
                    } else if (fmod(abs(y(n)), 1.0) < 10 * mach_eps<T>()) {
                        sprintf(buf, "%d Y_{%d,%d}", (int)abs(y(n)), l, m);
                        os << buf;
                    } else if (fmod(abs(y(n)), 1.0) >= 0.01) {
                        sprintf(buf, "%.2f Y_{%d,%d}", abs(y(n)), l, m);
                        os << buf;
                    } else {
                        sprintf(buf, "%.2e Y_{%d,%d}", abs(y(n)), l, m);
                        os << buf;
                    }
                    nterms++;
                }
                n++;
            }
        }
        if (nterms == 0)
            os << "Null";
        os << ">";
        return std::string(os.str());
    }


    /* ------------- */
    /*   ROTATIONS   */
    /* ------------- */


    // Rotate the base map in-place given `theta` in **degrees**
    template <class T>
    void Map<T>::rotate(const T& theta_deg) {
        T theta_rad = theta_deg * (PI<T>() / 180.);
        W.rotate(cos(theta_rad), sin(theta_rad), y);
        update();
    }


    /* ------------- */
    /*   INTENSITY   */
    /* ------------- */


    // Evaluate our map at a given (x0, y0) coordinate
    template <class T>
    inline T Map<T>::evaluate(const T& theta_deg, const T& x0, const T& y0) {

        // Convert to radians
        T theta_rad = theta_deg * (PI<T>() / 180.);

        // Rotate the map into view
        if (theta_rad == 0) {
            tmp_vec_ptr = &p;
        } else {
            W.rotate(cos(theta_rad), sin(theta_rad), tmp_vec);
            tmp_vec = B.A1 * tmp_vec;
            tmp_vec_ptr = &tmp_vec;
        }

        // Check if outside the sphere
        if (x0 * x0 + y0 * y0 > 1.0) return T(NAN);

        int l, m, mu, nu, n = 0;
        T z0 = sqrt(1.0 - x0 * x0 - y0 * y0);

        // Evaluate each harmonic
        T res = 0;
        for (l=0; l<lmax+1; l++) {
            for (m=-l; m<l+1; m++) {
                if (abs((*tmp_vec_ptr)(n)) > 10 * mach_eps<T>()) {
                    mu = l - m;
                    nu = l + m;
                    if ((nu % 2) == 0) {
                        if ((mu > 0) && (nu > 0))
                            res += (*tmp_vec_ptr)(n) * pow(x0, mu / 2) * pow(y0, nu / 2);
                        else if (mu > 0)
                            res += (*tmp_vec_ptr)(n) * pow(x0, mu / 2);
                        else if (nu > 0)
                            res += (*tmp_vec_ptr)(n) * pow(y0, nu / 2);
                        else
                            res += (*tmp_vec_ptr)(n);
                    } else {
                        if ((mu > 1) && (nu > 1))
                            res += (*tmp_vec_ptr)(n) * pow(x0, (mu - 1) / 2) * pow(y0, (nu - 1) / 2) * z0;
                        else if (mu > 1)
                            res += (*tmp_vec_ptr)(n) * pow(x0, (mu - 1) / 2) * z0;
                        else if (nu > 1)
                            res += (*tmp_vec_ptr)(n) * pow(y0, (nu - 1) / 2) * z0;
                        else
                            res += (*tmp_vec_ptr)(n) * z0;
                    }
                }
                n++;
            }

        }
        return res;

    }

}; // namespace maps

#endif
