
#ifndef __XDUMP_H
#define __XDUMP_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <syslog.h>

#define USING_SYSLOG 1

/*define as 1 if we need the functions to log the buffer as hex-dump, 0 for nothing*/
#define USING_XDUMP 1

#define MAX_SBUF_LEN 8096 /*8KB*/

#define _FMT_8   1
#define FMT_8    "%"PRIx8
#define _FMT_16  2
#define FMT_16   "%"PRIx16
#define _FMT_32  3
#define FMT_32   "%"PRIx32
#define _FMT_64  4
#define FMT_64   "%"PRIx64

#if USING_XDUMP
/**
 * @function           : xdump
 * @brief              : xdump given a named buffer in given int format
 * @caller             : THOU SHAN'T CALL ME DIRECTLY!
 *                       Checkout my wrappers defined below myself
 * @input              : char *bufname - name of the buffer for proper identity
 *                       uint8_t *buf - buffer pointer
 *                       uint32_t len - length of buf
 *                       int fmt - format of integer selectable for xdump
 * @output             : none
 * @return             : none
 */
static inline void xdump (char *bufname, uint8_t *buf, uint32_t len, int fmt)
{
	uint32_t i = 0;
	uint32_t n = 0;
	char sbuf[MAX_SBUF_LEN];
	uint8_t *buf8;
	uint16_t *buf16;
	uint32_t *buf32;
	uint64_t *buf64;

	switch (fmt) {
		case _FMT_8:
			for (i = 0, buf8 = (uint8_t *)buf; i < len; i++) {
				n += sprintf (sbuf + n, FMT_8" ", buf8[i]);
			}
			break;
		case _FMT_16:
			for (i = 0, buf16 = (uint16_t *)buf; i < len; i++) {
				n += sprintf (sbuf + n, FMT_16" ", buf16[i]);
			}
			break;
		case _FMT_32:
			for (i = 0, buf32 = (uint32_t *)buf; i < len; i++) {
				n += sprintf (sbuf + n, FMT_32" ", buf32[i]);
			}
			break;
		case _FMT_64:
			for (i = 0, buf64 = (uint64_t *)buf; i < len; i++) {
				n += sprintf (sbuf + n, FMT_64" ", buf64[i]);
			}
			break;
	}
	sbuf[n] = '\0';

#if USING_SYSLOG
	syslog (LOG_DEBUG, "%s: %s\n", bufname, sbuf);
#else
	printf ("%s: %s\n", bufname, sbuf);
#endif
}
#endif

/**
 * @function           : xdump_xx (xx = 8 or 16 or 32 or 64 - bits)
 * @brief              : xdump given a named buffer in given int format
 *                       in 8 or 16 or 32 or 64 bit integer format as per name
 * @caller             : anyone who needs to debug buffer sent/received via n/w
 * @input              : char *bufname - name of the buffer for proper identity
 *                       uint8_t *buf - buffer pointer
 *                       uint32_t len - length of buf
 * @output             : none
 * @return             : none
 */
#if USING_XDUMP
#define xdump_8b(bufname, buf, len) \
	xdump (bufname, buf, len/sizeof (uint8_t), _FMT_8)
#define xdump_16b(bufname, buf, len) \
	xdump (bufname, buf, len/sizeof (uint16_t), _FMT_16)
#define xdump_32b(bufname, buf, len) \
	xdump (bufname, buf, len/sizeof (uint32_t), _FMT_32)
#define xdump_64b(bufname, buf, len) \
	xdump (bufname, buf, len/sizeof (uint64_t), _FMT_64)

#else
#define xdump_8b(bufname, buf, len)
#define xdump_16b(bufname, buf, len)
#define xdump_32b(bufname, buf, len)
#define xdump_64b(bufname, buf, len)

#endif /*USING_XDUMP*/

#endif /*__XDUMP_H*/

