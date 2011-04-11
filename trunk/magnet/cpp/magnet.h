/*
	The MIT License

	Copyright (c) 2011 Escience Research Institute

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

/*-----------------------------------------------------------------------------
	MAGNET
-----------------------------------------------------------------------------*/

class MagnetGame : public IGame, public IFRVolume::DataProvider {
	public:
						MagnetGame	( void );
						~MagnetGame	( void );
		virtual void	Frame		( uint dtime );

		// IFRVolume::DataProvider 
		virtual float Value( const EPoint &local_point ) const;
		
	protected:
		uint time;
	
		void			SetupCamera		( void );
		void			UpdateMagnets	( void );
		
		IPxFRLight		light;
	
		IPxFREntity		base_rend;
		IPxPhysEntity	base_phys;
		IPxFRVolume		volume;

		struct view_t {		
			view_t () : yaw(0), pitch(0), roll(0) {}
			EPoint	center;
			float	yaw, pitch, roll;
		} view;
		
		class Magnet {
		public:
			IPxFREntity		rend_obj;
			IPxPhysEntity	phys_obj;
			EPoint			pp;
			EPoint			pn;
		};
		
		vector<Magnet>	magnets;
	};