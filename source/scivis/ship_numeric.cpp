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

#include "sci_local.h"
#include "ship.h"
 
/*-----------------------------------------------------------------------------
	Ship numeric simulation :
-----------------------------------------------------------------------------*/

#if 0
//
//	EShip::UpdateNumeric
//
//	Equation :
//	M * zeta''  + N1 * zeta'  + lamda * S * zeta = 0	//	Hanovich	(formula 41)
//	M * zeta_d'	+ N1 * zeta_d + lamba * S * zeta = 0
//
//	zeta_d'		=	- 1 / M  *  ( lamda * S * zeta  +  N1 * zeta_d )
//	zeta'		=	
//
class expr_zeta_dd {
	public:
		float	N1;
		float	M;
		float	lamda;
		float	S;
		float operator () ( float zeta, float zeta_d, float zeta_w ) {
			//LOGF("Fnum = %g (KN)", (S * zeta * 1000 * GRAVITY) / 1000.0);
			return - ( GRAVITY * lamda * S * (zeta - zeta_w)  +  N1 * zeta_d ) / M;	
		}
	};
	

class expr_roll_dd {
	public:
		float	N2;
		float	M;
		float	MCH;
		float	I;
		float operator () ( float roll, float roll_d, float roll_w ) {
			return - ( N2 * roll_d * abs(roll_d) + M * GRAVITY * MCH * (roll + roll_w) ) / I; 
		}
	};


template<typename Func> void Euler2( Func func, float dt, float &x, float &dx, float a )
{
	float next_dx	=	dx + dt * ( func (x, dx, a) );
	float next_x	=	x  + dt * dx;
	x	=	next_x;
	dx	=	next_dx;
}


template<typename Func> void RungeKutta2( Func func, float dt, float &x, float &dx, float a )
{
	float k1	=	func( x,			dx,					a );
	float k2	=	func( x + 0.5*dt,	dx + 0.5*dt * k1,	a );
	float k3	=	func( x + 0.5*dt,	dx + 0.5*dt * k2,	a );
	float k4	=	func( x + dt,		dx + dt * k2,		a );

	float next_dx	=	dx + dt/6 * (k1 + 2*k2 + 2*k3 + k4);
	float next_x	=	x  + dt * dx;
	x	=	next_x;
	dx	=	next_dx;
}


void EShip::EulerStep( float dt, IPxWaving waving )
{
	const float N1		= 0*(0.1 * ship_length	* ship_width);	//	Hanovich (formula 56)
	const float N2		= (0.086 * ship_length * (ship_width * ship_width * ship_width * ship_width));	// devided by 'roll_d'
	const float M		= (ship_mass);
	const float S		= (ship_length * ship_width);
	const float lamda	= (0.7	* WATER_DENSITY);
	const float I		= ship_param.Ix;

	float zeta_w = waving->GetPosition  ( EVec4(0,0,0,1) ).z;
	float delta  = waving->GetWaveSlopeX ( EVec4(0,0,0,1) );
	
	expr_zeta_dd	_expr_zeta_dd;
	_expr_zeta_dd.lamda	=	lamda;
	_expr_zeta_dd.N1	=	N1;
	_expr_zeta_dd.M		=	M;
	_expr_zeta_dd.S		=	S;
	
	expr_roll_dd	_expr_roll_dd;
	_expr_roll_dd.N2	=	N2;
	_expr_roll_dd.M		=	M;
	_expr_roll_dd.MCH	=	ship_param.gmt;
	_expr_roll_dd.I		=	I;

	Euler2( _expr_zeta_dd, dt, num_zeta, num_zeta_d, zeta_w );
	Euler2( _expr_roll_dd, dt, num_roll, num_roll_d, delta );

	//RungeKutta2	( _expr_zeta_dd, dt, num_zeta, num_zeta_d, zeta_w );
	//RungeKutta2	( _expr_roll_dd, dt, num_roll, num_roll_d, delta );
}


void EShip::UpdateNumeric( float dtime, IPxWaving waving )
{
	for (uint i=0; i<30; i++) {
		EulerStep( dtime / 30.0f, waving );
	}
}
#endif