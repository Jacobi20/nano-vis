/***************************************************************************
                         Matrix.h  -  description
			 Classes: Vector, Matrix for sequential calculations
                             -------------------
    begin                :  9.2003
    copyright          : (C) 2003 by  Sergei V. Ivanov, freeware
    email                : zarva@hotbox.ru
 ***************************************************************************/
#ifndef MATRIX_H
#define	MATRIX_H

typedef 	float* 	pfloat;

#include 		"generator.h"
#include 		<stdio.h>
#include 		<math.h>
#include 		<string.h>



class Matrix
{//Matrix
public:
	class Vector
	{//Vector
		public:
			//Useful data
			pfloat v1;
			int size;

		public:
			//additional information
			int IS_ROW;

			//*******************constructors*******************
			Vector()
			{//default
				size=0;
				IS_ROW=0;
			}//efault

			Vector(int d1)
			{//Vector
				size=d1;
				v1=new float[size];
				IS_ROW=0;
			}//Vector

			Vector(const Vector& V)
			{//Vector
					size=V.size;
					v1=new float[size];
					memcpy(v1,V.v1,size*sizeof(float));
					IS_ROW=0;
			}//Vector
			//*******************constructors*******************

			void init(pfloat& v2)//only after DEFAULT constructor(!!!): superlock=1
			{//init
				v1=v2;
			}//init

			void resize(int sz)
			{//resize
				Vector tmp(*this);//!!!
				if(size>0)		delete [] v1;//only after cpying data
				v1=new float[sz];
				if(size>sz) size=sz;//for decreasing vector
				memcpy(v1,tmp.v1,size*sizeof(float));
				size=sz;
			}//resize

			float& 	operator[](int index)
			{//operator[]
				return *(v1+index);
			}//operator[]

			float&	operator()(int index)
			{//operator()
				if((index<size)&&(index>=0))
				return *(v1+index);
				else	printf("Error: bad Vector index %d\n",index);
				return *v1;
			}//operator()

			int 	operator=(const Vector& V)
			{//operator=
				int ret_code=1;
				if(IS_ROW!=1)
				{//not row
					if(size>0)	delete [] v1;
					size=V.size;
					v1=new float[size];
					memcpy(v1,V.v1,size*sizeof(float));
				}//not row
				else
				{//for Vector row
					if(size==V.size)	memcpy(v1,V.v1,size*sizeof(float));
					else  ret_code=0;//not succeful
				}//for Vector row
				return ret_code;//succeful
			}//operator=

			void 	operator+=(const Vector& V)
			{//operator+=
				if(size==V.size)
					for(int i=0;i<size;i++) v1[i]=v1[i]+V.v1[i];//is  it fastest code?
			}//operator+=

			void 	operator-=(const Vector& V)
			{//operator-=
				if(size==V.size)
					for(int i=0;i<size;i++) v1[i]=v1[i]-V.v1[i];//is  it fastest code?
			}//operator-=

			void 	operator=(float val)
			{//operator=
					for(int i=0;i<size;i++) v1[i]=val;
			}//operator=

			float 	operator*(const Vector& V)//scalar multiplication
			{//operator*
				float rez=0;
				if(size==V.size)
					for(int i=0;i<size;i++) rez+=v1[i]*V.v1[i];
				return rez;
			}//operator*

			void	 operator*=(float f)
			{//operator*=
					for(int i=0;i<size;i++) v1[i]=v1[i]*f;
			}//operator*=

			void	 operator-=(float f)
			{//operator-=
					for(int i=0;i<size;i++) v1[i]=v1[i]-f;
			}//operator-=

			void	 operator+=(float f)
			{//operator+=
					for(int i=0;i<size;i++) v1[i]=v1[i]+f;
			}//operator+=

			Vector	 operator*(float f)
			{//operator*
					Vector v_tmp(size);
					for(int i=0;i<size;i++) v_tmp[i]=v1[i]*f;
					return v_tmp;
			}//operator*

			pfloat& 	getRef(){return v1;}

			~Vector()
			{//destructor
				if(IS_ROW==0)
						if(size>0)		delete [] v1;
			}//destructor

			void print()
			{//print
				for(int i=0;i<size;i++) printf("%f  ",v1[i]);
				printf("\n");
			}//print

			void print(char message[])
			{//print
				printf("\n");
				printf(message);
				printf(":   ");
				for(int i=0;i<size;i++) printf("%f  ",v1[i]);
				printf("\n");
			}//print

			float norma()//Evklid
			{// norma
				float rez=0.;
				for(int i=0;i<size;i++) rez+=v1[i]*v1[i];
				return sqrt(rez);
			}// norma

			float norma2()//Evklid^2
			{// norma
				float rez=0.;
				for(int i=0;i<size;i++) rez+=v1[i]*v1[i];
				return rez;
			}// norma

			void randomize()
			{//randomize()
				for(int i=0;i<size;i++) v1[i]=rnd();
			}//randomize()

			float mid()
			{//middle()
				float mid=0.;
				for(int i=0;i<size;i++) mid+=v1[i];
				return mid/size;
			}//middle()

			void randomize_gauss()
			{//randomize()
				for(int i=0;i<size;i++) v1[i]=gauss_val();
			}//randomize()

			void write_to_file(char filename[])
			{//write_to_file
				FILE* stream=fopen(filename,"w+");
				fprintf(stream,"%d\n",size);
				for(int i=0;i<size;i++)
						fprintf(stream,"   %f",v1[i]);
				fprintf(stream,"\n");
				fclose(stream);
			}//write_to_file

			void add_to_file(char filename[])
			{//write_to_file
				FILE* stream=fopen(filename,"a+");
				fprintf(stream,"%d\n",size);
				for(int i=0;i<size;i++)
						fprintf(stream,"   %f",v1[i]);
				fprintf(stream,"\n");
				fclose(stream);
			}//write_to_file

 			void load_from_file(char filename[])
			{//load_from_file
				if(size>0)		delete [] v1;
				FILE* stream=fopen(filename,"r");
				fscanf(stream,"%d",&size);
				v1=new float[size];
				for(int i=0;i<size;i++)
				fscanf(stream,"%f",&v1[i]);
				fclose(stream);
			}//load_from_file

			Vector sort()//0...size-1
			{//QuickSort
				Vector tmp(*this);
				int lo0=0;
				int hi0=size-1;
				QuickSort(tmp,lo0,hi0);
				return tmp;
			}//QuickSort

		private:

			int swap(int i, int j)
			{//swap
      				float	T = v1[i];
      				v1[i] = v1[j];
      				v1[j] = T;
				return 0;
			}//swap

			int QuickSort(Vector& a, int lo0, int hi0)
			{//QuickSort
	  			if ( hi0 != lo0)
      				{//if1
		 			int lo = lo0,hi = hi0;
					float mid=a[(lo0+hi0)/2];
         				while( lo <= hi )
         				{//while
	            				while((lo<hi0)&&(a[lo]<mid))	++lo;
        	    				while((hi>lo0)&&(a[hi]>mid))	--hi;
            					(lo<=hi)? a.swap(lo++, hi--):0;
	      				}//while
		 			(lo0<hi)?QuickSort(a,lo0,hi):0;
         				(lo<hi0)?QuickSort(a,lo,hi0):0;
	   			}//if1
				return 0;
			}//QuickSort

			float gauss_val()
			{//gauss_val()
				float sum=0.;
				for(int i=0;i<12;i++) sum+=rnd();
				return sum-6.;
			}//gauss_val()
	};//Vector

	public:
		pfloat m1;//data

		int d1,d2,size;

		//**********************CONSTRUCTORS**********************
		Matrix()//minim of matrix == scalar
		{//by default
			d1=1;
			d2=1;
			size=1;
			m1=new float[size];
			init_vecs();
		}//by default

		Matrix(int D1,int D2)
		{//constructor
			d1=D1;
			d2=D2;
			size=d1*d2;
			m1=new float[size];
			init_vecs();
		}//constructor
		//**********************CONSTRUCTORS**********************

		void resize(int D1,int D2)
		{//init
			delete  [] m1;
			d1=D1;
			d2=D2;
			size=d1*d2;
			m1= new float[size];//(!!!)
			re_init_vecs();//not tested
		}//init

		Matrix(const Matrix& M)
		{//constructor copy
			d1=M.d1;
			d2=M.d2;
			size=M.size;
			m1=new float[size];
			memcpy(m1,M.m1,size*sizeof(float));
			init_vecs();//not tested: only for constructors
		}//constructor copy

		void operator=(const Matrix& M)
		{//operator=
			delete [] m1;
			d1=M.d1;
			d2=M.d2;
			size=M.size;
			m1=new float[size];
			memcpy(m1,M.m1,size*sizeof(float));
			re_init_vecs();
		}//operator=

		Matrix divide(int cpu_rank/*0...cpu_sz-1*/,int cpu_sz/*from 1*/)//<<MPI_Rank, MPI_Size
		{//divide by IS_ROWs
			//must use memcopy for this function
			int m_sz=d1/cpu_sz;				//new Matrix size: round to less way
			int m_sz_add=d1%cpu_sz;			//for last indexes
			if(cpu_rank!=cpu_sz-1) m_sz_add=0;
  			Matrix m_tmp(m_sz+m_sz_add,d2);	//for last indexes
			int st_indx=cpu_rank*m_sz;			//start index
			for(int i=0;i<m_sz+m_sz_add;i++) 	 //for last indexes
				for(int j=0;j<d2;j++) m_tmp[i][j]=row[i+st_indx][j];
			return m_tmp;
		}//divide by IS_ROWs

		void load_square_from_file(char filename[])//square & float
		{//load_from_matrix
			delete [] m1;
			FILE* stream=fopen(filename,"r");
			int n=0;
			fscanf(stream,"%d",&n);
			d1=n;
			d2=n;
			size=d1*d2;
			m1=new float[size];
			for(int i=0;i<size;i++)
			fscanf(stream,"%f",&m1[i]);
			fclose(stream);
			re_init_vecs();
		}//load_from_matrix

		 void load_from_file(char filename[])//square & float
		{//load_from_file
			delete [] m1;
			FILE* stream=fopen(filename,"r");
			fscanf(stream,"%d",&d1);
			fscanf(stream,"%d",&d2);
			size=d1*d2;
			m1=new float[size];
			for(int i=0;i<size;i++)
			fscanf(stream,"%f",&m1[i]);
			fclose(stream);
			re_init_vecs();
		}//load_from_file

		void write_to_file(const char filename[], char label[])
		{//write_to_file
			FILE* stream=fopen(filename,"w+");
			fprintf(stream,"%s\n\n",label);
			fprintf(stream,"%d\n%d\n",d1,d2);
			for(int i=0;i<d1;i++,fprintf(stream,"\n"))
				for(int j=0;j<d2;j++)
					fprintf(stream,"   %f",row[i][j]);
			fprintf(stream,"\n");
			fclose(stream);
		}//write_to_file

		void write_to_file(const char filename[])
		{//write_to_file
			FILE* stream=fopen(filename,"w+");
			fprintf(stream,"%d\n%d\n",d1,d2);
			for(int i=0;i<d1;i++,fprintf(stream,"\n"))
				for(int j=0;j<d2;j++)
					fprintf(stream,"   %f",row[i][j]);
			fprintf(stream,"\n");
			fclose(stream);
		}//write_to_file

		void add_to_file(const char filename[], char label[])
		{//add_to_file
			FILE* stream=fopen(filename,"a+");
			fprintf(stream,"%s\n\n",label);
			fprintf(stream,"%d\n%d\n",d1,d2);
			for(int i=0;i<d1;i++,fprintf(stream,"\n"))
				for(int j=0;j<d2;j++)
					fprintf(stream,"   %f",row[i][j]);
			fprintf(stream,"\n");
			fclose(stream);
		}//aad_to_file

		void add_to_file(const char filename[])
		{//add_to_file
			FILE* stream=fopen(filename,"a+");
			fprintf(stream,"%d\n%d\n",d1,d2);
			for(int i=0;i<d1;i++,fprintf(stream,"\n"))
				for(int j=0;j<d2;j++)
					fprintf(stream,"   %f",row[i][j]);
			fprintf(stream,"\n");
			fclose(stream);
		}//aad_to_file

		void operator=(float val)
		{//operator=
			for(int i=0;i<size;i++) m1[i]=val;
		}//operator=

		Vector& operator[](int index)
		{//operator[]
			return row[index];
		}//operator[]

		Vector& operator()(int index)
		{//operator()
			if((index<d1)&&(index>=0))
			return row[index];
			else	printf("Error: Matrix index error\n");
			return row[0];
		}//operator()

		Vector operator*(Vector& V)//return Matrix x Vector1
		{//operator[]
			Vector R(d1);
			pfloat Vf=V.getRef();
			if(d2==V.size)
				for(int i=0;i<d1;i++)
				{//FOR
					pfloat m1f=m1+i*d2;
					float val=0;
					for(int j=0;j<d2;j++) val+=Vf[j]*m1f[j];
					R[i]=val;
				}//FOR
			return R;
		}//operator[]

		Matrix operator*(Matrix& M)
		{//operator*
			Matrix R(d1,M.d2);//!!
			if(d2==M.d1)
			{//if
				for(int i=0;i<d1;i++)
					for(int j=0;j<M.d2;j++)
					{//for
						float r=0.;
						for(int k=0;k<d2;k++) r+=row[i][k]*M[k][j];
						R[i][j]=r;
					}//for
			}//if
			return R;
		}//operator*

		void  mv1v2(Vector& V1,Vector& V2)//Matrix x Vector1 == Vector2
		{//mv1v2
			if(d2==V1.size)
				for(int i=0;i<d1;i++)
				{//FOR
					pfloat m1f=m1+i*d2;
					float val=0;
					for(int j=0;j<d2;j++) val+=V1[j]*m1f[j];
					V2[i]=val;
				}//FOR
		}//mv1v2

		pfloat& getRef()	{	return m1;	}

		~Matrix()
		{//destructor
			delete [] m1;
			delete [] row;//important (!)
		}//destructor

		void print()
		{//print
			for(int i=0;i<d1;i++,printf("\n"))
				for(int j=0;j<d2;j++)
					printf("%f  ",m1[i*d2+j]);
			printf("\n");
		}//print

		void print(char  message[])
		{//print
			printf("\n");
			printf(message);
			printf(":\n");
			for(int i=0;i<d1;i++,printf("\n"))
				for(int j=0;j<d2;j++)
					printf("%f  ",m1[i*d2+j]);
			printf("\n");
		}//print

		void randomize()
		{//randomize()
		/*
			initialization data by random values [0;1]
			parameters: no
			return: no
		*/
			for(int i=0;i<size;i++) m1[i]=rnd();
		}//randomize()

		void randomize_gauss()
		{//randomize_gauss
		/*
			initialization data by random values
			with gauss law
			parameters: no
			return: no
		*/
			for(int i=0;i<size;i++) m1[i]=gauss_val();
		}//randomize_gauss

		int  transpose_square()
		{//transpose
		/*
			transposal this square matrix
			parameters: no
			return: succcess/failure
		*/
			if(d1!=d2) return 0;
			for(int i=0;i<d1;i++)
				for(int j=i+1;j<d1;j++)
				{//swap
					float tmp=row[i][j];
					row[i][j]=row[j][i];
					row[j][i]=tmp;
				}//swap
			return 1;
		}//transpose

		int  transpose()
		{//transpose
		/*
			transposal any matrix
			parameters: no
			return: succcess/failure
		*/
			Matrix tmp(*this);
			int d=d1;
			d1=d2;
			d2=d;
			re_init_vecs();

			for(int i=0;i<d1;i++)
				for(int j=0;j<d2;j++)
					row[i][j]=tmp[j][i];
			return 1;
		}//transpose

		Vector gauss(Vector& B)
		{//Jordan-Gauss
		   /*l
			solving a system of linear algebraic equals
			using Jordan-Gauss method
			parameters:
				Matrix B - unknown values vector
			return: Vector - system solution
		*/
		  if(B.size==d1)
		  	if(d1==d2)
			{//d1==d2
				Matrix Row(*this);
				for(int t=0;t<d1;t++)
  				for(int i=t+1;i<d1;i++)
				{//for
					B[i]=B[i]-B[t]*Row[i][t]/Row[t][t];
   					for(int l=d1-1;l>=0;l--)
						Row[i][l]=Row[i][l]-Row[t][l]*Row[i][t]/Row[t][t];
				}//for

				for(int t=d1-1;t>=0;t--)
    				for(int i=t-1;i>=0;i--)
				{//for
					B[i]=B[i]-B[t]*Row[i][t]/Row[t][t];
     					for(int l=d1-1;l>=0;l--)
						Row[i][l]=Row[i][l]-Row[t][l]*Row[i][t]/Row[t][t];
				}//for


				for(int t=0;t<d1;t++)
				{//for
					B[t]=B[t]/Row[t][t];
					for(int i=d1-1;i>=t;i--)
						Row[t][i]=Row[t][i]/Row[t][t];
				}//for
			}//d1==d2
			return B;
		}//Jordan-Gauss

		Matrix gauss(Matrix& B/*d1xB.d2.*/)
		{//Jordan-Gauss
		//!! Not Optimal -- must be changed
		/*
			solving a system of linear algebraic equals
			using Jordan-Gauss method
			parameters:
				Matrix B - unknown values matrix
			return: Matrix - system solution
		*/
		  if(B.d1==d1)
		  	if(d1==d2)
			{//d1==d2
				Matrix Row(*this);
				for(int t=0;t<d1;t++)//forward
  				for(int i=t+1;i<d1;i++)
				{//for
					for(int l=B.d2-1;l>=0;l--)
					B[i][l]=B[i][l]-B[t][l]*Row[i][t]/Row[t][t];

  					for(int l=d1-1;l>=0;l--)
					Row[i][l]=Row[i][l]-Row[t][l]*Row[i][t]/Row[t][t];
				}//for

				for(int t=d1-1;t>=0;t--)//back
    				for(int i=t-1;i>=0;i--)
				{//for
					for(int l=B.d2-1;l>=0;l--)
					B[i][l]=B[i][l]-B[t][l]*Row[i][t]/Row[t][t];

     					for(int l=d1-1;l>=0;l--)
						Row[i][l]=Row[i][l]-Row[t][l]*Row[i][t]/Row[t][t];
				}//for

				for(int t=0;t<d1;t++)//diagonal elements == 1
				{//for
					for(int i=B.d2-1;i>=0;i--)
						B[t][i]=B[t][i]/Row[t][t];

					for(int i=d1-1;i>=t;i--)
						Row[t][i]=Row[t][i]/Row[t][t];
				}//for
			}//d1==d2
			return B;
		}//Jordan-Gauss

		Matrix operator-(const Matrix& M)
		{//operator-
			Matrix R(d1,d2);//!!
			if(M.d1==d1)
			if(M.d2==d2)
			for(int i=0;i<size;i++) R.m1[i]=m1[i]-M.m1[i];
			return R;
		}//operator-

		Matrix operator+(const Matrix& M)
		{//operator+
			Matrix R(d1,d2);//!!
			if(M.d1==d1)
			if(M.d2==d2)
			for(int i=0;i<size;i++) R.m1[i]=m1[i]+M.m1[i];
			return R;
		}//operator+

		void operator*=(const float fl)
		{//operator*=
			for(int i=0;i<size;i++) m1[i]=m1[i]*fl;
		}//operator*=

		void operator+=(const Matrix& M)
		{//operator+=
			if(M.d1==d1)
			if(M.d2==d2)
			for(int i=0;i<size;i++) m1[i]=m1[i]+M.m1[i];
		}//operator+=

		void operator-=(const Matrix& M)
		{//operator-=
			if(M.d1==d1)
			if(M.d2==d2)
			for(int i=0;i<size;i++) m1[i]=m1[i]-M.m1[i];
		}//operator-=

		Matrix holesky()
		{//holesky-decomposition
		/*
			for symmetric, plus matrixes
			parameters: no
			return: low triangle matrix
		*/
			Matrix TMP(*this);
			for(int j=0;j<d1;j++)
			{//for
				int n=d1-j;
				Vector v(n);
				for(int i=0;i<n;i++) v[i]=TMP[j+i][j];
				for(int k=0;k<=j-1;k++)
				{//for
					Vector G(n);
					for(int i=0;i<n;i++)	 G[i]=TMP[i+j][k];
					G*=TMP[j][k];
					v-=G;
				}//for
				if(v[0]<0) printf("Error: %f(V[0])<0\n",v[0]);
				float  sq=sqrt(fabs(v[0]));
				for(int i=0;i<n;i++) TMP[j+i][j]=v[i]/sq;
			}//for
			for(int i=0;i<d1;i++)
				for(int j=i+1;j<d1;j++) TMP[i][j]=0.;
			return TMP;
		}//holesky-decomposition

		Vector column(int n)
		{//column
		/*
			parameters:
				int n - number of column we need
			return: Vector-column of the matrix
		*/
			Vector tmp(d1);
			if(n<d2)
			for(int i=0;i<d1;i++) tmp[i]=row[i][n];
			return tmp;
		}//column

	private:
		Vector* row;//dynamic array of ROWs

	private:

		void init_vecs()
		{//init_vecs
			row=new Vector[d1];
			for(int i=0;i<d1;i++)
			{//for
				row[i].size=d2;
				row[i].IS_ROW=1;
				row[i].v1=m1+i*d2;
			}//for
		}//init_vecs

		void re_init_vecs()
		{//init_vecs
			delete [] row;
			init_vecs();
		}//init_vecs

		float gauss_val()
		{//gauss_val()
			float sum=0.;
			for(int i=0;i<12;i++) sum+=rnd();
			return sum-6.;
		}//gauss_val()
};//Matrix

#endif
