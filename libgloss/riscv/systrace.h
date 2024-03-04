#ifndef SYSTRACE_H
#define SYSTRACE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

extern void sys_tick(int code);
extern void sys_tock();

#endif
