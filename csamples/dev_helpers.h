
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/**
 * Taking from various sources, we may come back with more..!
 * http://stackoverflow.com/questions/1772119/c-the-most-useful-user-made-c-macros-in-gcc-also-c99
 * http://www.hudku.com/blog/essential-macros-for-c-programming/
 *
 */

/* ABC - Just a token specific to your project.
 * Can be replaced with yours; It better be 3-4 letter token.
 * We don't want lengthy macros now.. Do we? */

/**
 * @func    : ABC_MEMSET_S
 * @brief   : Simple and flexible wrapper to memset
 * 			  application only if ptr type is not (void *)
 * @inputs  : ptr - datatype of pointer
 *            n - number of elements
 * @outputs : none
 * @return  : none
 */
#define ABC_MEMSET(ptr, n) memset (ptr, 0 , n * sizeof (*ptr))
/**
 * @func   : ABC_BCALLOC
 * @brief  : Allocates zeroed n-bytes memory using calloc;
 * @input  : ptr - pointer to buffer to be allocated
 *           size - size of buffer to be allocated
 * @output : ptr - return pointer of calloc
 * @return : ptr
 * @eg     : ABC_BCALLOC (ptr, 8);
 *           if (ABC_BCALLOC (ptr, sizeof (struct timeval))) {//do something}
 */
#define ABC_BCALLOC(ptr, size) (ptr = calloc (1, size))

/**
 * @func   : ABC_MALLOC
 * @brief  : Allocates n-bytes structure specific memory using malloc;
 *           A simple and flexible wrapper to malloc;
 *           We assume the datatype of memory allocated is same as ptr's
 * @input  : ptr - pointer to buffer to be allocated
 *           n - number of elements (structures or standard data-types)
 * @output : ptr - return pointer of calloc
 * @return : ptr
 * @eg     : ABC_MALLOC (ptr, 8);
 *           if (!ABC_MALLOC (ptr, 4)) {//do something}
 */
#define ABC_MALLOC(ptr, n) (ptr = malloc (ptr, n * sizeof(*ptr)))

/**
 * @func   : ABC_CALLOC
 * @brief  : Allocates zeroed n-bytes structure specific memory using calloc;
 *           A simple and flexible wrapper to calloc;
 *           We assume the datatype of memory allocated is same as ptr's
 * @input  : ptr - pointer to buffer to be allocated
 *           n - number of elements (structures or standard data-types)
 * @output : ptr - return pointer of calloc
 * @return : ptr
 * @eg     : ABC_CALLOC (ptr, 8);
 *           if (!ABC_CALLOC (ptr, 4)) {//do something}
 */
#define ABC_CALLOC(ptr, n) (ptr = calloc (ptr, n, sizeof(*ptr)))

/**
 * @func    : ABC_FREE
 * @brief   : Frees memory if the pointer is valid and sets pointer to NULL
 * @inputs  : ptr
 * @outputs : none
 * @return  : void
 */
#define ABC_FREE(ptr) \
	do { if (ptr) { free (ptr); ptr = NULL; } } while (0)

/**
 * @func    : ABC_CLOSE
 * @brief   : Closes file descriptor if valid and sets fd to 0
 * @inputs  : file descriptor - int fd
 * @outputs : errno, when retError is 1 (User needs it)
 * @return  :
 */
#define ABC_CLOSE(fd) \
	do { if (fd > 0) { close (fd); fd = 0; } } while (0)

/**
 * @func    : ABC_CLOSE_V
 * @brief   : Closes file descriptor if valid and sets fd to 0
 *            and returns errno (status of close operation)
 * @inputs  : int fd - file descriptor
 * @outputs : errno
 * @return  :
 */
static inline int ABC_CLOSE_V (int fd)
{
	if (fd > 0) {
		close (fd);
		fd = 0;
		return errno;
	}
}

/**
 * @func    : RET_ON_ERR
 * @brief   : save status of 'cond' into 'ret' and if ret is non-zero,
 *            print user's error log if any and return ret value
 * @inputs  : cond - function call or a value or variable
 *            ret - return variable
 *            fmt - format string for error log (if "", nothing is printed)
 *            args - standard args notation required by printf family
 * @outputs : ret
 * @return  : ret
 */
#define RET_ON_ERR(cond, ret, fmt, args...)      \
    do {                                         \
        if (0 != (ret = cond)) {                 \
            if (*fmt) printf (fmt, ##args);      \
            return ret;                          \
        }                                        \
    } while (0)

/**
 * @func    : JUMP_ON_ERR
 * @brief   : save status of 'cond' into 'ret' and if ret is non-zero,
 *            print user's error log if any and GO TO Label CLEAN_EXIT;
 *            User can replace the label to any other label name like
 *            FAIL_CASE, CLEANUP, RELEASE, etc
 * @inputs  : cond - function call or a value or variable
 *            ret - return variable
 *            fmt - format string for error log (if "", nothing is printed)
 *            args - standard args notation required by printf family
 * @outputs : ret
 * @return  : none
 */
#define JUMP_ON_ERR(cond, ret, fmt, args...)     \
    do {                                         \
        if (0 != (ret = cond)) {                 \
            if (*fmt) printf (fmt, ##args);      \
            goto CLEAN_EXIT;                     \
        }                                        \
    } while (0)

/**
 * @func    : JUMP_ON_PERR
 * @brief   : if 'cond' is nonzero, print user's error log if any and
 *            save errno value into ret and then GO TO Label CLEAN_EXIT;
 *            User can replace the label to any other label name like
 *            FAIL_CASE, CLEANUP, RELEASE, etc
 * @inputs  : cond - function call or a value or variable
 *            ret - return variable
 *            fmt - format string for error log (if "", nothing is printed)
 *            args - standard args notation required by printf family
 * @outputs : ret (set to errno - standard linux error number)
 * @return  : none
 */
#define JUMP_ON_PERR(cond, ret, fmt, args...)    \
    do {                                         \
        if (0 != cond) {                         \
            if (*fmt) printf (fmt, ##args);      \
            ret = errno;                         \
            goto CLEAN_EXIT;                     \
        }                                        \
    } while (0)

/**
 * @func    : JUMPL_ON_ERR
 * @brief   : save status of 'cond' into 'ret' and if ret is non-zero,
 *            print user's error log if any and GO TO given label;
 * @inputs  : cond - function call or a value or variable
 *            ret - return variable
 *            fmt - format string for error log (if "", nothing is printed)
 *            args - standard args notation required by printf family
 * @outputs : ret
 * @return  : none
 */
#define JUMPL_ON_ERR(cond, ret, label, fmt, args...) \
    do {                                             \
        if (0 != (ret = cond)) {                     \
            if (*fmt) printf (fmt, ##args);          \
            goto label;                              \
        }                                            \
    } while (0)

/**
 * @func    : JUMPL_ON_PERR
 * @brief   : if 'cond' is nonzero, print user's error log if any and
 *            save errno value into ret and then GO TO given label;
 * @inputs  : cond - function call or a value or variable
 *            ret - return variable
 *            fmt - format string for error log (if "", nothing is printed)
 *            args - standard args notation required by printf family
 * @outputs : ret (set to errno - standard linux error number)
 * @return  : none
 */
#define JUMPL_ON_PERR(cond, ret, label, fmt, args...) \
    do {                                              \
        if (0 != cond) {                              \
            if (*fmt) printf (fmt, ##args);           \
            ret = errno;                              \
            goto label;                               \
        }                                             \
    } while (0)

/**
 * @func    : ABC_MIN
 * @brief   : return minimum of given two numbers
 * @inputs  : a and b - two numbers (Can't guarantee about Floating point)
 * @outputs : Maximum of the two values
 * @return  : Maximum of the two values
 */
#define ABC_MIN(a,b) (a<b)?a:b

/**
 * @func    : ABC_ARRAY_SZ
 * @brief   : return size of array
 * @inputs  : a (of whatever type except void in which case typecast is must)
 * @outputs : size of array
 * @return  : size of array
 */
#define ABC_ARRAY_SZ(a) (sizeof(a) / sizeof(*a))

/**
 * @func    : ABC_SWAP
 * @brief   : swap two integers
 * @inputs  : a & b - two integers
 * @outputs : swapped values in a & b
 * @return  : none
 */
#define ABC_SWAP(a, b)          \
	do {                        \
		(typeof (a)) tswap = a; \
		a = b;                  \
		b = tswap;              \
	}

/**
 * @func    : BVAL
 * @brief   : shift 1 by n places to the left
 * @inputs  : n - number of times the shift happens
 * @outputs : 1 << n
 * @return  : 1 << n
 */
#ifndef BVAL
#define BVAL(n) (1 << (n))
#endif

/**
 * @func    : BVAL_V
 * @brief   : same as BVAL, but validate the shift number
 * @inputs  : n - number of times the shift happens
 * @outputs : 1 << n
 * @return  : 1 << n for n >= 0, 
 * 			  0 for n < 0
 */
#ifndef BVAL_V
#define BVAL_V(n) (((n) < 0)?(n):(1 << (n)))
#endif

/**
 * @func    : LSB_BITSET
 * @brief   : get the LSB which is set in a given integer
 * @inputs  : n - an integer
 * @outputs : bit location - starting from bit 0 to relevant range
 * @return  : bit location - starting from bit 0 to relevant range
 */
#define LSB_SETBIT(x) (((x) ^ ((x) - 1) & (x)) - 1)

/**
 * @func    : UPDATE_COUNT
 * @brief   : takes last pkt-count and cur pkt-count and does the count update
 * 			  such that the actual pkt-count to be added is properly taken and
 * 			  added to backup counter in software
 * @inputs  : cur_hw_cnt, cur_sw_cnt, last_hw_cnt
 * @outputs : updated cur_sw_cnt and last_hw_cnt
 * @return  : last_hw_cnt
 */
#define UPDATE_COUNT (cur_hw_cnt, cur_sw_cnt, last_hw_cnt) \
{ \
	cur_sw_cnt += (0x1000000000LL + cur_hw_cnt - last_hw_cnt) & 0xFFFFFFFFFLL; \
	last_hw_cnt = cur_hw_cnt; \
}

/**
 * @func    : 
 * @brief   : 
 * @inputs  : 
 * @outputs : 
 * @return  : 
 */

