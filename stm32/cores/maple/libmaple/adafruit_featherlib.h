/**************************************************************************/
/*!
    @file     adafruit_featherlib.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

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
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _ADAFRUIT_FEATHERLIB_H_
#define _ADAFRUIT_FEATHERLIB_H_

#include <stdint.h>
#include "compiler.h"
#include "adafruit_constants.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Define UNISTD.h replacements */
#define STDIN_FILENO          0
#define STDOUT_FILENO         1
#define STDERR_FILENO         2
#define FILENO_USB_SERIAL     3
#define FILENO_USB_ATPARSER   4
//#define FILENO_UART           5

//------------- Arduino Shared Structure -------------//
#define CFG_ARDUINO_CODE_MAGIC    0xDEC0DED

typedef struct ATTR_ALIGNED(512)
{
  // Static Information
  uint32_t arduino_magic;
  uint32_t arduino_version;
  uint8_t  reserved[56];

  void (*startup) (void);

  // Peripheral Interrupt
  uint32_t reserved2[16];

  // USBCDC callback
  void (*cdc_serial_event_cb)(uint32_t eid, void* p_data);

  // HTTP callback
  void (*http_callback)(uint8_t* data, uint16_t data_length, uint16_t avail);

  // MQTT callback
  void (*mqtt_callback)(mqtt_evt_opcode_t event, uint16_t len, uint8_t* data);

}adafruit_arduino_t;


//------------- Featherlib Shared Structure -------------//
typedef struct
{
  uint16_t    len;
  void const* p_value;
}sdep_cmd_para_t;

typedef struct ATTR_ALIGNED(512)
{
  // Static Information
  uint32_t firmware_magic;
  uint32_t firmware_version;
  uint32_t codebase_verion;

  char     board_name[12];         // e.g "Feather"
  char     mcu_str[12];            // e.g "ST32F205RGY"
  char     firmware_builddate[12]; // e.g "Dec 07 2015"

  uint8_t  reserved[80];

  // SDEP Command
  uint16_t (*sdep_execute) (uint16_t  cmd_id     ,
                            uint16_t   paylen     , void const* parameter,
                            uint16_t* result_len , void* result_buffer);

  err_t (*sdep_execute_n)(uint16_t cmd_id,
                          uint8_t para_count, sdep_cmd_para_t const* para_arr,
                          uint16_t* p_result_len, void* p_result);

  uint32_t sdep_reserved[6];

  // RTOS API
  uint32_t (*rtos_delay_ms) (uint32_t ms);
  uint32_t (*rtos_delay_us) (uint32_t us);
  void     (*rtos_yield) (void);
  uint32_t rtos_reserved[13];

  // Peripheral API
  uint32_t (*system_millis) (void);
  uint32_t peripheral_reserved[15];

  // FILE Interface
  int (*file_write) (int file, char *ptr, int len);
  int (*file_read)  (int file, char *ptr, int len);
  int (*file_peek)  (int file);
  uint32_t file_reserved;

  //
}adafruit_featherlib_t;

#define FEATHERLIB_BASE    ((uint32_t) 0x8010200)
#define FEATHERLIB         ((adafruit_featherlib_t const*) FEATHERLIB_BASE)

#define DBG_SERIAL FILENO_UART
//#define DBG_SERIAL FILENO_UART

#define DBG_LOCATION()       do {\
    FEATHERLIB->file_write(DBG_SERIAL, (char*) __PRETTY_FUNCTION__, strlen(__PRETTY_FUNCTION__));\
    FEATHERLIB->file_write(DBG_SERIAL, ": ", 2);\
    FEATHERLIB->file_write(DBG_SERIAL, XSTRING_(__LINE__), strlen( XSTRING_(__LINE__) ));\
    FEATHERLIB->file_write(DBG_SERIAL, "\r\n", 2);\
  } while(0)

#define DBG_INT(x)       do {\
    FEATHERLIB->file_write(DBG_SERIAL, #x, strlen(#x));\
    FEATHERLIB->file_write(DBG_SERIAL, " = ", 3);\
    char ch[2] = { x+ '0', 0 } ;\
    FEATHERLIB->file_write(DBG_SERIAL, ch, 1);\
    FEATHERLIB->file_write(DBG_SERIAL, "\r\n", 2);\
  } while(0)

#ifdef __cplusplus
 }
#endif

#endif /* _ADAFRUIT_FEATHERLIB_H_ */