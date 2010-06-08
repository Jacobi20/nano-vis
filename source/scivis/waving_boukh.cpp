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

/*-----------------------------------------------------------------------------
	Waving :
-----------------------------------------------------------------------------*/
const uint	NN			=	100;
const float GRAVITY		=	9.81f;

class EWavingBoukh : public IWaving {
	public:
							EWavingBoukh	( lua_State *L, int idx );
							~EWavingBoukh	( void );
		
		virtual	EVec4		GetVelocity		( const EVec4 &init_pos, float time ) const;
		virtual	EVec4		GetPosition		( const EVec4 &init_pos, float time ) const;
		
	protected:
		void				InitGenerator	( void );
	
		float	h3		;	// --- высота волный 3%-ой обеспеченности, м --- 	 			
		float	Wmax	;	/*2.0f*/ // == Максимальная частота ==
		float	BSp		;	/*1.3f*/  // == Параметр формы спектра ==
		float	ASp		;	/*10*/ // == Параметр масштаба спектра ==

		float	Wmin	;	/*0.2f*/	//	Минимальная частота
		float	W0		;	/*1.2f*/	//	Средняя частота волны, рад/c
		float	A_2D	[NN];
		float	Fi_2D	[NN];
		float	W		[NN];
	
	};

IWaving	*create_boukh_waving(lua_State *L, int idx) { return new EWavingBoukh(L, idx); }

/*-----------------------------------------------------------------------------
	Implementation :
-----------------------------------------------------------------------------*/


EWavingBoukh::EWavingBoukh( lua_State *L, int idx )
{
	Wmax	=	 5.1f	;
	W0		=	 3.51f	;
	Wmin	=	 0.5f	;
	BSp		=	 0.030f	;
	ASp		=	 0.233f * 3	;

	h3		=	5		;
	InitGenerator();
}


void EWavingBoukh::InitGenerator( void )
{
	float dW = (Wmax-Wmin)/(float)NN;
	
	for (int i=0; i<NN; i++) { 
		W[i]=Wmin+i*dW;
	}

	for(int i=0; i<NN; i++) {
		float w	 = W[i];
		A_2D[i]	 = (float)sqrt(2*ASp*(1.0f/(w*w*w*w*w))*exp(-BSp/(w*w*w*w))*dW);
		Fi_2D[i] = (float)FRand()*2.0f*PI ;//rand() - 0..1
	}
}


EWavingBoukh::~EWavingBoukh( void )
{

}


EVec4 EWavingBoukh::GetPosition( const EVec4 &init_pos, float time ) const
{
	float sum_sin = 0.0;
	float sum_cos = 0.0;
	
	float dW = (Wmax-Wmin)/(float)NN;
	
	float x	= init_pos.x;
	float t = time;

	for(int i=0; i<NN; i++) {
		float W	=	Wmin + i * dW;
		float K	=	W * W / GRAVITY;
		float s, c;
		
		sincos(K*x - W*t + Fi_2D[i], s, c);
		sum_sin += A_2D[i] * s;		
		sum_cos += A_2D[i] * c;		
	}
	return EVec4(sum_sin, 0, sum_sin, 1);
}


EVec4 EWavingBoukh::GetVelocity( const EVec4 &init_pos, float time ) const
{
	float dt = 0.00390625;	//	1 / 256
	EVec4 p1 = GetPosition(init_pos, time);
	EVec4 p2 = GetPosition(init_pos, time + dt);
	return (p2 - p1) / dt;
}


