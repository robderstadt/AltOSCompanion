/*
 *   AltusMetrum flight computer companion board library for Arduino.
 *   Copyright (C) 2015  Rob Derstadt
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AltOSCompanion.h"

typedef struct  AltOSMessage {
   byte command;
   byte flight_state;
   unsigned int tick;
   unsigned int serial;
   unsigned int flight;
} AltOSMessage;

typedef struct AltOSSetupResponse {
  unsigned int board_id;
  unsigned int board_id_inverse;
  byte update_period;
  byte channels;
} AltOSSetupResponse;

AltOSSetupResponse setup_response;

volatile AltOSMessage message;
volatile byte bytes_read;
volatile byte setup_response_bytes_sent;
volatile byte fetch_response_bytes_sent;
volatile byte channel_buffer[32];
byte total_channel_bytes;

void AltOSCompanion::configure(unsigned int board_id, byte update_period, byte channels) {

  // Setup slave
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);

  bytes_read = 0;
  setup_response_bytes_sent = 0;
  fetch_response_bytes_sent = 0;

  setup_response.board_id = board_id;
  setup_response.board_id_inverse = ~setup_response.board_id;
  setup_response.update_period = update_period; // n * 100Hz ticks for FETCH
  setup_response.channels = channels;
  total_channel_bytes = channels * 2;
}

void AltOSCompanion::updateChannel(byte channel, unsigned int value) {
  if (digitalRead(10) == HIGH && channel < setup_response.channels) {
      *((byte*)&channel_buffer + (channel * 2)) = value;
   }
}

unsigned int AltOSCompanion::serialNumber() {
  return message.serial;
}

unsigned int AltOSCompanion::flightNumber() {
  return message.flight;
}

unsigned int AltOSCompanion::lastTick() {
  return message.tick;
}

bool AltOSCompanion::isConnected() {
  return (message.command > 1);
}


// See http://altusmetrum.org/AltOS/doc/companion.html 
ISR (SPI_STC_vect) {
  
  byte c = SPDR;  
  
  if (bytes_read < 8 && bytes_read >= 0) {
    *((byte*)&message + bytes_read) = c; 
  } 

  bytes_read++; 
  
  if(message.command == 1 && bytes_read >= 16) {
    SPDR = *((byte*) &setup_response + setup_response_bytes_sent++);
   if (setup_response_bytes_sent == 6) {
      setup_response_bytes_sent = 0;
      bytes_read = -1;
    }
  } else if (message.command == 2 && bytes_read >= 16) {
      SPDR = *((byte*)(&channel_buffer[0] + fetch_response_bytes_sent++)); 
      if (fetch_response_bytes_sent == total_channel_bytes) {
        fetch_response_bytes_sent = 0;
        bytes_read = -1;
      }
  } else if (message.command == 3 && bytes_read >= 16) {
      bytes_read = -1;  
      SPDR = 0x00;
  } else {
    SPDR = 0x00;
  }
 }
  
