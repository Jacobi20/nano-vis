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
		wave_pos2D(posit/*x,y=0*/,iterat/*time*/);

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
//void setMain(int value)
//{//setMain
//    switch(value) 
//    {//switch
//        //case 1:		edit();    break;
//        case GO:		go();		break; /* set idle func to something */
//        case NO_MOVE:	stop();		break; /* set idle func to null */
//        case EXIT:		exit(0);	break;
//		case DEMO:		
//		{//Demo
//			Demo=true;  
//			reset(_2DW);
//			change_papams_to_base_reson();
//			go() ;
//
//			sphi=90.0;
//			stheta=55;
//			sdepth = 1.7f;
//
//			frw_bck=0.;
//			rht_lft=0.;
//		}//Demo
//									break;
//		case _3DW:
//		{//_3DW
//			Demo=false;
//			Tonem=false;
//			reset(_3DW);
//			generator_initiation3D();
//			sphi=45.0;
//			stheta=55;
//			sdepth = 1.7;
//			go();
//		}//_3DW
//    }//switch
//}//setMain
//
//
//void drawFaceNormals(void)
//{//drawFaceNormals
//    glColor3f(1.0,1.0,1.0);
//    for (i = 0; i < grid - 1; ++i)
//    {//for 1
//        for (j = 0; j < grid - 1; ++j)
//        {//for 2
//            glBegin(GL_LINES);
//				glVertex3fv(faceNormSegs[0][0][i][j]);
//				glVertex3fv(faceNormSegs[0][1][i][j]);
//            glEnd();
//
//            glBegin(GL_LINES);
//				glVertex3fv(faceNormSegs[1][0][i][j]);
//				glVertex3fv(faceNormSegs[1][1][i][j]);
//            glEnd();
//        }//for 2
//    }//for 1
//}//drawFaceNormals
//
//void drawSmoothShaded(void)//waves only 
//{//drawSmoothShaded
//	glEnable(GL_COLOR_MATERIAL);//!!
//	glColor3f(0.1f, 0.6f, 1.0f);
//    for (i = 0; i < grid - 1; ++i)
//    {//for
//        glBegin(GL_TRIANGLE_STRIP);//BEGIN DRAW
//        for (j = 0; j < grid; ++j)
//        {//for
//			glNormal3fv( vertNorms[i][j] );
//				glVertex3f( i/cf, j/cf, posit[i][j] );
//			glNormal3fv( vertNorms[i+1][j] );
//				glVertex3f( (i+1)/cf, j/cf, posit[i+1][j] );
//        }//for
//        glEnd();//END DRAW
//   }//for
//	glDisable(GL_COLOR_MATERIAL);//!!
////*/
//}//drawSmoothShaded
//
//void drawWireframe(void)
//{//drawWireframe
//    glColor3f(1.0, 1.0, 1.0);
//
//    for(i=0;i<grid;i++)
//    {
//        glBegin(GL_LINE_STRIP);
//        for(j=0;j<grid;j++)
//            glVertex3f( (float) i/cf, (float) j/cf, (float) posit[i][j]);
//        glEnd();
//    }
//    
//    for(i=0;i<grid;i++)
//    {
//        glBegin(GL_LINE_STRIP);
//        for(j=0;j<grid;j++)
//            glVertex3f( (float) j/cf, (float) i/cf, (float) posit[j][i]);
//        glEnd();
//    }
//}//drawWireframe
//
//void drawFlatShaded(void)
//{
//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glColor3f(0.8f, 0.2f, 0.8f);
//    for (i = 0; i < grid - 1; ++i)
//    {
//        glBegin(GL_TRIANGLE_STRIP);
//        glVertex3f( (float) i, (float) 0, (float) posit[i][0]);
//        glVertex3f( (float) i+1, (float) 0, (float) posit[i+1][0]);
//        for (j = 1; j < grid; ++j)
//        {
//            glNormal3fv( faceNorms[0][i][j-1] );
//            glVertex3f( (float) i, (float) j, (float) posit[i][j]);
//              glNormal3fv( faceNorms[1][i][j-1] );
//            glVertex3f( (float) i+1, (float) j, (float) posit[i+1][j]);
//        }
//        glEnd();
//    }
//    glDisable(GL_POLYGON_OFFSET_FILL);
//}
//
//void drawHiddenLine(void)
//{
//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glColor3f(0.8f, 0.2f, 0.8f);
//    for (i = 0; i < grid - 1; ++i)
//    {
//        glBegin(GL_TRIANGLE_STRIP);
//        glVertex3f( (float) i, (float) 0, (float) posit[i][0]);
//        glVertex3f( (float) i+1, (float) 0, (float) posit[i+1][0]);
//        for (j = 1; j < grid; ++j)
//        {
//            glVertex3f( (float) i, (float) j, (float) posit[i][j]);
//            glVertex3f( (float) i+1, (float) j, (float) posit[i+1][j]);
//        }
//        glEnd();
//    }    
//    glDisable(GL_POLYGON_OFFSET_FILL);
//    
//    glColor3f(1.0,1.0,1.0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    for (i = 0; i < grid - 1; ++i)
//    {
//        glBegin(GL_TRIANGLE_STRIP);
//        glVertex3f( (float) i, (float) 0, (float) posit[i][0]);
//        glVertex3f( (float) i+1, (float) 0, (float) posit[i+1][0]);
//        for (j = 1; j < grid; ++j)
//        {
//            glVertex3f( (float) i, (float) j, (float) posit[i][j]);
//            glVertex3f( (float) i+1, (float) j, (float) posit[i+1][j]);
//        }
//        glEnd();
//    }    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
//
//void loadImageTexture(void)
//{//loadImageTexture
//
//    glGenTextures(1,&texId1);
//    glBindTexture(GL_TEXTURE_2D, texId1);
//    imgLoad(texFilename1, 0, 0, &texWidth, &texHeight, &texData);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, 
//        texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//        texData );
//
//    glGenTextures(1,&texId2);
//    glBindTexture(GL_TEXTURE_2D, texId2);
//    imgLoad(texFilename2, 0, 0, &texWidth, &texHeight, &texData);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, 4, 
//        texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//        texData );
//}//loadImageTexture
//
//void drawTextured(void)
//{
//    glColor3f(1.0f, 1.0f, 1.0f);
//    glEnable(GL_TEXTURE_2D);
//    for (i = 0; i < grid - 1; ++i)
//    {
//        glBegin(GL_TRIANGLE_STRIP);
//        for (j = 0; j < grid; ++j)
//        {
//            glNormal3fv( vertNorms[i][j] );
//            glTexCoord2fv( texCoords[i][j] );
//            glVertex3f( i/cf, j/cf, posit[i][j] );
//            glNormal3fv( vertNorms[i+1][j] );
//            glTexCoord2fv( texCoords[i+1][j] );
//            glVertex3f( (i+1)/cf, j/cf, posit[i+1][j] );
//        }
//        glEnd();
//    }
//    glDisable(GL_TEXTURE_2D);
//}
//
//
//void reshape(int width, int height)
//{//reshape
//	glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0, width, height, 0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//}//reshape
//	
////directions & angles for rotation
//float cf_sphi=0.5;
//float cf_stheta=0.4;
//float cf_sdepth=0.01;

//void display(void) 
//{//display
/////*
//	//glClearColor(0.0 , 0.0, 0.4, 1.0);
//	//glClearColor(0.0 , 0.0, 0.2, 0.0);
//	//if (waving)		init_boukh_wave();
//
////	glPushMatrix();
////			glColor3f(1.0,1.0,0.0);
////			glTranslatef(-0.7,+0.55,-1.0);
////			setfont("helvetica", 10);
////			char buf[160];
////			//sprintf(buf,"Center: %f",posit[50][50]*5);
////			int xi=abs((int)Xi);
////			if(xi>180) xi=abs(360-xi);
////			
////
////sprintf(buf,"Course: %d Rolling: %d  Heaving: %d Pitching: %d",xi,(int)(Q*57.3),
////				(int)E,(int)(F*57.3));
////			
////			//fprintf(vert,"%f\n",E);
////			//fprintf(bort,"%f\n",Q*57.3);
////			//if(iterations++==200) exit(0);_3DW
////			if(!Demo)
////			if(wave_mode!=_3DW)
////			if(!Tonem)
////			drawstr(0.0, .5 , buf);
////			if(Tonem)
////				if(int(iterat)%2!=0)
////			drawstr(0.0, .5 , "Nave has sunk");
////	glPopMatrix();
//
//
//	glPushMatrix();//push - 0
//
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		gluPerspective(64.0, aspect, 0.1, zFar);
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity(); 
//		glTranslatef(0.0,0.0,-sdepth);
//
//		glRotatef(-stheta, 1.0, 0.0, 0.0);
//		glRotatef(sphi, 0.0, 0.0, 1.0);
//
//		glTranslatef(frw_bck/200.,0.0,0.0);//forward+/back-
//		glTranslatef(0.0,rht_lft/200.,0.0);//left-/right+
//
//		//glRotatef(fabs(++angle2/8.), 0.0, 0.0, 1.0);//camera rotation
//
//		glPushMatrix();//push - 1
//					
//			glTranslatef(-(float)((grid+1)/(cf*2)), -(float)((grid+1)/(cf*2)), 0.0);
//			getFaceNorms();
//			getVertNorms();
//		
//		switch (displayMode) 
//		{//switch 
//			case WIREFRAME: drawWireframe();		break;
//			case SMOOTHSHADED: drawSmoothShaded();	break;
//			case TEXTURED: drawTextured();			break;
//		}//switch 
//		
//
//		if (drawFaceNorms)    
//		{//drawFaceNorms
//			getFaceNormSegs();
//			drawFaceNormals();
//		}//drawFaceNorms
//
//	glPopMatrix();//pop - 1
//
//	glPushMatrix();//push - 2
//		glTranslatef(0.0,0.0, 0.25+E*0.015/*015*/);//!!! - vert
//		//glRotatef(fabs(angle2/4.), 0.0, 0.0, 1.0);//ship rotation
//		glRotatef(90, 1.0, 0.0, 0.0);
//		glRotatef(Xi, 0.0, 1.0, 0.0);
//		glColor3f(0.8f, 0.8f, 0.8f);
//
//		glRotatef(F*57.3, 1.0, 0.0, 0.0);//kil
//		glRotatef(Q*57.3, 0.0, 0.0, 1.0);//bort
//		if(wave_mode!=_3DW)
//		drawmodel();
//		
//		float speed_cf=dT/0.4;
//		if(waving)	
//			if(Demo)	
//			{//if
//				Xi+=speed_cf;//change kurs angle 
//					sphi+=cf_sphi*speed_cf;
//				if((sphi>550)||(sphi<-200)) cf_sphi=-cf_sphi;
//					stheta+=cf_stheta*speed_cf;
//				if((stheta<15)||(stheta>75)) cf_stheta=-cf_stheta;
//					sdepth+=cf_sdepth*speed_cf;
//				if((sdepth<0.6)||(sdepth>2)) cf_sdepth=-cf_sdepth;
//			}//if
//		if((Xi>360)||(Xi<-360))	Xi=0.;
//
//	glPopMatrix();//pop - 2
//	glPopMatrix();//pop - 0
//
////    glutSwapBuffers();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}//display
//
//void visibility(int state)
//{
//    //if ((state == GLUT_VISIBLE) && waving)
//    //    go();
//    //else 
//    //    stop();
//}
//
//void motion(int x, int y)
//{
//	if(!Demo)
//	{//Demostration
//		if (leftButton) 
//		{//leftButton
//			frw_bck+=(float)(downY - y) / 10.0;
//			rht_lft+=(float)(x - downX) / 10.0;
//			//sphi += (float)(x - downX) / 4.0;
//			//stheta += (float)(downY - y) / 4.0;
//		}//leftButton
//
//		if (middleButton)
//        sdepth += (float)(downY - y) / 10.0;
//
//	    downX = x;//old values
//	    downY = y;//old values
//	}//Demostration
//
////    glutPostRedisplay();
//}
//
//void mouse(int button, int state, int x, int y)
//{//mouse
//    //downX = x;
//    //downY = y;
//    //leftButton = ((button == GLUT_LEFT_BUTTON) && 
//    //              (state == GLUT_DOWN));
//    //middleButton = ((button == GLUT_MIDDLE_BUTTON) && 
//    //                (state == GLUT_DOWN));
//}//mouse
//
//
//void keyboard(unsigned char ch, int x, int y)
//{//keyboard
//    switch (ch) 
//    {
//		case 51:  sphi-=2; break;
//		case 49:  sphi+=2; break;
//
//		case 56: if(stheta>15) stheta-=2; break;
//		case 50: if(stheta<75)stheta+=2; break;
//
//		case 52: if(!Tonem) Xi-=2; break;
//		case 54: if(!Tonem) Xi+=2; break;
//
//        case '-': sdepth += 0.05; break;
//        case '+': if(sdepth>.6)sdepth-=0.05; break;
//        case 27: exit(0); break;
//		//default: C=(int)ch;
//    }
//    //glutPostRedisplay();
//}//keyboard
//   
//void create_sw_menu()
//{//create_sw_menu
//#if 0
//	displayMenu = glutCreateMenu(setDisplay);
//		glutAddMenuEntry("Сетка", WIREFRAME);
////		glutAddMenuEntry("Сглаживание", SMOOTHSHADED);
//		glutAddMenuEntry("Текстуры", TEXTURED);
//
///*
//    otherMenu = glutCreateMenu(setOther);
//		glutAddMenuEntry("Full Screen", FULLSCREEN);
//		glutAddMenuEntry("Face Normals", FACENORMALS);
//		glutAddMenuEntry("Antialias", ANTIALIAS);
//		//glutAddMenuEntry("Environment Map", ENVMAP);
//*/
//
//    speedMenu = glutCreateMenu(setSpeed);
//		glutAddMenuEntry("Быстрее", STRONG);
//		glutAddMenuEntry("Медленнее", WEAK);
//		glutAddMenuEntry("Нормально", NORMAL);
//
//	rotMenu=glutCreateMenu(rotSet);
//		glutAddMenuEntry("Быстрее", FASTER);
//		glutAddMenuEntry("Медленнее", LOWER);
//		glutAddMenuEntry("Отсутсвует", STOP);
//    
///*
//    sizeMenu = glutCreateMenu(setSize1);
//		glutAddMenuEntry("Small", SMALL);
//		glutAddMenuEntry("Medium", MEDIUM);
//		glutAddMenuEntry("Large", LARGE);
//		glutAddMenuEntry("Extra Large", XLARGE);
//*/
//
//    resetMenu = glutCreateMenu(resetReson);
//		glutAddMenuEntry("Основной", BASE);
//		glutAddMenuEntry("Параметрический", PARAM);
//		glutAddMenuEntry("Сильный параметрический", PARAM_EXTR);
//
//    mainMenu = glutCreateMenu(setMain);
//		glutAddMenuEntry("Поплыли ", GO);
//		glutAddMenuEntry("Стоп", NO_MOVE);
//		// glutAddMenuEntry("Reverse", 4);
//		glutAddSubMenu("Режим отображения", displayMenu);
//		glutAddSubMenu("Тип резонанса", resetMenu);
//		// glutAddSubMenu("Размер", sizeMenu);
//		glutAddSubMenu("Скорость", speedMenu);
//		// glutAddSubMenu("Дополнительно", otherMenu);
//		// glutAddSubMenu("Автоматическое вращение", rotMenu);
//		glutAddMenuEntry("Трёхмерное волнение", _3DW);
//		glutAddMenuEntry("Демонстрация", DEMO);
//		glutAddMenuEntry("______________", 100);
//		glutAddMenuEntry("Выход (ESC)", EXIT);
//
//	glutAttachMenu(GLUT_RIGHT_BUTTON);
//#endif	
//}//create_sw_menu
//
//void drawmodel(void)
//{//drawmodel
//	LOGF(__FUNCTION__);
//}//drawmode