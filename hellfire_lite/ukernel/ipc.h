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
file: ipc.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 08/2011
last revision: 08/2011
function: ipc typedefs
=====================================================================
interprocess communication typedefs
=====================================================================
*/

typedef struct {
	void *msg;
	uint8_t n_waiting_tasks;
	uint8_t count;
	semaphore msend;
	semaphore mrecv;
} mailbox;

