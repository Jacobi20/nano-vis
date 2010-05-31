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

/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

#if 0
class	ISciObject;
class	ISciSpace;
class	ISciView;

typedef hard_ref<ISciObject>	IPxSciObject;
typedef hard_ref<ISciSpace>		IPxSciSpace;
typedef hard_ref<ISciView>		IPxSciView;


const uint	SCI_VIS_NORMAL		=	0x00000000;


ISciObject *(*ESciObjectCreator_f)(void);


class	ISciObject : public IDisposable {
	public:
		virtual void		Visualize		( const EMatrix4 &view, const EMatrix4 &proj, uint flags ) = 0;
		virtual void		SetPose			( const EVec4 &pos, const EQuat &orient ) = 0;
		virtual void		GetPose			( EVec4 &pos, EQuat &orient ) const = 0;
	};
	
	
class	ISciSpace : public IDisposable {
	public:
		virtual void		Visualize		( IPxSciVis vis, uint flags ) = 0;
		virtual void		AddObject		( IPxSciObject object ) = 0;
		virtual void		RemoveObject	( IPxSciObject object ) = 0;
	};
	
	
class	ISciView : public IDisposable {
	public:
		virtual EMatrix4	GetViewMatrix	( void ) const = 0;
		virtual EMatrix4	GetProjMatrix	( void ) const = 0;
	};	
#endif 