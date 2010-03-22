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

class	ECplx
	{
	public:
		float	a, b;
		
					ECplx	( void );
					ECplx	( float a, float b = 0);
					ECplx	( const ECplx &C );
				
		ECplx		operator -	( void ) const;					// unary minus		-A
		ECplx		operator +	( const ECplx &B ) const;		// addition			A+B
		ECplx		operator -	( const ECplx &B ) const;		// subtraction		A-B
		ECplx		operator *	( const ECplx &B ) const;		// multiplication	A*B
		ECplx		operator /	( const ECplx &B ) const;		// division			A/B
				
		ECplx		&operator+=	( const ECplx &B ) { *this = ((*this) + B); return *this; }
		ECplx		&operator-=	( const ECplx &B ) { *this = ((*this) - B); return *this; }
		ECplx		&operator*=	( const ECplx &B ) { *this = ((*this) * B); return *this; }
		ECplx		&operator/=	( const ECplx &B ) { *this = ((*this) / B); return *this; }
				
		bool		operator==	( const ECplx &B ) const;
		bool		operator!=	( const ECplx &B ) const;

		ECplx		Conjugate	( void ) const;
		float		Abs			( void ) const;
	};
	

inline ECplx::ECplx( void ) {
	a = b = 0.0;
}

inline ECplx::ECplx( float a, float b ) {
	this->a = a;
	this->b = b;
}

inline ECplx::ECplx( const ECplx &C ) {
	this->a = C.a;
	this->b = C.b;
}


inline ECplx	ECplx::operator - ( void ) const {
	return ECplx ( -a, -b );
}

inline ECplx	ECplx::operator + ( const ECplx &B ) const {
	return ECplx( a + B.a, b + B.b );
}

inline ECplx	ECplx::operator - ( const ECplx &B ) const {
	return ECplx( a - B.a, b - B.b );
}

inline ECplx	ECplx::operator * ( const ECplx &B ) const {
	float _a = a*B.a - b*B.b;
	float _b = b*B.a + a*B.b;
	return ECplx( _a, _b );
}

inline ECplx	ECplx::operator / ( const ECplx &B ) const {
	ECplx	C = ((*this) * B.Conjugate());
	float	d = ( B.a*B.a + B.b*B.b );
	ASSERT( d );
	return ECplx( C.a/d, C.b/d );
}

inline bool		ECplx::operator == ( const ECplx &B ) const {
	return ( a==B.a && b==B.b );
}

inline bool		ECplx::operator != ( const ECplx &B ) const {
	return !(*this == B);
}



inline ECplx	ECplx::Conjugate(void) const {
	return ECplx( a, -b );
}

inline float	ECplx::Abs(void) const {
	return sqrt( a*a + b*b );
}

