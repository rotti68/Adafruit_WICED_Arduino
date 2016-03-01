/**************************************************************************/
/*!
    @file     adafruit_mqtt_publisher.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
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

#ifndef _ADAFRUIT_MQTT_PUBLISHER_H_
#define _ADAFRUIT_MQTT_PUBLISHER_H_

#include <Arduino.h>
#include <adafruit_feather.h>

class AdafruitMQTTPublisher : public Print
{
protected:
  AdafruitMQTT& _mqtt;
  const char*   _topic;
  uint8_t       _qos;
  bool          _retained;

public:
  AdafruitMQTTPublisher(AdafruitMQTT& mqtt, const char* topic, uint8_t qos = MQTT_QOS_AT_MOST_ONCE) : _mqtt(mqtt)
  {
    _topic = topic;
    _qos  = qos;
    _retained = false;
  }

  void retain(bool on) { _retained = on; }

  virtual size_t write(const uint8_t *buf, size_t len)
  {
    _mqtt.publish(_topic, UTF8String(buf, len), _qos, _retained);
  }

  virtual size_t write(uint8_t ch) { return write(&ch, 1); }
  using Print::write;
};

#endif /* _ADAFRUIT_MQTT_PUBLISHER_H_ */
