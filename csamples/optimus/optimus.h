
#ifndef __OPTIMUS_H
#define __OPTIMUS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZEOF_ARRAY(arr_var) (sizeof (arr_var) / sizeof (*arr_var))

int test_circinc (uint8_t argc, char **argv);
int test_minmax (uint8_t argc, char **argv);
int test_ifcond (uint8_t argc, char **argv);

#endif /*__OPTIMUS_H*/