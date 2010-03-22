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



class	EMatrix3
	{
	public:
		float	M[3][3];
		
						EMatrix3		( void );
						EMatrix3		( const EMatrix3 &m );
						EMatrix3		(	float a00, float a01, float a02, 
											float a10, float a11, float a12,
											float a20, float a21, float a22  );

		float			&operator	()	( uint row, uint col );
		float			operator	()	( uint row, uint col ) const;

		float			*Ptr			( void );
		const	float	*Ptr			( void ) const;
		
		EMatrix3		&operator	+=	( const EMatrix3 &A );
		EMatrix3		&operator	-=	( const EMatrix3 &A );
		EMatrix3		&operator	*=	( const EMatrix3 &A );
		
		EMatrix3		&operator	*=	( float a );
		EMatrix3		&operator	/=	( float a );
		
		EMatrix3		operator	+	( void ) const;
		EMatrix3		operator	-	( void ) const;

		friend	EMatrix3	operator	*	( const EMatrix3 &A, const EMatrix3 &B );	
		friend	EMatrix3	operator	+	( const EMatrix3 &A, const EMatrix3 &B );	
		friend	EMatrix3	operator	-	( const EMatrix3 &A, const EMatrix3 &B );	
		
		friend	EMatrix3	operator	*	( const EMatrix3 &A, float s );	
		friend	EMatrix3	operator	*	( float s, const EMatrix3 &A );	
		friend	EMatrix3	operator	/	( const EMatrix3 &A, float s );	
	};


EMatrix3	Matrix3Identity		( void );
float		Matrix3Det			( const EMatrix3 &A );
float		Matrix3Minor		( const EMatrix3 &A, uint row, uint col );
EMatrix3	Matrix3Inverse		( const EMatrix3 &A );
EMatrix3	Matrix3Transpose	( const EMatrix3 &A );
EVec3		Matrix3Transform	( const EVec3 &v, const EMatrix3 &T );


inline EMatrix3::EMatrix3(void) 
{
	M[0][0] = 1;	M[0][1] = 0;	M[0][2] = 0;
	M[1][0] = 0;	M[1][1] = 1;	M[1][2] = 0;
	M[2][0] = 0;	M[2][1] = 0;	M[2][2] = 1;
}

inline EMatrix3::EMatrix3(const EMatrix3 &m) 
{
	*this = m;
}

inline EMatrix3::EMatrix3(	float a00, float a01, float a02, 
							float a10, float a11, float a12,
							float a20, float a21, float a22  ) 
{
	M[0][0] = a00;	M[0][1] = a01;	M[0][2] = a02;
	M[1][0] = a10;	M[1][1] = a11;	M[1][2] = a12;
	M[2][0] = a20;	M[2][1] = a21;	M[2][2] = a22;
}


inline float *EMatrix3::Ptr(void)
{
	return &M[0][0];
}


inline const float *EMatrix3::Ptr(void) const
{
	return &M[0][0];
}


inline float EMatrix3::operator ()(uint row, uint col) const
{
	return M[row][col];
}


inline float &EMatrix3::operator ()(uint row, uint col)
{
	return M[row][col];
}


inline EMatrix3 &EMatrix3::operator +=	( const EMatrix3 &A )
{
	for (int i=0; i<9; i++)	Ptr()[i] += A.Ptr()[i];
	return *this;
}

inline EMatrix3 &EMatrix3::operator -=	( const EMatrix3 &A )
{
	for (int i=0; i<9; i++)	Ptr()[i] -= A.Ptr()[i];
	return *this;
}


inline EMatrix3 &EMatrix3::operator *=	( const EMatrix3 &A )
{
	EMatrix3 temp = *this;
	
	for (register int i=0; i<3; i++)
	for (register int j=0; j<3; j++)
	{
		temp.M[i][j] = 
			M[i][0] * A.M[0][j] +
			M[i][1] * A.M[1][j] +
			M[i][2] * A.M[2][j];
	}
	
	*this	=	temp;
	
	return *this;
}


inline EMatrix3 &EMatrix3::operator *=	( float s )
{
	for (int i=0; i<9; i++)	Ptr()[i] *= s;
	return *this;
}


inline EMatrix3 &EMatrix3::operator /=	( float s )
{
	for (int i=0; i<9; i++)	Ptr()[i] /= s;
	return *this;
}


inline EMatrix3 EMatrix3::operator - ( void ) const
{
	return 	EMatrix3(
		-M[0][0],	-M[0][1],	-M[0][2],	
		-M[1][0],	-M[1][1],	-M[1][2],	
		-M[2][0],	-M[2][1],	-M[2][2]);
}



inline EMatrix3 operator + ( const EMatrix3 &A, const EMatrix3 &B )
{
	EMatrix3 temp(A);
	temp += B;
	return temp;
}

inline EMatrix3 operator - ( const EMatrix3 &A, const EMatrix3 &B )
{
	EMatrix3 temp(A);
	temp -= B;
	return temp;
}

inline EMatrix3 operator * ( const EMatrix3 &A, const EMatrix3 &B )
{
	EMatrix3 temp(A);
	temp *=	B;
	return temp;
}

inline EMatrix3 operator * ( const EMatrix3 &A, float s )
{
	EMatrix3 temp(A);
	temp *= s;
	return temp;
}

inline EMatrix3 operator * ( float s, const EMatrix3 &A )
{
	EMatrix3 temp(A);
	temp *= s;
	return temp;
}

inline EMatrix3 operator / ( const EMatrix3 &A, float s )
{
	EMatrix3 temp(A);
	temp *= s;
	return temp;
}



/*-----------------------------------------------------------------------------
	MATRIX 4x4
-----------------------------------------------------------------------------*/

class	EMatrix4	
	{
	public:
		float M[4][4];
		
						EMatrix4		( void );
						EMatrix4		( const EMatrix4 & m);
						EMatrix4		(	float a00, float a01, float a02, float a03, 
											float a10, float a11, float a12, float a13,
											float a20, float a21, float a22, float a23,
											float a30, float a31, float a32, float a33 );

		float			&operator	()	( uint row, uint col );
		float			operator	()	( uint row, uint col ) const;

		float			*Ptr			( void );
		const	float	*Ptr			( void ) const;
		
		EMatrix4		&operator	+=	( const EMatrix4 &A );
		EMatrix4		&operator	-=	( const EMatrix4 &A );
		EMatrix4		&operator	*=	( const EMatrix4 &A );
		
		EMatrix4		&operator	*=	( float a );
		EMatrix4		&operator	/=	( float a );
		
		EMatrix4		operator	+	( void ) const;
		EMatrix4		operator	-	( void ) const;

		friend	EMatrix4	operator	*	( const EMatrix4 &A, const EMatrix4 &B );	
		friend	EMatrix4	operator	+	( const EMatrix4 &A, const EMatrix4 &B );	
		friend	EMatrix4	operator	-	( const EMatrix4 &A, const EMatrix4 &B );	
		
		friend	EMatrix4	operator	*	( const EMatrix4 &A, float s );	
		friend	EMatrix4	operator	*	( float s, const EMatrix4 &A );	
		friend	EMatrix4	operator	/	( const EMatrix4 &A, float s );	
		
		friend bool			operator	==	( const EMatrix4 &A, const EMatrix4 &B );	
		friend bool			operator	!=	( const EMatrix4 &A, const EMatrix4 &B );	
	};



EMatrix4	Matrix4Identity			( void );
float		Matrix4Det				( const EMatrix4 &A );
float		Matrix4Minor			( const EMatrix4 &A, uint row, uint col );
EMatrix4	Matrix4Inverse			( const EMatrix4 &A );
EMatrix4	Matrix4Transpose		( const EMatrix4 &A );
EVec4		Matrix4Transform		( const EVec4 &v, const EMatrix4 &T );
void		Matrix4ToAngles			( const EMatrix4 &A, float &yaw, float &pitch, float &roll );

EMatrix4	Matrix4Translate		( float x, float y, float z );
EMatrix4	Matrix4Translate		( const EVec4 &dir );
EMatrix4	Matrix4Scale			( float x, float y, float z );
EMatrix4	Matrix4RotateX			( float a );
EMatrix4	Matrix4RotateY			( float a );
EMatrix4	Matrix4RotateZ			( float a );
EMatrix4	Matrix4PerspectiveRH	( float w, float h, float zn, float zf );
EMatrix4	Matrix4PerspectiveLH	( float w, float h, float zn, float zf );
EMatrix4	Matrix4PerspectiveFovRH	( float fovy, float aspect, float zn, float zf );
EMatrix4	Matrix4PerspectiveFovLH	( float fovy, float aspect, float zn, float zf );
EMatrix4	Matrix4OrthoRH			( float w, float h, float zn, float zf );
EMatrix4	Matrix4OrthoLH			( float w, float h, float zn, float zf );


inline EMatrix4::EMatrix4(void) 
{
	M[0][0] = 1;	M[0][1] = 0;	M[0][2] = 0;	M[0][3] = 0;
	M[1][0] = 0;	M[1][1] = 1;	M[1][2] = 0;	M[1][3] = 0;
	M[2][0] = 0;	M[2][1] = 0;	M[2][2] = 1;	M[2][3] = 0;
	M[3][0] = 0;	M[3][1] = 0;	M[3][2] = 0;	M[3][3] = 1;
}

inline EMatrix4::EMatrix4(const EMatrix4 &m) 
{
	*this = m;
}

inline EMatrix4::EMatrix4(	float a00, float a01, float a02, float a03, 
							float a10, float a11, float a12, float a13,
							float a20, float a21, float a22, float a23,
							float a30, float a31, float a32, float a33 ) {
	M[0][0] = a00;	M[0][1] = a01;	M[0][2] = a02,	M[0][3] = a03;
	M[1][0] = a10;	M[1][1] = a11;	M[1][2] = a12,	M[1][3] = a13;
	M[2][0] = a20;	M[2][1] = a21;	M[2][2] = a22,	M[2][3] = a23;
	M[3][0] = a30;	M[3][1] = a31;	M[3][2] = a32,	M[3][3] = a33;
}



inline float *EMatrix4::Ptr(void)
{
	return &M[0][0];
}


inline const float *EMatrix4::Ptr(void) const
{
	return &M[0][0];
}


inline float EMatrix4::operator ()(uint row, uint col) const
{
	return M[row][col];
}


inline float &EMatrix4::operator ()(uint row, uint col)
{
	return M[row][col];
}


inline EMatrix4 &EMatrix4::operator +=	( const EMatrix4 &A )
{
	for (int i=0; i<16; i++)	Ptr()[i] += A.Ptr()[i];
	return *this;
}

inline EMatrix4 &EMatrix4::operator -=	( const EMatrix4 &A )
{
	for (int i=0; i<16; i++)	Ptr()[i] -= A.Ptr()[i];
	return *this;
}


inline EMatrix4 &EMatrix4::operator *=	( const EMatrix4 &A )
{
	EMatrix4 temp = *this;
	
	for (register int i=0; i<4; i++)
	for (register int j=0; j<4; j++)
	{
		temp.M[i][j] = 
			M[i][0] * A.M[0][j] +
			M[i][1] * A.M[1][j] +
			M[i][2] * A.M[2][j] +
			M[i][3] * A.M[3][j];
	}
	
	*this	=	temp;
	
	return *this;
}


inline EMatrix4 &EMatrix4::operator *=	( float s )
{
	for (int i=0; i<16; i++)	Ptr()[i] *= s;
	return *this;
}


inline EMatrix4 &EMatrix4::operator /=	( float s )
{
	for (int i=0; i<16; i++)	Ptr()[i] /= s;
	return *this;
}


inline EMatrix4 EMatrix4::operator - ( void ) const
{
	return 	EMatrix4(
		-M[0][0],	-M[0][1],	-M[0][2],	-M[0][3],	
		-M[1][0],	-M[1][1],	-M[1][2],	-M[1][3],	
		-M[2][0],	-M[2][1],	-M[2][2],	-M[2][3],	
		-M[3][0],	-M[3][1],	-M[3][2],	-M[3][3]);
}



inline EMatrix4 operator + ( const EMatrix4 &A, const EMatrix4 &B )
{
	EMatrix4 temp(A);
	temp += B;
	return temp;
}

inline EMatrix4 operator - ( const EMatrix4 &A, const EMatrix4 &B )
{
	EMatrix4 temp(A);
	temp -= B;
	return temp;
}

inline EMatrix4 operator * ( const EMatrix4 &A, const EMatrix4 &B )
{
	EMatrix4 temp(A);
	temp *=	B;
	return temp;
}

inline EMatrix4 operator * ( const EMatrix4 &A, float s )
{
	EMatrix4 temp(A);
	temp *= s;
	return temp;
}

inline EMatrix4 operator * ( float s, const EMatrix4 &A )
{
	EMatrix4 temp(A);
	temp *= s;
	return temp;
}

inline EMatrix4 operator / ( const EMatrix4 &A, float s )
{
	EMatrix4 temp(A);
	temp *= s;
	return temp;
}

inline bool operator == ( const EMatrix4 &A, const EMatrix4 &B )
{
	if (memcmp(&A, &B, sizeof(EMatrix4))==0) {
		return true;
	} else {
		return false;
	}
}

inline bool operator != ( const EMatrix4 &A, const EMatrix4 &B )
{
	return !(A==B);
}
