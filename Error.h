#pragma once

#include "winapi.h"
#include <cstdio>

#include <assert.h>

#ifndef NDEBUG
#define	debug_log(...) printf(__VA_ARGS__)
#else
#define	debug_log(...)
#endif

void SysError(LPCWSTR message);