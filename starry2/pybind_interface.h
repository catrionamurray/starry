/**
This defines the main Python interface to the code.

*/

#ifndef _STARRY_PYBIND_H_
#define _STARRY_PYBIND_H_

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <Eigen/Core>
#include <unsupported/Eigen/AutoDiff>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include "maps.h"
#include "docstrings.h"
#include "utils.h"
#include "errors.h"
#include "pybind_vectorize.h"


namespace pybind_interface {

    using namespace utils;
    using namespace pybind11::literals;
    namespace py = pybind11;
    namespace vectorize = pybind_vectorize;
    using std::min;
    using std::max;

    /**
    Re-interpret the `start`, `stop`, and `step` attributes of a `py::slice`,
    allowing for *actual* negative indices.

    */
    void reinterpret_slice(const py::slice& slice, const int smin,
                           const int smax, int& start, int& stop, int& step) {
        // This is super hacky. Because `m` indices can be negative, we need
        // to re-interpret what a slice with negative indices actually
        // means. Casting to an actual Python slice and running
        // `compute` interprets negative indices as indices counting
        // backwards from the end, which is not what we want. There
        // doesn't seem to be a way to reconstruct the original arguments
        // to `slice(start, stop, step)` that works in *all* cases (I've tried!)
        // so for now we'll parse the string representation of the slice.
        // This is likely slow, so a hack that digs into the actual
        // CPython backend and recovers the `start`, `stop`, and `step`
        // attributes of a `py::slice` object would be better. Suggestions welcome!
        std::ostringstream os;
        os << slice;
        std::string str_slice = std::string(os.str());
        size_t pos = 0;
        std::string str_start, str_stop, str_step;
        pos = str_slice.find(", ");
        str_start = str_slice.substr(6, pos - 6);
        str_slice = str_slice.substr(pos + 2, str_slice.size() - pos);
        pos = str_slice.find(", ");
        str_stop = str_slice.substr(0, pos);
        str_step = str_slice.substr(pos + 2, str_slice.size() - pos - 3);
        if (str_start == "None")
            start = smin;
        else
            start = stoi(str_start);
        if (str_stop == "None")
            stop = smax;
        else
            stop = stoi(str_stop);
        if (str_step == "None")
            step = 1;
        else
            step = stoi(str_step);
        if (step < 0)
            throw errors::ValueError("Slices with negative steps are not supported.");
    }

    /**

    */
    std::vector<int> get_Ylm_inds(const int lmax, const py::tuple& lm) {
        if (lm.size() != 2)
            throw errors::IndexError("Invalid `l`, `m` tuple.");
        std::vector<int> inds;
        if ((py::isinstance<py::int_>(lm[0])) && (py::isinstance<py::int_>(lm[1]))) {
            // User provided `(l, m)`
            int l = py::cast<int>(lm[0]);
            int m = py::cast<int>(lm[1]);
            inds.push_back(l * l + l + m);
            return inds;
        } else if ((py::isinstance<py::slice>(lm[0])) && (py::isinstance<py::slice>(lm[1]))) {
            // User provided `(slice, slice)`
            auto lslice = py::cast<py::slice>(lm[0]);
            auto mslice = py::cast<py::slice>(lm[1]);
            int lstart, lstop, lstep;
            int mstart, mstop, mstep;
            reinterpret_slice(lslice, 0, lmax, lstart, lstop, lstep);
            if ((lstart < 0) || (lstart > lmax))
                throw errors::IndexError("Invalid value for `l`.");
            for (int l = lstart; l < lstop + 1; l += lstep) {
                reinterpret_slice(mslice, -l, l, mstart, mstop, mstep);
                if (mstart < -l)
                    mstart = -l;
                if (mstop > l)
                    mstop = l;
                for (int m = mstart; m < mstop + 1; m += mstep) {
                    inds.push_back(l * l + l + m);
                }
            }
            return inds;
        } else if ((py::isinstance<py::int_>(lm[0])) && (py::isinstance<py::slice>(lm[1]))) {
            // User provided `(l, slice)`
            int l = py::cast<int>(lm[0]);
            auto mslice = py::cast<py::slice>(lm[1]);
            int mstart, mstop, mstep;
            reinterpret_slice(mslice, -l, l, mstart, mstop, mstep);
            if (mstart < -l)
                mstart = -l;
            if (mstop > l)
                mstop = l;
            for (int m = mstart; m < mstop + 1; m += mstep) {
                inds.push_back(l * l + l + m);
            }
            return inds;
        } else if ((py::isinstance<py::slice>(lm[0])) && (py::isinstance<py::int_>(lm[1]))) {
            // User provided `(slice, m)`
            int m = py::cast<int>(lm[1]);
            auto lslice = py::cast<py::slice>(lm[0]);
            int lstart, lstop, lstep;
            reinterpret_slice(lslice, 0, lmax, lstart, lstop, lstep);
            if ((lstart < 0) || (lstart > lmax))
                throw errors::IndexError("Invalid value for `l`.");
            for (int l = lstart; l < lstop + 1; l += lstep) {
                if ((m < -l) || (m > l))
                    continue;
                inds.push_back(l * l + l + m);
            }
            return inds;
        } else {
            // User provided something silly
            throw errors::IndexError("Unsupported input type for `l` and/or `m`.");
        }
    }

    /**

    */
    std::vector<int> get_Ul_inds(int lmax, const py::object& l) {
        std::vector<int> inds;
        if (py::isinstance<py::int_>(l)) {
            inds.push_back(py::cast<int>(l));
            return inds;
        } else if (py::isinstance<py::slice>(l)) {
            py::slice slice = py::cast<py::slice>(l);
            ssize_t start, stop, step, slicelength;
            if(!slice.compute(lmax,
                              reinterpret_cast<size_t*>(&start),
                              reinterpret_cast<size_t*>(&stop),
                              reinterpret_cast<size_t*>(&step),
                              reinterpret_cast<size_t*>(&slicelength)))
                throw pybind11::error_already_set();
            if ((start < 0) || (start > lmax)) {
                throw errors::IndexError("Invalid value for `l`.");
            } else if (step < 0) {
                throw errors::ValueError("Slices with negative steps are not supported.");
            } else if (start == 0) {
                // Let's give the user the benefit of the doubt here
                start = 1;
            }
            std::vector<int> inds;
            for (ssize_t i = start; i < stop + 1; i += step) {
                inds.push_back(i);
            }
            return inds;
        } else {
            // User provided something silly
            throw errors::IndexError("Unsupported input type for `l`.");
        }
    }

    template <typename T, int Module>
    void add_Map_extras(py::class_<maps::Map<T>>& PyMap,
                        const docstrings::docs<Module>& docs) {

        PyMap

            .def(py::init<int>(), "lmax"_a=2)

            .def("show", [](maps::Map<T> &map, std::string cmap, int res) {
                py::object show = py::module::import("starry2.maps").attr("show");
                Matrix<double> I;
                I.resize(res, res);
                Vector<Scalar<T>> x;
                x = Vector<Scalar<T>>::LinSpaced(res, -1, 1);
                for (int i = 0; i < res; i++){
                    for (int j = 0; j < res; j++){
                        I(j, i) = static_cast<double>(
                                  map.evaluate(0.0, x(i), x(j)));
                    }
                }
                show(I, "cmap"_a=cmap, "res"_a=res);
            }, docs.Map.show, "cmap"_a="plasma", "res"_a=300)

            .def("animate", [](maps::Map<T> &map, std::string cmap, int res,
                               int frames, std::string& gif) {
                std::cout << "Rendering..." << std::endl;
                py::object animate = py::module::import("starry2.maps").attr("animate");
                std::vector<Matrix<double>> I;
                Vector<Scalar<T>> x, theta;
                x = Vector<Scalar<T>>::LinSpaced(res, -1, 1);
                theta = Vector<Scalar<T>>::LinSpaced(frames, 0, 360);
                for (int t = 0; t < frames; t++){
                    I.push_back(Matrix<double>::Zero(res, res));
                    for (int i = 0; i < res; i++){
                        for (int j = 0; j < res; j++){
                            I[t](j, i) = static_cast<double>(
                                         map.evaluate(theta(t), x(i), x(j)));
                        }
                    }
                }
                animate(I, "cmap"_a=cmap, "res"_a=res, "gif"_a=gif);
             }, docs.Map.animate, "cmap"_a="plasma", "res"_a=150,
                                 "frames"_a=50, "gif"_a="")

             .def("load_image", [](maps::Map<T> &map, std::string& image, int lmax) {
                 py::object load_map = py::module::import("starry.maps").attr("load_map");
                 if (lmax == -1)
                    lmax = map.lmax;
                 Vector<double> y_double = load_map(image, map.lmax).template cast<Vector<double>>();
                 T y = y_double.template cast<Scalar<T>>();
                 Scalar<T> y_normed;
                 int n = 0;
                 for (int l = 0; l < lmax + 1; ++l) {
                     for (int m = -l; m < l + 1; ++m) {
                         y_normed = y(n) / y(0);
                         map.setYlm(l, m, y_normed);
                         ++n;
                     }
                 }
                 // We need to apply some rotations to get to the
                 // desired orientation, where the center of the image
                 // is projected onto the sub-observer point
                 auto map_axis = map.getAxis();
                 map.setAxis(xhat<Scalar<T>>());
                 map.rotate(90.0);
                 map.setAxis(zhat<Scalar<T>>());
                 map.rotate(180.0);
                 map.setAxis(yhat<Scalar<T>>());
                 map.rotate(90.0);
                 map.setAxis(map_axis);
             }, docs.Map.load_image, "image"_a, "lmax"_a=-1);

    }

    template <>
    void add_Map_extras(py::class_<maps::Map<Matrix<double>>>& PyMap,
                        const docstrings::docs<STARRY_MODULE_SPECTRAL>& docs) {

        PyMap

            .def(py::init<int, int>(), "lmax"_a=2, "nwav"_a=1)

            .def_property_readonly("nwav",
                [](maps::Map<Matrix<double>> &map){
                    return map.nwav;
                }, docs.Map.nwav);

            // TODO: SHOW, ANIMATE, LOAD_IMAGE

    }

    template <typename T, int Module>
    void add_Map(py::class_<maps::Map<T>>& PyMap,
                 const docstrings::docs<Module>& docs) {

        PyMap

            // Set one or more spherical harmonic coefficients to the same value
            .def("__setitem__", [](maps::Map<T>& map, py::tuple lm, RowDouble<T>& coeff) {
                auto inds = get_Ylm_inds(map.lmax, lm);
                auto y = map.getY();
                for (auto n : inds)
                    setRow(y, n, coeff);
                map.setY(y);
            })

            // Set one or more spherical harmonic coefficients to an array of values
            .def("__setitem__", [](maps::Map<T>& map, py::tuple lm, MapDouble<T>& coeff) {
                auto inds = get_Ylm_inds(map.lmax, lm);
                auto y = map.getY();
                int i = 0;
                Row<T> row;
                for (auto n : inds) {
                    row = getRow(coeff, i++);
                    setRow(y, n, row);
                }
                map.setY(y);
            })

            // Retrieve one or more spherical harmonic coefficients
            .def("__getitem__", [](maps::Map<T>& map, py::tuple lm) -> py::object {
                auto inds = get_Ylm_inds(map.lmax, lm);
                auto y = map.getY();
                MapDouble<T> res;
                resize(res, inds.size(), map.nwav);
                int i = 0;
                Row<T> row;
                for (auto n : inds) {
                    row = getRow(y, n);
                    setRow(res, i++, row);
                }
                if (inds.size() == 1)
                    return py::cast<RowDouble<T>>(getRow(res, 0));
                else
                    return py::cast<MapDouble<T>>(res);
            })

            // Set one or more limb darkening coefficients to the same value
            .def("__setitem__", [](maps::Map<T>& map, py::object l, RowDouble<T>& coeff) {
                auto inds = get_Ul_inds(map.lmax, l);
                auto u = map.getU();
                for (auto n : inds)
                    setRow(u, n - 1, coeff);
                map.setU(u);
            })

            // Set one or more limb darkening coefficients to an array of values
            .def("__setitem__", [](maps::Map<T>& map, py::object l, MapDouble<T>& coeff) {
                auto inds = get_Ul_inds(map.lmax, l);
                auto u = map.getU();
                int i = 0;
                Row<T> row;
                for (auto n : inds) {
                    row = getRow(coeff, i++);
                    setRow(u, n - 1, row);
                }
                map.setU(u);
            })

            // Retrieve one or more limb darkening coefficients
            .def("__getitem__", [](maps::Map<T>& map, py::object l) -> py::object {
                auto inds = get_Ul_inds(map.lmax, l);
                auto u = map.getU();
                MapDouble<T> res;
                resize(res, inds.size(), map.nwav);
                int i = 0;
                Row<T> row;
                for (auto n : inds) {
                    row = getRow(u, n - 1);
                    setRow(res, i++, row);
                }
                if (inds.size() == 1)
                    return py::cast<RowDouble<T>>(getRow(res, 0));
                else
                    return py::cast<MapDouble<T>>(res);
            })

            .def_property("axis",
                [](maps::Map<T> &map) -> UnitVector<double> {
                        return map.getAxis().template cast<double>();
                    },
                [](maps::Map<T> &map, UnitVector<double>& axis){
                        map.setAxis(axis.template cast<Scalar<T>>());
                    },
                docs.Map.axis)

            .def("reset", &maps::Map<T>::reset, docs.Map.reset)

            .def_property_readonly("lmax", [](maps::Map<T> &map){
                    return map.lmax;
                }, docs.Map.lmax)

            .def_property_readonly("N", [](maps::Map<T> &map){
                    return map.N;
                }, docs.Map.N)

            .def_property_readonly("y", [](maps::Map<T> &map) -> MapDouble<T>{
                        return map.getY().template cast<double>();
                    }, docs.Map.y)

            .def_property_readonly("u", [](maps::Map<T> &map) -> MapDouble<T>{
                        return map.getU().template cast<double>();
                    }, docs.Map.u)

            .def_property_readonly("p", [](maps::Map<T> &map) -> MapDouble<T>{
                    return map.getP().template cast<double>();
                }, docs.Map.p)

            .def_property_readonly("g", [](maps::Map<T> &map) -> MapDouble<T>{
                    return map.getG().template cast<double>();
                }, docs.Map.g)

            .def_property_readonly("r", [](maps::Map<T> &map) -> VectorT<double>{
                    return map.getR().template cast<double>();
                }, docs.Map.r)

            .def_property_readonly("s", [](maps::Map<T> &map) -> VectorT<double>{
                    return map.getS().template cast<double>();
                }, docs.Map.s)

            .def("evaluate", [](maps::Map<T> &map,
                                py::array_t<double>& theta,
                                py::array_t<double>& x,
                                py::array_t<double>& y,
                                bool gradient)
                                -> py::object {
                    return vectorize::evaluate(map, theta, x, y, gradient);
                }, docs.Map.evaluate, "theta"_a=0.0, "x"_a=0.0, "y"_a=0.0,
                                      "gradient"_a=false)

            .def("flux", [](maps::Map<T> &map,
                            py::array_t<double>& theta,
                            py::array_t<double>& xo,
                            py::array_t<double>& yo,
                            py::array_t<double>& ro,
                            bool gradient)
                            -> py::object {
                    return vectorize::flux(map, theta, xo, yo, ro, gradient);
                }, docs.Map.flux, "theta"_a=0.0, "xo"_a=0.0, "yo"_a=0.0,
                                   "ro"_a=0.0, "gradient"_a=false)

            .def("rotate", [](maps::Map<T> &map, double theta) {
                    map.rotate(static_cast<Scalar<T>>(theta));
            }, docs.Map.rotate, "theta"_a=0)

            .def("__repr__", &maps::Map<T>::__repr__);

        add_Map_extras(PyMap, docs);

    }

    template <int Module>
    void add_extras(py::module& m, const docstrings::docs<Module>& docs) { }

    template <>
    void add_extras(py::module& m,
                    const docstrings::docs<STARRY_MODULE_MULTI>& docs) {
        m.attr("NMULTI") = STARRY_NMULTI;
    }

    template <typename T, int Module>
    void add_starry(py::module& m, const docstrings::docs<Module>& docs) {

        // Main docs
        m.doc() = docs.doc;

        // Type-specific stuff
        add_extras(m, docs);

        // Surface map class
        py::class_<maps::Map<T>> PyMap(m, "Map", docs.Map.doc);
        add_Map(PyMap, docs);

    }

}; // namespace pybind_interface

#endif
