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
file: stdint.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 03/2014
function: type definitions
=====================================================================
just that.
=====================================================================
*/

/* C type extensions */
#ifndef _STDINT
#define _STDINT
typedef unsigned char		uint8_t;
typedef char			int8_t;
typedef unsigned short int	uint16_t;
typedef short int		int16_t;
typedef unsigned long		uint32_t;
typedef long			int32_t;
typedef unsigned long long	uint64_t;
typedef long long		int64_t;
typedef void			(*HF_funcptr)(void *arg);

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif

