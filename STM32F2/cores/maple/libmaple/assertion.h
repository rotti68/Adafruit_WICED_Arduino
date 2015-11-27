/******************************************************************************/
/*!
    @file     assertion.h
    @author   hathach (tinyusb.org)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    Copyright (c) 2015, Adafruit Industries (adafruit.com)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************/

/** \file
 *  \brief TBD
 */

#ifndef _ASSERTION_H_
#define _ASSERTION_H_

#include "compiler.h"
#include "adafruit_constants.h"

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------+
// Compile-time Assert
//--------------------------------------------------------------------+
#if defined __COUNTER__ && __COUNTER__ != __COUNTER__
  #define _ASSERT_COUNTER __COUNTER__
#else
  #define _ASSERT_COUNTER __LINE__
#endif

#define ASSERT_STATIC(const_expr) enum { XSTRING_CONCAT_(static_assert_, _ASSERT_COUNTER) = 1/(!!(const_expr)) }

//--------------------------------------------------------------------+
// Assert Helper
//--------------------------------------------------------------------+
#if CFG_DEBUG >= 2
//  #define ASSERT_MESSAGE(format, ...) fprintf(stderr, "[%08ld] %s: %d: " format "\r\n", get_millis(), __func__, __LINE__, __VA_ARGS__)
//  #define ASSERT_MESSAGE(format, ...) printf("[%08ld] %s: %d: " format "\r\n", get_millis(), __func__, __LINE__, __VA_ARGS__)
  #define ASSERT_MESSAGE(format, ...) fprintf(stderr, "%s: %d: " format "\r\n", __func__, __LINE__, __VA_ARGS__)
#else
  #define ASSERT_MESSAGE(format, ...)
#endif

#define VOID_RETURN
#define ASSERT_ERROR_HANDLER(x, para) return x

#define ASSERT_DEFINE_WITH_HANDLER(error_handler, handler_para, setup_statement, condition, error, format, ...) \
  do{\
    setup_statement;\
    if (!(condition)) {\
      ASSERT_MESSAGE(format, __VA_ARGS__);\
      error_handler(error, handler_para);\
    }\
  }while(0)

#define ASSERT_DEFINE(...) ASSERT_DEFINE_WITH_HANDLER(ASSERT_ERROR_HANDLER, NULL, __VA_ARGS__)

#define VERIFY_DEFINE(setup_statement, condition, error) \
  do{\
    setup_statement;\
    if (!(condition)) return error;\
  }while(0)

//--------------------------------------------------------------------+
// err_t Status Assert TODO use ASSERT_DEFINE
//--------------------------------------------------------------------+
#define ASSERT_STATUS_MESSAGE(sts, message) \
    ASSERT_DEFINE(err_t status = (err_t)(sts),\
                  ERROR_NONE == status, status, "%s: %s", ErrorStr[status], message)

#define ASSERT_STATUS(sts) \
    ASSERT_DEFINE(err_t status = (err_t)(sts),\
                  ERROR_NONE == status, status, "error = %d", status)

#define ASSERT_STATUS_RET_VOID(sts) \
    ASSERT_DEFINE(err_t status = (err_t)(sts),\
                  ERROR_NONE == status, (void) 0, "error = %d", status)

#define VERIFY_STATUS(sts)  \
    VERIFY_DEFINE(err_t _status = (err_t)(sts) , ERROR_NONE == _status, _status)

#define VERIFY_STATUS_RETVOID(sts)  \
    VERIFY_DEFINE(err_t _status = (err_t)(sts), ERROR_NONE == _status, VOID_RETURN)

//--------------------------------------------------------------------+
// Logical Assert
//--------------------------------------------------------------------+
#define ASSERT_FAULT(condition)          ASSERT(condition)
#if CFG_DEBUG
  #define ASSERT(condition)              ASSERT_RETVOID(condition)
#else
  #define ASSERT(condition)
#endif

#define ASSERT_RETVOID(condition)        ASSERT_DEFINE( , (condition), VOID_RETURN, "", "")
#define ASSERT_RETERR(...)               ASSERT_TRUE(__VA_ARGS__)

#define ASSERT_TRUE(condition  , error)  ASSERT_DEFINE( , (condition), error, "%s", "")
#define ASSERT_FALSE(condition , error)  ASSERT_DEFINE( ,!(condition), error, "%s", "")

#define VERIFY(condition, error)         VERIFY_DEFINE( , (condition), error)
#define VERIFY_RETVOID(condition)        VERIFY_DEFINE( , (condition), VOID_RETURN)

//--------------------------------------------------------------------+
// Pointer Assert
//--------------------------------------------------------------------+
#define ASSERT_PTR(...)                     ASSERT_PTR_NOT_NULL(__VA_ARGS__)
#define ASSERT_PTR_NOT_NULL(pointer, error) ASSERT_DEFINE( , NULL != (pointer), error, "%s", "pointer is NULL")
#define ASSERT_PTR_NULL(pointer, error)     ASSERT_DEFINE( , NULL == (pointer), error, "%s", "pointer is not NULL")

//--------------------------------------------------------------------+
// Integral Assert
//--------------------------------------------------------------------+
#define ASSERT_XXX_EQUAL(type_format, expected, actual, error) \
    ASSERT_DEFINE(\
                  uint32_t exp = (expected); uint32_t act = (actual),\
                  exp==act,\
                  error,\
                  "expected " type_format ", actual " type_format, exp, act)

#define ASSERT_XXX_WITHIN(type_format, lower, upper, actual, error) \
    ASSERT_DEFINE(\
                  uint32_t low = (lower); uint32_t up = (upper); uint32_t act = (actual),\
                  (low <= act) && (act <= up),\
                  error,\
                  "expected within " type_format " - " type_format ", actual " type_format, low, up, act)

//--------------------------------------------------------------------+
// Integer Assert
//--------------------------------------------------------------------+
#define ASSERT_INT(...)        ASSERT_INT_EQUAL(__VA_ARGS__)
#define ASSERT_INT_EQUAL(...)  ASSERT_XXX_EQUAL("%lu", __VA_ARGS__)
#define ASSERT_INT_WITHIN(...) ASSERT_XXX_WITHIN("%lu", __VA_ARGS__)

//--------------------------------------------------------------------+
// Hex Assert
//--------------------------------------------------------------------+
#define ASSERT_HEX(...)        ASSERT_HEX_EQUAL(__VA_ARGS__)
#define ASSERT_HEX_EQUAL(...)  ASSERT_XXX_EQUAL("0x%x", __VA_ARGS__)
#define ASSERT_HEX_WITHIN(...) ASSERT_XXX_WITHIN("0x%x", __VA_ARGS__)

//--------------------------------------------------------------------+
// Bin Assert
//--------------------------------------------------------------------+
#define BIN8_PRINTF_PATTERN "%d%d%d%d%d%d%d%d"
#define BIN8_PRINTF_CONVERT(byte)  \
  ((byte) & 0x80 ? 1 : 0), \
  ((byte) & 0x40 ? 1 : 0), \
  ((byte) & 0x20 ? 1 : 0), \
  ((byte) & 0x10 ? 1 : 0), \
  ((byte) & 0x08 ? 1 : 0), \
  ((byte) & 0x04 ? 1 : 0), \
  ((byte) & 0x02 ? 1 : 0), \
  ((byte) & 0x01 ? 1 : 0)

#define ASSERT_BIN8(...)        ASSERT_BIN8_EQUAL(__VA_ARGS__)
#define ASSERT_BIN8_EQUAL(expected, actual, error)\
    ASSERT_DEFINE(\
                  uint8_t exp = (expected); uint8_t act = (actual),\
                  exp==act,\
                  error,\
                  "expected " BIN8_PRINTF_PATTERN ", actual " BIN8_PRINTF_PATTERN, BIN8_PRINTF_CONVERT(exp), BIN8_PRINTF_CONVERT(act) )

//--------------------------------------------------------------------+
// DEBUG HELPER
//--------------------------------------------------------------------+

#define DBG_LOCATION()        fprintf(stderr, "%s: %d: \r\n", __func__, __LINE__)
#define PRINT_INT(x)          fprintf(stderr, #x " = %ld\r\n", (uint32_t) (x) )
#define PRINT_HEX(x)          fprintf(stderr, #x " = %08lx\r\n", (uint32_t) (x) )
#define PRINT_BUFFER(buf, n) \
  do {\
    uint8_t* p8 = (uint8_t*) (buf);\
    fprintf(stderr, #buf ": ");\
    for(uint32_t i=0; i<(n); i++) fprintf(stderr, "%02x ", p8[i]);\
    fprintf(stderr, "\r\n");\
  }while(0)

#ifdef __cplusplus
}
#endif

#endif /* _ASSERTION_H_ */

/** @} */
