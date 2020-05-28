
#ifndef TEARDOWN_H
#define TEARDOWN_H

#ifdef OPENMC

#include "openmc/capi.h"

void teardown() { openmc_finalize(); }

#else

void teardown() {}

#endif

#endif