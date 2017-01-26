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

//... crickets

/////////////////
// CONSTRUCTOR //
/////////////////

AtlasNW::AtlasNW(){}

//////////////////////
// MEMBER FUNCTIONS //
//////////////////////

void Logger::AtlasScientific_SoftwareSerial(char* command, int softSerRX, \
               int softSerTX, uint32_t baudRate, bool printReturn, \
               bool saveReturn){
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

  // Serial Number is from an old version of the code
  // SerialNumber and baudRate default to 0 and 38400, respectively.
  // SerialNumber could be changed for Arduino Mega.

  // Check if re-instantiating class causes problems -- maybe this is what 
  // happened with Nick's loggers (but then why different times before
  // failure?
  // Same name, so maybe overwrite memory in same place?
  // Also, general timing issues with software serial; consider using 
  // hardware serial for this function.
  SoftwareSerial mySerial(softSerRX, softSerTX);

  mySerial.begin(baudRate);
  //Serial2.println("L,0\r");
  
  /*
  // HARDWARE SERIAL
  uint32_t start_millis_clear_buffer = millis();
  Serial2.write(13); // Clear buffer -- carriage return
  while ( (millis() - start_millis_clear_buffer < 500) || Serial2.available() ){
    Serial2.read();
  }
  */

  String sensorString = ""; // a char array to hold the data from the Atlas Scientific product
                         // Atlas' example indicates that they expect all returns to be <= 30 bytes
  sensorString.reserve(30); // Sets aside memory for the Sring
  char inChar; // incoming characters on the serial port

  // "println" prints <values>\r\n. Atlas uses println to send data in its 
  // Arduino sample code, so I presume the "\r" is taken as the cutoff (its 
  // sensors terminate with a carriage return) and the "\n" is discarded.
  // Still, I wonder if it would be better to "Serial.print(...\n);"
  // (or ASCII 13, however Arduino does it) explicitly
  mySerial.print(command); // send the command to the Atlas Scientific product
  mySerial.write(13); // carriage return to signal the end of the command
  
  mySerial.flush(); // Wait for transmit to finish
  
  delay(100);

  // Safety guard in case all data not received -- won't hang for > t_timeout
  // milliseconds
  int t_timeout = 5000;
  uint32_t start_millis = millis();
  bool endflag = false;
  
  // t_timeout just to ensure that there is no hang if signal isn't good
  while ((millis() - start_millis < t_timeout) && !endflag){
    if (mySerial.available()){
      inChar = mySerial.read();
      // Serial.print(inChar); // uncomment for debugging input from sensor
      if (inChar != '\r'){
        sensorString += inChar;
      }
      else{
        endflag = true;
      }
    }
  }
  if (saveReturn){
    datafile.print(sensorString); // Should work without clock's CSpinRTC -- digging into object that is already made
    datafile.print(F(","));
  }
  // Echo to serial
  if (saveReturn || printReturn){
    Serial.print(sensorString);
    Serial.print(F(","));
  }

  // Get rid of the incoming Serial stream
  while( mySerial.read() != -1 );

  mySerial.end();

}

/*
void Logger::AtlasScientific_LEDtest(){
  Serial.begin(38400);
  Serial.write(13); // Clear buffer -- carriage return
  Serial.print("L,1");
  Serial.write(13);
  Serial.end();
}

void Logger::AtlasScientific_test(char* command){
  Serial.begin(38400);
  Serial.write(13); // Clear buffer -- carriage return
  Serial.print(command);
  Serial.write(13);
  Serial.end();
}
*/

void Logger::AtlasScientific_UART(char* command, int SerialNumber, 
                        uint32_t baudRate, bool printReturn, bool saveReturn){
  // * "command" is the command sent to the Atlas Scientific product.
  //   see the data sheet as well as the above quick lists
  // SerialNumber and baudRate default to 0 and 38400, respectively.
  // SerialNumber could be changed for Arduino Mega.
  // baudRate will stay constant insofar as Atlas sensors' baud rates do.
  // getReturn determines whether you care about the Serial response, or whether
  // you would just like to clear the buffer.
  //

  Serial.begin(baudRate);

  String sensorString = ""; // a char array to hold the data from the Atlas Scientific product
                         // Atlas' example indicates that they expect all returns to be <= 30 bytes
  sensorString.reserve(30); // Sets aside memory for the Sring
  char inChar; // incoming characters on the serial port

  // "println" prints <values>\r\n. Atlas uses println to send data in its 
  // Arduino sample code, so I presume the "\r" is taken as the cutoff (its 
  // sensors terminate with a carriage return) and the "\n" is discarded.
  // Still, I wonder if it would be better to "Serial.print(...\n);"
  // (or ASCII 13, however Arduino does it) explicitly
  Serial.print(command); // send the command to the Atlas Scientific product
  Serial.write(13); // carriage return to signal the end of the command
  
  Serial.flush(); // Wait for transmit to finish
  
  delay(100);

  //delay();
  
  // Safety guard in case all data not received -- won't hang for > 30 seconds
  uint32_t start_millis = millis();
  
  bool endflag = false;
  
  // "5000" just to ensure that there is no hang if signal isn't good
  while ((millis() - start_millis < 5000) && !endflag){
    // used to have if (printReturn || saveReturn) here, but
    // should read return either way
    if (Serial.available()){
      inChar = Serial.read();
      // Serial.print(inChar); // uncomment for debugging input from sensor
      if (inChar != '\r'){
        //sensorString[i] = inChar;
        sensorString += inChar;
      }
      else{
        endflag = true;
      }
    }
  }
  // save iff both getReturn and saveReturn are true
  // Currently also echoes the return to serial port if it will
  // also be saved (sent to SD card)
  if (saveReturn){
    SDstart();
    // Should work without clock's CSpinRTC -- digging into object that is
    // already made
    datafile.print(sensorString);
    datafile.print(",");
    SDend();
  }
  // Echo to serial
  if (saveReturn || printReturn){
    Serial.print(sensorString);
    Serial.print(F(","));
  }
  /*
  else if (printReturn){ // so !savereturn
    Serial.println();
    Serial.print("Sensor returns: ");
    Serial.println(sensorString);
    Serial.println();
  }
  */
  
  // Get rid of the incoming Serial stream
  // get rid of the rest of the buffer after Line 1
  //while (_tx_buffer->head != _tx_buffer->tail);
  while( Serial.read() != -1 );

  Serial.end();

}

float Logger::AtlasScientific__setLED(bool on, bool use_UART, bool use_software_serial){
  if (use_UART){
    if(on){
      if (use_software_serial){
        
        Serial.print("\r");
      }
      else{
        Serial.print("L,1");
        Serial.print("\r");
      }
    }
  }
  else if 

}

