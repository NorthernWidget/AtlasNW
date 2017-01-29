/**
@file AtlasNW.h

Arduino library for Atlas Scientific sensors, specially tailored towards
functionality with the Northern Widget ALog data logger (though written with 
generalized usability in mind).

# LICENSE: GNU GPL v3

Logger.cpp is part of Logger, an Arduino library written by Andrew D. Wickert
and Chad T. Sandell
Copyright (C) 2014-2017, Andrew D. Wickert
(Library as standalone starting in 2017, but earliest functions written in 2014)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/////////////////////////////////
// DEFINE SELF / INCLUDE GUARD //
/////////////////////////////////

#ifndef Logger_h
#define Logger_h

///////////////////////
// INCLUDE LIBRARIES //
///////////////////////

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <stdlib.h>

class AtlasNW {

  public:
    // Constructor
    AtlasNW(char* _measurement_type, char* _communications_type, uint8_t UART_number__I2C_address, uint32_t baudRate=9600);
    AtlasNW(char* _measurement_type, char* _communications_type, uint8_t _softSerRX, uint8_t _softSerTX, uint32_t _baudRate=9600);
    
    // Functions
    void AtlasNW::I2C_write(char* _transmission, uint8_t _transmission_length);
    void AtlasNW::Serial_write(char* _transmission);
    void AtlasNW::SoftSer_write(char* _transmission);
    void AtlasNW::LED_on(bool _state);
    void AtlasNW::response_code_on(bool _state);
    void AtlasNW::continuous_readings_on(bool _state);
    void AtlasNW::set_K_constant(float K);
    void AtlasNW::set_Temperature(float T);
    void AtlasNW::calibrate();
    char* AtlasNW::read();
    
};

// End include guard
#endif
