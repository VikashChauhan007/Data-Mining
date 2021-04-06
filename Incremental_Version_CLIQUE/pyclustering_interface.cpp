/*!

@authors Andrei Novikov (pyclustering@yandex.ru)
@date 2014-2020
@copyright BSD-3-Clause

*/


#include <pyclustering_interface.hpp>


//#include <pyclustering_interface.hpp>

void free_pyclustering_package(pyclustering_package * package) {
    delete package;
}
