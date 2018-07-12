#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include <rtthread.h>
#include <rthw.h>

rt_uint8_t wireless_getchar(void);
void wireless_putchar(const rt_uint8_t c);
void wireless_putstring(const rt_uint8_t *s);
rt_err_t wireless_open(const char *name);
//int wireless_init(void);
#endif
