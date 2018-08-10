#include <stdlib.h>
#include <iostream>
#include "maps.h"
#include "utils.h"
using namespace utils;

int main() {

    int lmax = 2;

    // Spectral
    int nwav = 2;
    maps::Map<Matrix<double>, Vector<double>, VectorT<double>> map(lmax, nwav);
    for (int l = 0; l < lmax + 1; ++l) {
        for (int m = -l; m < l + 1; ++m) {
            map.setYlm(l, m, Vector<double>::Constant(nwav, 1));
        }
    }
    std::cout << map.evaluate(30, 0.1, 0.3, false).transpose() << std::endl;
    std::cout << map.flux(30, 0.1, 0.3, 0.1, false).transpose() << std::endl;

    // Monochromatic
    maps::Map<Vector<double>, double, double> map2(lmax);
    for (int l = 0; l < lmax + 1; ++l) {
        for (int m = -l; m < l + 1; ++m) {
            map2.setYlm(l, m, 1.0);
        }
    }
    std::cout << map2.evaluate(30, 0.1, 0.3, false) << std::endl;
    std::cout << map2.flux(30, 0.1, 0.3, 0.1, false) << std::endl;

}
