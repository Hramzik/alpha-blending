#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED
//--------------------------------------------------


#include <stdio.h>
#include <time.h>


//--------------------------------------------------


#include "../../lib/types/Return_code.hpp"


//--------------------------------------------------


#include "structs.hpp"


//--------------------------------------------------


double my_min (double val1, double val2);
size_t my_min (size_t val1, size_t val2);
int    my_min (int    val1, int    val2);

double my_max (double val1, double val2);
size_t my_max (size_t val1, size_t val2);
int    my_max (int    val1, int    val2);


//--------------------------------------------------


#endif