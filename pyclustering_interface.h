#ifndef PYCLUSTERING_INTERFACE_H_INCLUDED
#define PYCLUSTERING_INTERFACE_H_INCLUDED





#pragma once


#include <pyclustering_package.hpp>


/**
 *
 * @brief   Deallocate pyclustering package.
 *
 * @param[in]: package: pointer to clustering results.
 *
 */
extern "C" DECLARATION void free_pyclustering_package(pyclustering_package * package);


#endif // PYCLUSTERING_INTERFACE_H_INCLUDED
