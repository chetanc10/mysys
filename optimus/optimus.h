
#ifndef __OPTIMUS_H
#define __OPTIMUS_H

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <emmintrin.h>

#define USING_CLOCK_LIB_API 1

#if USING_CLOCK_LIB_API
#define clock clock

#else
#define clock _read_ts
static inline __attribute__((always_inline)) uint64_t _read_ts (void)
{ 
    unsigned int eax_reg_rdtsc, edx_reg_rdtsc;
    unsigned long long int uptime;
    
    asm volatile  ("rdtsc\n\t" 
            "mov %%eax, %0\n\t"
            "mov %%edx, %1\n\t"
            :       "=r" (eax_reg_rdtsc) , "=r" (edx_reg_rdtsc)
            :
            : "eax" , "edx"
            );
    
    uptime =  ((unsigned long long int)edx_reg_rdtsc << 32) | eax_reg_rdtsc;
    return uptime;
}

#endif

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
int test_absval (uint8_t argc, char **argv);
int test_structbits (uint8_t argc, char **argv);
int test_bitset (uint8_t argc, char **argv);
int test_condret (uint8_t argc, char **argv);
int test_zerofy (uint8_t argc, char **argv);
int test_quotient (uint8_t argc, char **argv);
int test_mcopy (uint8_t argc, char **argv);
int test_cache (uint8_t argc, char **argv);

#endif /*__OPTIMUS_H*/

