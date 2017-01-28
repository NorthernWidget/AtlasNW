/**
@file AtlasNW.cpp

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

////////////////////////////////////////////////////
// INCLUDE HEADER FILE AND ITS INCLUDED LIBRARIES //
////////////////////////////////////////////////////

#include <AtlasNW.h>


/////////////////////////////////////////////////////
// STATIC MEMBERS OF CLASS, SO ACCESSIBLE ANYWHERE //
/////////////////////////////////////////////////////

// Define numbers to represent different sensor types
const uint8_t conductivity = 1;
const uint8_t pH = 2;
const uint8_t ORP = 3;
const uint8_t DO = 4;
const uint8_t flow = 5;
const uint8_t temperature = 6;
const uint8_t color = 7;

// Define numbers to represent different communications types
const uint8_t use_UART = 1;
const uint8_t use_SoftSerial = 2;
const uint8_t use_I2C = 3;

// Global variables to hold information
uint8_t measurement_type;
uint8_t communications_type;

// If some kind of serial communication
// Defaults to the standard baud rate for the newer Atlas devices
// This should be redundant with the constructor
uint32_t baudRate=9600;

// If multiple UARTs
uint8_t UART_number;

// If software serial
uint8_t softSerRX;
uint8_t softSerTX;

// If I2C, save address
uint8_t I2C_address;

/////////////////
// CONSTRUCTOR //
/////////////////

// Overload constructor for different communications types

// Two options (ambiguous, so handle in one function):
// Hardware serial: char* char* uint8_t OPTuint32_t
// I2C: char* char* uint8_t
AtlasNW::AtlasNW(char* _measurement_type, char* _communications_type, uint8_t UART_number__I2C_address, uint32_t baudRate){
  Serial.end();
  Serial.begin(38400); // hard-coded for now
  if(strcmp(_measurement_type, "conductivity") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "pH") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "ORP") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "DO") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "flow") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "temperature") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "color") == 0){
    measurement_type = conductivity;
  };
  else{
    Serial.println("No valid sensor chosen. Halting.");
    while(1){};
    // Possibly add warning LED here
  }
  
  if(strcmp(_communications_type, "UART") == 0){
    communications_type = use_UART;
    //Atlas_UART comms;
  };
  else if(strcmp(_measurement_type, "SoftSerial") == 0){
    communications_type = use_SoftSerial;
    SoftwareSerial mySerial (softSerRX, softSerTX);
    //SoftSerial comms;
    
  };
  else if(strcmp(_measurement_type, "I2C") == 0){
    communications_type = use_I2C;
    //Atlas_SoftSerial comms;
    Wire.begin(); // Concerns about loading this more than once in different
                  // libraries. See:
                  // https://github.com/esp8266/Arduino/issues/2607
  };
  else{
    Serial.println("No valid measurement type chosen. Halting.");
    while(1){};
    // Possibly add warning LED here
  }
}

// Software serial: char* char* uint8_t uint8_t OPTuint32_t
AtlasNW::AtlasNW(char* _measurement_type, char* _communications_type, uint8_t _softSerRX, uint8_t _softSerTX, uint32_t _baudRate){
  Serial.end();
  Serial.begin(38400); // hard-coded for now
  if(strcmp(_measurement_type, "conductivity") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "pH") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "ORP") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "DO") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "flow") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "temperature") == 0){
    measurement_type = conductivity;
  };
  else if(strcmp(_measurement_type, "color") == 0){
    measurement_type = conductivity;
  };
  else{
    Serial.println("No valid sensor chosen. Halting.");
    while(1){};
    // Possibly add warning LED here
  }
  
  if(strcmp(_communications_type, "UART") == 0){
    communications_type = use_UART;
    //Atlas_UART comms;
  };
  else if(strcmp(_measurement_type, "SoftSerial") == 0){
    communications_type = use_SoftSerial;
    softSerRX = _softSerRX;
    softSerTX = _softSerTX;
    SoftwareSerial mySerial (softSerRX, softSerTX);
    //SoftSerial comms;
    
  };
  else if(strcmp(_measurement_type, "I2C") == 0){
    communications_type = use_I2C;
    //Atlas_SoftSerial comms;
    Wire.begin(); // Concerns about loading this more than once in different
                  // libraries. See:
                  // https://github.com/esp8266/Arduino/issues/2607
  };
  else{
    Serial.println("No valid measurement type chosen. Halting.");
    while(1){};
    // Possibly add warning LED here
  }
}

/*
AtlasNW::AtlasNW(char* _measurement_type, char* _communications_type, uint8_t I2C_address){
  //if(strcmp(str1, str2) == 0){};
}
*/

//////////////////
// SENDING DATA //
//////////////////

void AtlasNW::I2C_write(char* _transmission, _transmission_length){
  Wire.beginTransmission(I2C_address);
  Wire.write(_transmission, _transmission_length);
  Wire.write('\r');
  Wire.endTransmission();
}

// What if Serial is already being used?
void AtlasNW::Serial_write(char* _transmission){
  Serial.begin(baudRate);
  Serial.print(_transmission);
  Serial.print('\r');
  Serial.end();
}

// Just use software serial for now, to get the ball rolling in time
// to go to the field
void AtlasNW::SoftSer_write(char* _transmission){
  mySerial.begin(baudRate);
  mySerial.print(_transmission);
  mySerial.print('\r');
  mySerial.end();
}

////////////////////
// RECEIVING DATA //
////////////////////



///////////////////////
// SPECIFIC COMMANDS //
///////////////////////

// Hard-code for software serial for now

void AtlasNW::LED_on(bool _state){
  /*
   TRUE for LED on
   FALSE for LED off
   */
  char* _transmission;
  uint8_t response_byte;
  mySerial.begin(baudRate);
  mySerial.print("L,");
  mySerial.print(_state);
  mySerial.print("\r");
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
}

void AtlasNW::response_code_on(bool _state){
  /*
   TRUE for response code (*OK) on
   FALSE for *OK off
   */
  char* _transmission;
  uint8_t response_byte;
  mySerial.begin(baudRate);
  mySerial.print("RESPONSE,");
  mySerial.print(_state);
  mySerial.print("\r");
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
}

void AtlasNW::continuous_readings_on(bool _state){
  /*
   TRUE for ON, false for OFF
   */
  char* _transmission;
  uint8_t response_byte;
  mySerial.begin(baudRate);
  mySerial.print("C,");
  mySerial.print(_state);
  mySerial.print("\r");
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
}

void AtlasNW::set_K_constant(float K){
  /*
   For conductivity
   */
  char* _transmission;
  uint8_t response_byte;
  mySerial.begin(baudRate);
  mySerial.print("K,");
  mySerial.print(K);
  mySerial.print("\r");
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
}

void AtlasNW::set_Temperature(float T){
  /*
   For temperature calibration of EC
   */
  char* _transmission;
  uint8_t response_byte;
  mySerial.begin(baudRate);
  mySerial.print("T,");
  mySerial.print(T);
  mySerial.print("\r");
  // Time to process
  delay(300);
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
}

void AtlasNW::calibrate();
  /*
   Two-point calibration: low
   */
  char* _transmission;
  mySerial.begin(baudRate);
  Serial.println("Clearing calibration data");
  mySerial.print("Cal,clear\r");
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  Serial.println("Now dry the probe.");
  Serial.println("Enter any character and press <ENTER> when ready")
  Serial.println("to perform the dry calibration.");
  while(!Serial.available()); // Wait for input from user
  mySerial.print("Cal,dry\r");
  Serial.println("Now place the probe in the low-conductivity solution.");
  Serial.println("Enter the E.C. value [uS] of the solution and press <ENTER>");
  Serial.println("to begin low-point calibration");
  while(!Serial.available()); // Wait for input from user
  mySerial.print("Cal,low,");
  while Serial.available(){
    mySerial.print(Serial.read());
  }
  mySerial.print("\r")
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  Serial.println("Low-point calibration done.");
  Serial.println("Remove the probe from the low-conductivity solution.");
  Serial.println("Cap the low-conductivity solution.");
  Serial.println("Dry the probe.");
  Serial.println("Then place the probe in the high-conductivity solution.");
  Serial.println("Enter the E.C. value [uS] of the solution and press <ENTER>");
  Serial.println("to begin high-point calibration");
  while(!Serial.available()); // Wait for input from user
  mySerial.print("Cal,high,");
  while Serial.available(){
    mySerial.print(Serial.read());
  }
  mySerial.print("\r")
  // dump the response
  while mySerial.available(){
    response_byte = mySerial.read();
  }
  mySerial.end();
  Serial.println("Calibration complete.");
}

char* AtlasNW::read(bool _state){
  /*
   Read sensor
   */
  char* _transmission;
  char _tmpchar;
  char* response;
  mySerial.begin(baudRate);
  mySerial.print("R\n");
  // Record the response
  // Dump response code
  while mySerial.available(){
    _tmpchar = mySerial.read();
  }
  // But record actual response
  delay(1000);
  int i = 0;
  while mySerial.available(){
    _tmpchar = mySerial.read();
    // Don't write the carriage return
    while (_tmpchar != "\r"){
      response[i] = mySerial.read();
      i++;
    }
    //Dump any characters after carriage return
    _tmpchar = mySerial.read();
  }
  mySerial.end();
  return response;
}

//////////////////////
// MEMBER FUNCTIONS //
//////////////////////

  /**
   * @brief 
   * Atlas Scientific sensors: water properties and chemistry.
   * 
   * @details
   * Generalized serial interface for Atlas Scientific sensors. It uses 
   * SoftwareSerial AND THEREFORE IS LIKELY UNSTABLE AFTER A FEW DAYS IN THE
   * FIELD. The watchdog timer SHOULD BE ABLE TO CATCH THIS, but this has not
   * yet been tested.
   * 
   * Code in comments is on the way to replacing this with hardware serial.
   * It is also possible to replace this with I2C code, which will be more
   * versitile: can connect many sensors to same I2C port, so long as they
   * remain isolated.
   * 
   * @param command is the instruction code sent to the Atlas Scientific product.
   * See the data sheet for your specific sensor.
   * 
   * @param softSerRx is the software serial receive port
   * 
   * @param softSerTx is the software serial transmit port
   * 
   * @param baudRate is set by the Atlas Scientific sensor's baud rate.
   * 
   * @param printReturn is true if you determines whether you care about (i.e. want 
   * to print) the Serial response, and false if you would just like to clear 
   * the buffer.
   *
   * @param saveReturn is true if you want to save the response to the SD card.
   * 
   * Example:
   * ```
   * // read a pH probe using pins 7 and 8 as Rx and Tx, and save its results:
   * logger.AtlasScientific("R", D5, A7);
   * ```
   * 
   */

