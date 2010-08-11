 /***************************************************************************
                         newave.h  -  description
			 3D Waves with Textures + 3D Model 
                             -------------------
    begin                :  12.2003
    copyright			 : (C) 2003 by  Sergei V. Ivanov, freeware
    email                : zarva@hotbox.ru
 ***************************************************************************/
#include <math.h>
#include <stdlib.h>
#include "waves.h"

//#define MAXGRID 100

enum {WIREFRAME, HIDDENLINE, FLATSHADED, SMOOTHSHADED, TEXTURED};
enum {FULLSCREEN, FACENORMALS, ANTIALIAS, ENVMAP};
enum {WEAK, NORMAL, STRONG};
enum {SMALL, MEDIUM, LARGE, XLARGE};
enum {FASTER, LOWER, STOP};
enum {EXIT, DEMO,GO,NO_MOVE};
enum {CURRENT, FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE, 
      MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR,BOUKH_WAVE,_2DW,_3DW/*,
	  BASE,PARAM, PARAM_EXTR*/};
enum {BASE,PARAM, PARAM_EXTR};
int wave_mode=_2DW;//or _2DW
int resetMode = DIAGONALBLOCK;
int grid = 17;
//int grid = MAXGRID;
bool Tonem = false;
bool Demo = false;

float dt = 0.004f;
float force[MAXGRID][MAXGRID],
      veloc[MAXGRID][MAXGRID],
//    posit[MAXGRID][MAXGRID],
      vertNorms[MAXGRID][MAXGRID][3],
      faceNorms[2][MAXGRID][MAXGRID][3],
      faceNormSegs[2][2][MAXGRID][MAXGRID][3];

Matrix   posit(MAXGRID,MAXGRID);

bool waving = false, editing = false, 
     drawFaceNorms = false, antialias = false,
     envMap = false;
#define SQRTOFTWOINV 1.0 / 1.414213562

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int C=30;

int texWidth, texHeight;
//unsigned char *texData;
char *texFilename1 = "korpus.rgb", *texFilename2 = "wave.rgb";
//unsigned int texId1, texId2;
float texCoords[MAXGRID][MAXGRID][2];

/* Viewing */
float sphi=90.0, stheta=55/*45.0*/;
float sdepth = /*10.0/4.0 * MAXGRID*/ 1.7f;
float zNear=15.0, zFar=100.0;
float aspect = 5.0/4.0;
long xsize, ysize;
int downX, downY;

float frw_bck=0.0;//	forward-back - translation
float rht_lft=0.0;//	left-right - translation
//float direct=0./*0.1*/;

//***FUNCTIONS***
void tonu();
void go(void);
void reset(int value);
void resetReson(int value);
void setSize1(int value);
void reshape(int width, int height);
void display(void);
void setSpeed(int value);
void setDisplay(int value);
void setOther(int value);
void keyboard(unsigned char ch, int x, int y);
void visibility(int state);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void create_sw_menu();
void drawmodel(void);
float normalize(float* v);
void rotSet(int value);


float cf=15.0;//decreasing coefficient
float rotcf=1.;

//***FUNCTIONS***
//GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

void setfont(char* name, int size)
{
}


void getforce(void)
{//getforce
    float d;
    
    int i, j;

    for(i=0;i<grid;i++) 
        for(j=0;j<grid;j++) 
        {//for-for
            force[i][j]=0.0;
        }//for-for

    for(i=2;i<grid;i++)
        for(j=2;j<grid;j++) 
        {//for-for
            d=posit[i][j]-posit[i][j-1];
            force[i][j] -= d;
            force[i][j-1] += d;

            d=posit[i][j]-posit[i-1][j];
            force[i][j] -= d;
            force[i-1][j] += d;

            d= (posit[i][j]-posit[i][j+1]); 
            force[i][j] -= d ;
            force[i][j+1] += d;

            d= (posit[i][j]-posit[i+1][j]); 
            force[i][j] -= d ;
            force[i+1][j] += d;

            d= (posit[i][j]-posit[i+1][j+1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i+1][j+1] += d;

            d= (posit[i][j]-posit[i-1][j-1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i-1][j-1] += d;

            d= (posit[i][j]-posit[i+1][j-1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i+1][j-1] += d;

            d= (posit[i][j]-posit[i-1][j+1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i- 1][j+1] += d;
        }//for-for
}//getforce

void getvelocity(void)
{//getvelocity
	int i,j;

    for(i=0;i<grid;i++)
        for(j=0;j<grid;j++)
            veloc[i][j]=veloc[i][j]+force[i][j] * dt;
}//getvelocity

void getposition(void)
{//getposition
	int i,j;
    for(i=0;i<grid;i++)
        for(j=0;j<grid;j++)
            posit[i][j]=posit[i][j]+veloc[i][j];
}//getposition


void copy(float vec0[3], float vec1[3])
{
    vec0[0] = vec1[0];
    vec0[1] = vec1[1];
    vec0[2] = vec1[2];
}

void sub(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] - vec2[0];
    vec0[1] = vec1[1] - vec2[1];
    vec0[2] = vec1[2] - vec2[2];
}

void add(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] + vec2[0];
    vec0[1] = vec1[1] + vec2[1];
    vec0[2] = vec1[2] + vec2[2];
}

void scalDiv(float vec[3], float c)
{
    vec[0] /= c; vec[1] /= c; vec[2] /= c;
}

void cross(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    vec0[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    vec0[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void norm(float vec[3])
{
    float c = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    scalDiv(vec, c); 
}

void set(float vec[3], float x, float y, float z)
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}


/* face normals - for flat shading */
void getFaceNorms(void)
{//getFaceNorms
	int i,j;
    float vec0[3], vec1[3], vec2[3], norm0[3], norm1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
    for (i = 0; i < grid-1; ++i)
    {
        for (j = 0; j < grid-1; ++j)
        {
            /* get vectors from geometry points */
            geom0[0] = i; geom0[1] = j; geom0[2] = posit[i][j];
            geom1[0] = i; geom1[1] = j+1; geom1[2] = posit[i][j+1];
            geom2[0] = i+1; geom2[1] = j; geom2[2] = posit[i+1][j];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            sub( vec0, geom1, geom0 );
            sub( vec1, geom1, geom2 );
            sub( vec2, geom1, geom3 );

            /* get triangle face normals from vectors & normalize them */
            cross( norm0, vec0, vec1 );
            norm( norm0 );

            cross( norm1, vec1, vec2 ); 
            norm( norm1 );

            copy( faceNorms[0][i][j], norm0 );
            copy( faceNorms[1][i][j], norm1 );
        }
    }
}//getFaceNorms

/* vertex normals - average of face normals for smooth shading */
void getVertNorms(void)
{//getVertNorms
    float avg[3];
//	int r;
	int i,j;

    for (i = 0; i < grid; ++i)
    {
        for (j = 0; j < grid; ++j)
        {
            /* For each vertex, average normals from all faces sharing */
            /* vertex.  Check each quadrant in turn */
            set(avg, 0.0, 0.0, 0.0);

            /* Right & above */
            if (j < grid-1 && i < grid-1)
            {
                add( avg, avg, faceNorms[0][i][j] );
            }
            /* Right & below */
            if (j < grid-1 && i > 0)
            {
                add( avg, avg, faceNorms[0][i-1][j] );
                add( avg, avg, faceNorms[1][i-1][j] );
            }
            /* Left & above */
            if (j > 0 && i < grid-1)
            {
                add( avg, avg, faceNorms[0][i][j-1] );
                add( avg, avg, faceNorms[1][i][j-1] );
            }
            /* Left & below */
            if (j > 0 && i > 0)
            {
                add( avg, avg, faceNorms[1][i-1][j-1] );
            }

            /* Normalize */
            norm( avg );

			//for(r=0;r<3;r++) avg[r]=avg[r]/5.;

            copy( vertNorms[i][j], avg );
        }
    }
}//getVertNorms


void getFaceNormSegs(void)
{
    float center0[3], center1[3], normSeg0[3], normSeg1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
	int i,j;
    for (i = 0; i < grid - 1; ++i)
    {
        for (j = 0; j < grid - 1; ++j)
        {
            geom0[0] = i; geom0[1] = j; geom0[2] = posit[i][j];
            geom1[0] = i; geom1[1] = j+1; geom1[2] = posit[i][j+1];
            geom2[0] = i+1; geom2[1] = j; geom2[2] = posit[i+1][j];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            /* find center of triangle face by averaging three vertices */
            add( center0, geom2, geom0 );
            add( center0, center0, geom1 );
            scalDiv( center0, 3.0 );

            add( center1, geom2, geom1 );
            add( center1, center1, geom3 );
            scalDiv( center1, 3.0 );

            /* translate normal to center of triangle face to get normal segment */
            add( normSeg0, center0, faceNorms[0][i][j] );
            add( normSeg1, center1, faceNorms[1][i][j] );

            copy( faceNormSegs[0][0][i][j], center0 );
            copy( faceNormSegs[1][0][i][j], center1 );

            copy( faceNormSegs[0][1][i][j], normSeg0 );
            copy( faceNormSegs[1][1][i][j], normSeg1 );
        }
    }
}

void getTexCoords(void)
{//getTexCoords
	int i,j;
    for (i = 0; i < grid; ++i)
    {//for 1
        for (j = 0; j < grid; ++j)
        {//for 2
            texCoords[i][j][0] = (float)j/(float)(grid-1);
            texCoords[i][j][1] = (float)i/(float)(grid-1);
        }//for 2
    }//for 1
}//getTexCoords

void tonu()
{//tonu
	float speed_cf=dT/0.4;
	float dQ0 = 0.5*speed_cf;
	float dQ1 = 0.3*speed_cf;
	float dQ2 = 0.15*speed_cf;
	float dE1 = 5.3*speed_cf;

	float UGL=(Q)*57.3;
	if((UGL>90)&&(UGL<180)) 
	{
		Q+=dQ0;
		if(UGL>150) Q-=dQ1;
		if(UGL>170) Q-=dQ2;
	}

	if((UGL<-90)&&(UGL>-180)) 
	{
		Q-=dQ0;
		if(UGL<-150) Q+=dQ1;
		if(UGL<-170) Q+=dQ2;
	}


	if((UGL>90)||(UGL<-90)) 
	{
		if(E>-35) E-=0.5*dE1;
		if(E>-10) E-=dE1;
		if(E>0) E-=2*dE1;
	}
}//tonu

float iterat=0;
float speed=0.5;
float step=0.;

void init_boukh_wave()
{//init_boukh_wave
	if(wave_mode==_3DW)
	{//_3DW
		iterat+=/*speed*/dT;
		wave_pos3D(posit/*x,y=0*/,iterat/*time*/);
		posit*=0.025f;//Decreation coef
	}//_3DW
	else
	{//_2DW
		iterat+=/*speed*/dT;
//		wave_pos2D(posit/*x,y=0*/,iterat/*time*/);

		float UGL=Q*57.3;
		if((UGL<90)&&(UGL>-90))
		RK_Step(MAXGRID*grid_step/2,	iterat);
		else
		{//esle
			tonu();
			Tonem=true;
		}//else

		posit.transpose();
		posit*=0.025f;//Decreation coef
	}//_2DW
}//init_boukh_wave

void wave(void)
{//wave
//	glutPostRedisplay();
}//wave

void go(void)
{//go
    waving = true;
    editing = false;
//    glutIdleFunc(wave);//argument - function
}//go

void stop(void)
{//stop
    waving = false;
//    glutIdleFunc(NULL);//argument - function
}//stop

void edit(void)
{//edit
    stop();
    editing = true;
}//edit

void reverse(void)
{//reverse
	int i,j;
    for(i=1;i<(grid-1);i++)
        for(j=1;j<(grid-1);j++)
            veloc[i][j]= -veloc[i][j];
    if (!waving)   go();
}//reverse

void resetReson(int value)
{//resetReson
	switch(value)
            {//switch
			case BASE:
				{//BASE
					Tonem=false;
					reset(_2DW);
					change_papams_to_base_reson();
					Demo=false;
					go();

					sphi=95.0;
					stheta=55;
					sdepth = 1.7f;

					frw_bck=0.;
					rht_lft=0.;
				}//BASE
                break;
			case PARAM:
				{//PARAM
					Tonem=false;
					reset(_2DW);
					change_papams_to_bort_reson();	
					Demo=false;
					go();


					sphi=35.0;
					stheta=55;
					sdepth = 1.7f;

					frw_bck=0.;
					rht_lft=0.;
				}//PARAM
                break;
         	case PARAM_EXTR:
				{//Extrem
					Tonem=false;
					reset(_2DW);
					change_papams_to_extr_bort_reson();
					Demo=false;
					go();

					sphi=35.0;
					stheta=55;
					sdepth = 1.7f;

					frw_bck=0.;
					rht_lft=0.;
				}//Extrem
		        break;        
            }//switch
//    glutPostRedisplay();
}//resetReson

void reset(int value)
{//reset
    if (value != CURRENT)
        resetMode = value;
        {//for
            switch(resetMode)
            {//switch
			case _3DW:
				{//_3DW
					wave_mode=_3DW;
					//waving=true;
				}//_3DW
                break;
			case _2DW:
				{//_2DW
					wave_mode=_2DW;
					//waving=true;
				}//_2DW
                break;
         	case BOUKH_WAVE:
					init_boukh_wave();//only once
		        break;        
            }//switch
        }//for
//    glutPostRedisplay();
}//reset

void setSize1(int value)
{//setSize1
    int prevGrid = grid;
    switch(value) 
    {//switch
        case SMALL : grid = MAXGRID/4; break;
        case MEDIUM: grid = MAXGRID/2; break;
        case LARGE : grid = MAXGRID/1.5; break;
        case XLARGE : grid = MAXGRID; break;
    }//switch

    if (prevGrid > grid)
    {//if
        reset(resetMode);
    }//if

    zNear= grid/10.0;
    zFar= grid*3.0;
    //sdepth = 5.0/4.0 * grid;
    getTexCoords();
//    glutPostRedisplay();
}//setSize1

void setSpeed(int value)
{//setSpeed
    switch(value) 
    {//switch
		case WEAK  : 
			{//WEAK
				if(dT-0.15>0)
				dT-=0.15f;
			}//WEAK
				break;
        case NORMAL:
			{//WEAK
				dT=0.4f;
			}//WEAK
				break;
        case STRONG:
			{//WEAK
				dT+=0.15f;
			}//WEAK
				 break;
    }//switch
}//setSpeed
