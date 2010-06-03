#ifndef		WAVES_H
#define		WAVES_H

//const float	Pi=3.14159265358979323846f;
const float	Pi = 3.14f;
const float	g = 9.81f;

#include "linear/linear.h"

//========��������� ���������� �����========
 const int		MAXGRID = 80;	//������ �����
 float	grid_step=20;	//��� ����� (�� ����������)
//========��������� ������������� �������� - 2D========
 float h3	=  5;// --- ������ ������ 3%-�� ��������������, � --- 	 			
 float Wmax  = 1.1f /*2.0f*/; // == ������������ ������� ==
 float BSp   = 0.030f /*1.3f*/ ; // == �������� ����� ������� ==
 float ASp   = 0.233f /*10*/; // == �������� �������� ������� ==

 float	Wmin = 0.1f /*0.2f*/ ;	//	����������� �������
 float	W0 = 0.71f  /*1.2f*/ ;	//	������� ������� �����, ���/c
 const int		NN = 400 ;		//	����� �������� � ������ �����-�������� (2D)
 const int		NN3D = 20 ;		//	����� �������� � ������ �����-�������� (3D)
 const int		NNet = 6;		//	����� �������� � 3D ������ 
 float	dW /* = (Wmax-Wmin)/(float)NN*/;//��� �����
 float	A_2D[NN];
 float	Fi_2D[NN];
 float	A_3D[NN3D][NNet];		    
 float	Fi3D[NN3D][NNet];

//========��������� ������������� �������� - 3D========
 const int p=5;// p>=2 : ������� �������� � ������ ������� 2*p 
 const float ETA_min=-Pi/2.;
 const float ETA_max= Pi/2.;
 const float dETA=(ETA_max-ETA_min)/(float)NNet;
//========��������� �����========
 const float	L=100.0f ;	//�����
 const float	B=15.0f	;	//������
 const int		Sm=81	;	//������� ���������
//========������� ��������========
 const float V =  0 /*14.0f*/  ;  //�������� �����, (m/s)
 float Xi =  22.; //�������� ����, ����.

//========������������ ���������========
//========�������� �����========
 const float nQ = 0.6f ;//
 const float a3 = 0.0f ;
 const float a5 = 0.0f ;
 const float v1_44 = 0.04f ;
 float Q=0.0f;//Q
 float DQ=0.;//Q'

//========������������ �����========
 const float v33 = 0.07f ;
 const float a33_1 = 0.50f ;
 const float a33_2 = 0.83f ;
 const int Mu33 = 700 ;
 const int Nu33 = 200 ;
 float E=0.;	//E
 float DE=0.;	//E'
 float alfa=0./*0.03f*/;
 float bS = 0.933f /* 1.44f */;
//========������� �����========
 const float v55 = 0.1f ;
 const float DH = 1.08f ;
 const float Jy2 = 8.33E5*1.5 ;//? -10^5
 float F=0.;	//F
 float DF=0.;	//F'
//========��������� ���������� ������========
 float	dT = 0.4f ;	//��� �������������� (��������� ��������)  
 float	ForX;		//����������� ����
 float	MomX;		//����������� ������

//========������� ��� ��������� ����������========
 float	W[NN];
 float	KxKyFi[MAXGRID][MAXGRID][NN][NNet];

void generator_initiation();//prototype

//=======��������� ���������� ��� ���������������� ���������========
void change_papams_to_bort_reson()
{//change_papams_to_bort_reson
	Wmax  = 2.0f;	// == ������������ ������� ==
	W0	  = 1.2f ;	//	������� ������� �����, ���/c
	BSp   = 1.3f ;	// == �������� ����� ������� ==
	ASp   = 10;		// == �������� �������� ������� ==
	Wmin  = 0.2f ;	//	����������� �������
	alfa  = /*0.03f*/ 0.12f/*0.115f*/ ;
	bS    = 1.44f ;
	grid_step=6;	//��� ����� (�� ����������)
	Xi=90;
		Q=0.;DQ=0;
		E=0.;DE=0;
	generator_initiation();
}//change_papams_to_bort_reson

//=======��������� ���������� ��� �������������� ���������������� ���������========
void change_papams_to_extr_bort_reson()
{//change_papams_to_bort_reson
	Wmax  = 2.0f;	// == ������������ ������� ==
	W0	  = 1.2f ;	//	������� ������� �����, ���/c
	BSp   = 1.3f ;	// == �������� ����� ������� ==
	ASp   = 10;		// == �������� �������� ������� ==
	Wmin  = 0.2f ;	//	����������� �������
	alfa  = 0.22f/*0.115f*/ ;
	bS    = 1.44f ;
	grid_step=6;	//��� ����� (�� ����������)
	Xi=90;
		Q=0.;DQ=0;
		E=0.;DE=0;
	generator_initiation();
}//change_papams_to_bort_reson

//=======��������� ���������� ��� ���������� ���������========
void change_papams_to_base_reson()
{//change_papams_to_bort_reson
	 Wmax = 1.1f /*2.0f*/;	// == ������������ ������� ==
	 W0   = 0.71f  /*1.2f*/ ;	//	������� ������� �����, ���/c
     BSp  = 0.030f /*1.3f*/ ; // == �������� ����� ������� ==
     ASp  = 0.233f /*10*/;	// == �������� �������� ������� ==
	 Wmin = 0.1f /*0.2f*/ ;	//	����������� �������
	 alfa=0./*0.03f*/;
	 bS = 0.933f /* 1.44f */;
	 grid_step=20;	//��� ����� (�� ����������)
	 Xi=22;
		Q=0.;DQ=0;
		E=0.;DE=0;
	generator_initiation();
}//change_papams_to_bort_reson

//========��������� ������������� ��������========
void generator_initiation()//init A & Fi
{//generator_initiation

	 // float Tmax=4.3f*/*sqrt(h3)*/pow(h3,0.5f);
	 // float m0=(h3*h3)/28.;
	 // Wmax=2*Pi/Tmax;
	 // BSp=(5./4.)*pow(Wmax,4);
	 // ASp=4*BSp*m0;

	dW=(Wmax-Wmin)/(float)NN;//��� �����
	for(int i=0;i<NN;i++) W[i]=Wmin+i*dW;

	
	for(int i=0;i<NN;i++)
	{//for
		float w=W[i];
		A_2D[i]=(float)sqrt(2*ASp*(1.0f/(w*w*w*w*w))*exp(-BSp/(w*w*w*w))*dW);
		Fi_2D[i]=(float)rand()*2.0f*Pi ;//rand() - 0..1

		/*
		for(int j=0;j<NNet;j++)
		{//for
			float ETA=ETA_min+j*dETA;
			float cos4=pow(cos(ETA),2*p);
			A_3D[i][j]=(float)sqrt(2*ASp*(1.0f/(w*w*w*w*w))*
						exp(-BSp/(w*w*w*w))*cos4*dW*dETA);
			Fi3D[i][j]=(float)rand()*2.0f*Pi;
		}//for	
		*/
	}//for
	
/*
	for(int px=0;px<MAXGRID;px++)
	{//for - px
		float	x=px*grid_step;
		for(int py=0;py<MAXGRID;py++)
		{//for py
			float	y=py*grid_step;
			for(int i=0;i<NN;i++)
				for(int j=0;j<NNet;j++)
				{//for j
					float	ETA=ETA_min+j*dETA;
					float	Kx=W[i]*W[i]*cos(ETA)/g;//g==9.81f
					float	Ky=W[i]*W[i]*sin(ETA)/g;//g==9.81f
					KxKyFi[px][py][i][j]=Kx*x + Ky*y + Fi3D[i][j];
				}//for j
		}//for py
	}//for - px 
*/
}//generator_initiation

void generator_initiation3D()//init A & Fi
{//generator_initiation
	change_papams_to_base_reson();

	dW=(Wmax-Wmin)/(float)NN3D;//��� �����
	for(int i=0;i<NN3D;i++) W[i]=Wmin+i*dW;
	
	for(int i=0;i<NN3D;i++)
	{//for
		float w=W[i];
		for(int j=0;j<NNet;j++)
		{//for
			float ETA=ETA_min+j*dETA;
			float cos4=pow(cos(ETA),2*p);
			A_3D[i][j]=(float)sqrt(2*ASp*(1.0f/(w*w*w*w*w))*
						exp(-BSp/(w*w*w*w))*cos4*dW*dETA);
			Fi3D[i][j]=(float)rand()*2.0f*Pi;
		}//for	
	}//for
	
	for(int px=0;px<MAXGRID;px++)
	{//for - px
		float	x=px*grid_step;
		for(int py=0;py<MAXGRID;py++)
		{//for py
			float	y=py*grid_step;
			for(int i=0;i<NN3D;i++)
				for(int j=0;j<NNet;j++)
				{//for j
					float	ETA=ETA_min+j*dETA;
					float	Kx=W[i]*W[i]*cos(ETA)/g;//g==9.81f
					float	Ky=W[i]*W[i]*sin(ETA)/g;//g==9.81f
					KxKyFi[px][py][i][j]=Kx*x + Ky*y + Fi3D[i][j];
				}//for j
		}//for py
	}//for - px 
}//generator_initiation3D


//=========���������� ����� ����� ���������� �������==========
void wave_pos2D(Matrix& pos/*x,y=1*/,float t)
{//wave_pos
	float x=0.;
	for(int p=0;p<pos.d1;p++)
	{//for-1
		float S=0.;
		for(int i=0;i<NN;i++)
		{//for-2
			float W=Wmin+i*dW;
			float K=W*W/g;//g==9.81f
			S+=A_2D[i]*cos(K*x - W*t + Fi_2D[i]);		
		}//for-2
		pos[p]=S;
		x+=grid_step;
	}//for-1
}//wave_pos

//=========���� ������� ������������� �����������==========
float Wave_Angle(float x, float t )//2D
{//Wave_Angle
 float S=0.;
 for(int i=0;i<NN;i++)
 {//for
  float W=Wmin + dW*i ;
  float  K=W*W/g ;
  S-=A_2D[i]*K*sin(K*x - W*t + Fi_2D[i]) ;
 }//for
 return S ;
}//Wave_Angle

//======����������� ���� � ������, ���������� ���������������========
void  Force_Moment(float X,float  t, float K0)//2D  
{//Force_Moment
 float Delta=L/10.0f ;
 float Sf=0.0f ;
 float Sm=0.0f ;
 float Qw=(B-K0*Sm);
 for(int i=0;i<10;i++)
 {//for1
  float X0=X-0.5f*L*cos(Xi/57.3f) + i*Delta*cos(Xi/57.3f);
  float S1=0.0f;
  for(int j=0;j<NN;j++)//�� ���� ����������
  {//for2
    float W=dW*j + Wmin ;
    float K=W*W/9.81f ;
    S1+=A_2D[j]*cos(K*X0 + W*t + Fi_2D[j]) ;//?+?
  }//for2
  Sf=Sf +  Qw*S1  ;
  Sm=Sm +  Qw*S1*(-0.5f*L+i*Delta)  ;
 }//for1
 ForX=Delta*Sf/(2*Mu33) ;
 MomX=Delta*Sm/(Jy2) ;
}//Force_Moment

//========�������-������ ����� ��������� �����=====================
//=============�������� �����======================================
float Rolling (float Q,float DQ, float  x, float t)//2D
{//Rolling
//����������� ������
 float DQ2=DQ*DQ ;
 float MDSO=nQ*nQ*(1.0f + a3*DQ2 + a5*DQ*DQ) ;
 float Mvozm=MDSO*fabs(sin(Xi/57.3f))*Wave_Angle(x,t) ;
//�������������
 float Fr=V/sqrt(9.81f*L) ;
 float v2_44=0.048f*Fr ;
 float V00=nQ*(v1_44 + 2*v2_44) ;
//������ �������
 return Mvozm - 2.0f*V00*DQ - Q*MDSO - alfa*Q*E ;
}//Rolling

//=========�������-������ ����� ��������� �����=====================
//=============������������ �����===================================
float Heaving (float Q,float DQ,float F,float DF,float x,float t)//2D
{//Heaving
	float K0=W0*W0/9.81f;
	Force_Moment(x,t,K0);//����������� ����
	//������ �������
	return ForX/*Mvozm*/ + V*a33_1*DF + V*a33_2*F - bS*Q - 2*2*v33*DQ ;
}//Heaving

//=========�������-������ ����� ��������� �����=====================
//==================������� �����===================================
float Pitching (float F,float  DF,float  x,float  t)//2D
{//Pitching
	float K0=W0*W0/9.81f;
	Force_Moment(x,t,K0) ;//����������� ������
	float V00=2.0f*(v55 + Nu33*(V*V/(W0*W0))/(Jy2)) ;//�������������
	float RC=V00;//?
	float MDSO=DH - V*V*(2.5E-4);	//����������������� ������
	return MomX/*??*/-MDSO*F-V00*DF;//������ �������
}//Pitching

//=========��� ������ �����-�����=========
void RK_Step(float x,	float t)
{//RK_Step
	float dX=/*dT*V*cos(Xi/57.3f)*//*grid_step*/0 ;
//===�������� �����===
   float K1=Rolling (Q, DQ, x, t)*dT ;
   float K2=Rolling (Q+DQ*dT/2.0f, DQ+K1/2.0f, x+dX/2 ,t+dT/2.0f)*dT ;
   float K3=Rolling (Q+DQ*dT/2.0f+K1*dT/4.0f, DQ+K2/2.0f, x+dX/2, t+dT/2.0f)*dT ;
   float K4=Rolling (Q+DQ*dT+K2*dT/2.0f, DQ+K3, t+dT, x+dX)*dT ;
//==���������� �������� �������� �������==
   Q=Q + DQ*dT + (K1 + K2 + K3)*dT/6.0f ;//Q
//==���������� �������� �������� �����������==
   DQ=DQ + (K1 + 2.0f*K2 + 2.0f*K3 + K4)/6.0f ;//Q'
//===������������ ����� � ������� �����===
   K1=Heaving(E, DE, F, DF, x, t)*dT ;
   float M1=Pitching (F, DF, x, t)*dT ;
   K2=Heaving(E+DE*dT/2.0f, DE+K1/2.0f,
               F+DF*dT/2.0f, DF+M1/2.0f,
               x+dX/2 ,t+dT/2.0f)*dT ;
   float M2=Pitching (F+DF*dT/2.0f, DF+M1/2.0f,
               x+dX/2 ,t+dT/2.0f)*dT ;
   K3=Heaving(E+DE*dT/2.0f+K1*dT/4.0f, DE+K2/2.0f,
                F+DF*dT/2.0f+M1*dT/4.0f, DF+M2/2.0f,
                x+dX/2, t+dT/2.0f)*dT ;
   float M3=Pitching(F+DF*dT/2.0f+M1*dT/4.0f,DF+M2/2.0f,x+dX/2,t+dT/2.0f)*dT ;
   K4=Heaving(E+DE*dT+K2*dT/2.0f, DE+K3,
              F+DF*dT+M2*dT/2.0f, DF+M3,
              t+dT, x+dX)*dT ;
   float M4=Pitching(F+DF*dT+M2*dT/2.0f,DF+M3,t+dT,x+dX)*dT ;
//==���������� �������� �������� �����������==
   DE +=(K1 + 2.0f*K2 + 2.0f*K3 + K4)/6.0f ;
   DF +=(M1 + 2.0f*M2 + 2.0f*M3 + M4)/6.0f ;
//==���������� �������� �������� �������==
   E += DE*dT + (K1 + K2 + K3)*dT/6.0f ;
   F += DF*dT + (M1 + M2 + M3)*dT/6.0f ;
}//RK_Step

//=========���������� ����� ����� ���������� �������==========
void wave_pos3D(Matrix& pos,float t)
{//wave_pos
	for(int px=0;px<MAXGRID;px++)
		for(int py=0;py<MAXGRID;py++)
		{//for - py
			float	S=0./*rnd()*0.01f*/;
			for(int i=0;i<NN3D;i++)
				for(int j=0;j<NNet;j++)
					S+=A_3D[i][j]*cos(KxKyFi[px][py][i][j] - W[i]*t);		
			pos[px][py]=S;
		}//for - py
}//wave_pos

#endif

/*
void wave_pos3D(Matrix& pos,float t)
{//wave_pos
	for(int px=0;px<pos.d1;px++)
	{//for - px
		float	x=px*grid_step;
		for(int py=0;py<pos.d2;py++)
		{//for - py
			float	y=py*grid_step;
			float	S=0.;
			for(int i=0;i<NN;i++)
			{//for-2
				float	W=Wmin+i*dW;//Omega
				for(int j=0;j<NN;j++)
				{//for - i
					float	ETA=ETA_min+j*dETA;
					float	Kx=W*W*cos(ETA)/g;//g==9.81f
					float	Ky=W*W*sin(ETA)/g;//g==9.81f
					S+=A_3D[i][j]*cos(Kx*x + Ky*y - W*t + Fi3D[i][j]);		
				}//for - i
			}//for-2
			pos[px][py]=S;
		}//for - py
	}//for - px
}//wave_pos
*/