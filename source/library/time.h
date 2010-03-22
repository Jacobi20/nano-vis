/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#pragma once

/*
const char MONTH[12][4]		= {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char DAYOFWEEK[7][4]	= {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

class ETime	
	{
	private:
		word	year;
		word	month;
		word	day_of_week;
		word	day;
		
		word	hour;
		word	min;
		word	sec;
		word	msec;

	public:		
		ETime	( void ) {}
		~ETime	( void ) {}
		
		//void	SetTimeDate	( const sys
		
			
	#define _DATETOSTR(s){ static char temp[256]; s; return temp; }
		const char	*TimeToStr		( void ) _DATETOSTR(sprintf(temp, "%02d:%02d:%02d", hour, min, sec));
		const char	*DateToStr		( void ) _DATETOSTR(sprintf(temp, "%02d/%02d/%04d", day, month, year));
		const char	*FullDateToStr	( void ) _DATETOSTR(sprintf(temp, "%s %d %s %d", DAYOFWEEK[day_of_week], day, MONTH[month-1], year));
		const char	*TimeDateToStr	( void ) _DATETOSTR(sprintf(temp, "%02d:%02d:%02d %02d/%02d/%04d", hour, min, sec, day, month, year)); 
	#undef _DATETOSTR
	
		void		FromMsec		( uint msec )
		{
			msec	=	(word)msec%1000;	msec/=1000;
			sec		=	(word)msec%60;		msec/=60;
			min		=	(word)msec%60;		msec/=60;
			hour	=	(word)msec%24;		msec/=24;
			day	= day_of_week = month = year = 0;
		}
	};
*/