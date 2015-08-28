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

#include <Arduino.h>

#ifndef _ALTOS_COMPANION_H_
#define _ALTOS_COMPANION_H_

class AltOSCompanion {
  public:
    static void configure(unsigned int board_id, byte update_period /* n * 100Hz samples */, byte channels);
    static void updateChannel(byte channel, unsigned int value);
    static unsigned int serialNumber();
    static unsigned int flightNumber();
    static unsigned int lastTick();
    static bool isConnected();
};

#endif
