/*
    This file is part of HellFire.

    HellFire is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HellFire is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HellFire.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
=====================================================================
file: mpsoc.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 12/2008
last revision: 01/2014
function: mpsoc definitions
=====================================================================
core naming / mapping to network nodes / driver configuration
=====================================================================
*/


#ifdef NOC_INTERCONNECT				// NoC core mapping and OS configuration
#define HF_PACKET_SLOTS		32		// powers of two! this is in number of packets
#define HF_PACKET_HDR_SIZE	8		// this is in flits
//#define NOC_CRC16
//#define DEBUG_FULL

#define NOC_COLUMN(core_n)	((core_n) % NOC_WIDTH)
#define NOC_LINE(core_n)	((core_n) / NOC_WIDTH)

typedef struct{
	volatile uint16_t data[HF_PACKET_SLOTS * PACKET_SIZE];
	volatile uint16_t start, end;
} packet_in_buffer_type;

typedef struct{
	volatile uint16_t data[PACKET_SIZE];
} packet_out_buffer_type;

#endif

