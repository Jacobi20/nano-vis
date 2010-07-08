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
	3d-dimensional container for various stuff :
	
	indexing:
		[min..max[
		minx = -2, maxx = 3 -> [-2][-1][ 0][ 1][ 2]
-----------------------------------------------------------------------------*/

template <class T>	class array3d {
	public:
		array3d	( void ) {
			minx = maxx = 0;
			miny = maxy = 0;
			minz = maxz = 0;
			update_sizes();
		}
	
		void set_ranges(int minx, int maxx, int miny, int maxy, int minz, int maxz, const T &value) 	{
			this->minx = minx;
			this->miny = miny;
			this->minz = minz;
			
			this->maxx = maxx;
			this->maxy = maxy;
			this->maxz = maxz;

			update_sizes();
			
			buffer.clear();
			buffer.resize( szx * szy * szz, value );
		}
		
		T at(int idx, int idy, int idz) const {
			bool in_range;
			return buffer[ index(idx, idy, idz) ];
		}
		
		T &at(int idx, int idy, int idz) {
			bool in_range;
			return buffer[ index(idx, idy, idz) ];
		}
		
	protected:
		void	update_sizes(void) {
			szx = maxx - minx;
			szy = maxy - miny;
			szz = maxz - minz;
		}
		
		uint index(int idx, int idy, int idz) {

			if (  idx < minx || idx >=maxx
			   || idy < miny || idy >=maxy
			   || idz < minz || idz >=maxz ) {
				throw exception("out of range");
			}
			
			idx -= minx;
			idy -= miny;
			idz -= minz;
		
			return	+ idx
					+ idy * szx
					+ idz * szx	* szy;
		}
	
		int szx, szy, szz;
		int minx, maxx;
		int miny, maxy;
		int minz, maxz;
		vector<T> buffer;
	};
