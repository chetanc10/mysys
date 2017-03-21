
#ifndef __OPTIMUS_H
#define __OPTIMUS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZEOF_ARRAY(arr_var) (sizeof (arr_var) / sizeof (*arr_var))

int test_circinc (uint8_t argc, char **argv);
int test_minmax (uint8_t argc, char **argv);
int test_sigunsig (uint8_t argc, char **argv);
int test_intalign (uint8_t argc, char **argv);
int test_remainder (uint8_t argc, char **argv);
int test_vassign (uint8_t argc, char **argv);
int test_funccall (uint8_t argc, char **argv);
int test_codeskip (uint8_t argc, char **argv);
int test_exitloop (uint8_t argc, char **argv);
int test_printdelay (uint8_t argc, char **argv);

#endif /*__OPTIMUS_H*/
