/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#ifdef PIO_UNIT_TESTING
#include <unity.h>
// #warning "Compiling for unit testing"
#else
#define TEST_PRINTF(message, ...) 
#define TEST_MESSAGE(message) 
#endif
