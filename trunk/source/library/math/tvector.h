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

#include "../library.h"


#define INLINE	__forceinline

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

template <int dim=4, typename Type=float>
class	EVec
	{
	private:
		typedef	EVec<dim,Type>	vec_t;
		Type	v[dim];
		
	public:
					EVec			( void );
					EVec			( float *v );
									
		Type		Length			( void )	const;					//	get length of this vector
		Type		LengthSqr		( void )	const;					//	get square of length of this vector
		void		NormalizeSelf	( void );							//	normalize this vector
		vec_t		Normalize		( void )	const;					//	return normalized vector
		int			GetNearestAxis	( void )	const;					//	return index of nearest axix
		
		void		Lerp			( vec_t &a, vec_t &b, Type time );	//	snap this vector to grid
		void		Snap			( vec_t &grid );					//	lerp this vector (a,b,t)
		
		bool		Compare			( vec_t &a, float eps=0 ) const;	//	compare with epcilon
		bool		Compare			( vec_t &a ) const;					//	exact compare
		
		vec_t			operator+=		( vec_t &a );
		vec_t			operator-=		( vec_t &a );
		vec_t			operator*=		( vec_t &a );
		vec_t			operator|=		( vec_t &a );
		vec_t			operator^=		( vec_t &a );
		vec_t			operator*=		( Type s );
		
		vec_t			operator-		( void ) const;				//	unary minus
		friend vec_t	operator+		( vec_t &a, vec_t &b );		//	addition
		friend vec_t	operator-		( vec_t &a, vec_t &b );		//	subtraction
		friend vec_t	operator*		( vec_t &a, vec_t &b );		//	component multiplication
		friend Type		operator|		( vec_t &a, vec_t &b );		//	dot product
		friend vec_t	operator^		( vec_t &a, vec_t &b );		//	vector product
		friend vec_t	operator*		( vec_t &a, Type s );		//	scale
		friend vec_t	operator*		( Type s, vec_t &a );		//	scale
		
		friend bool		operator==		( vec_t &a, vec_t &b );		//	exact
		friend bool		operator!=		( vec_t &a, vec_t &b );		//	exact
		
	};
	
	
//
//	EVec::EVec
//
template <int dim, typename Type>
EVec<dim,Type>::EVec(void)
{
	memset(v, 0, sizeof(Type)*dim);
}


//
//	EVec::EVec
//
template <int dim, typename Type>
EVec<dim,Type>::EVec(Type *v)
{
	for (int i=0; i<dim; i++)
		this->v[i] = v[i];
}


//
//	EVec::Length
//
template <int dim, typename Type>
Type EVec<dim,Type>::Length(void) const
{
	Type sum = 0;
	for (i=0; i<dim; i++) sum+=(v[i]*v[i]);
	return sqrt(sum);
}
	
	
//
//	EVec::LengthSqr
//
template <int dim, typename Type>
Type EVec<dim,Type>::Length(void) const
{
	Type sum = 0;
	for (i=0; i<dim; i++) sum+=(v[i]*v[i]);
	return sum;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
