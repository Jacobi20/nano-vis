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


class  EImage {
	//private:
       // EVec4 const_ref_vec4 = EVec4(0,0,0,1);
    public:
				EImage	( void );
				EImage	( uint w, uint h );
				~EImage	( void );

        //static 
		// Image creation stuff :				

		void	CreateImage	( uint w, uint h, const EVec4& fill_color =( EVec4(0,0,0,1)) );

		uint	Width		( void ) const { return width; }
		uint	Height		( void ) const { return height; }

		//	Image data access stuff :		
		uint	OffsetC		( int x, int y ) const;
		uint	OffsetW		( int x, int y ) const;
		EVec4	ReadC		( int x, int y ) const;
		EVec4	ReadW		( int x, int y ) const;
		void	WriteC		( int x, int y, const EVec4 &clr );
		void	WriteW		( int x, int y, const EVec4 &clr );
		EVec4	ReadLinearC	( float x, float y ) const;
		
	protected:
		uint width;
		uint height;
		vector<EVec4>	image_data;
	};


