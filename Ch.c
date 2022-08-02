#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265359
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 //1 graun en radionos	

int mapX = 8, mapY = 8, mapS = 64;

float PlayX, PlayY, pDeltax, pDeltay, pAng; //PlayerPosition

int map[] =
{
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 1, 1,
	1, 0, 1, 1, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

void drawPlayer()
{
	glColor3f(1, 2, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(PlayX, PlayY);
	glEnd();
	
	glLineWidth (3);
	glBegin(GL_LINES);
	glVertex2i (PlayX, PlayY);
	glVertex2i (PlayX + pDeltax*5, PlayY + pDeltay*5);
	glEnd();
}

void drawMap2D()
{
	int x, y, xo, yo;
	
	for(y = 0; y < mapY; y++)
	{
		for(x = 0; x < mapX; x++)
		{
			if(map[y*mapX + x] == 1)
			{
				glColor3f(1, 1, 1);
			}
			else 
			{
				glColor3f(0, 0, 0);
			}
			xo = x*mapS;
			yo = y*mapS;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + mapS - 1);			
			glVertex2i(xo + mapS - 1, yo + 1);
			glEnd();
		}
	}
}

float degToRad(int a)
{ 
	return a*M_PI/180.0;
}

int FixAng (int a)
{
	if(a > 359)
	{
		a -= 360;
	}
	if (a < 0)
	{
		a += 360;
	}

	return a;
}

float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void DrawRays2D()
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, disT;
	ra = pAng - DR * 30;

	if(ra < 0) 
	{
		ra += 2*PI;
	}
	if(ra > 2*PI)
	{
		ra -= 2*PI;
	}
	
	for(r = 0; r < 60; r++)
	{
		dof = 0;//linhas horizontais ia ua
		float disH = 1000000, hx = PlayX, hy = PlayY;
		float aTan = -1/tan(ra);
		
		if(ra > PI)//Oias para baixo
		{
			ry = (((int) PlayY >> 6) << 6) - 0.0001;
			rx = (PlayY - ry) * aTan + PlayX;
			yo = -64;
			xo = -yo * aTan;
		}
		if(ra < PI)//Oias para cima
		{
			ry = (((int) PlayY >> 6) << 6) +64;
			rx = (PlayY - ry) * aTan + PlayX;
			yo = 64;
			xo = -yo * aTan;			
		}
		if(ra == 0 || ra == PI) //Oiando pros lados
		{
			rx = PlayX;
			ry = PlayY;
			dof = 8;
		}
		while(dof < 8)
		{
			mx = (int)(rx)>>6;
			my = (int)(ry)>>6; 
			mp = my*mapX+mx;
			if(mp > 0 && mp < mapX*mapY && map[mp] == 1) //Encosto na parece patrão
			{
				hx = rx; 
				hy = ry;
				disH = dist(PlayX, PlayY, hx, hy, ra);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}		

		dof = 0; //chechando as vwerical
		float disV = 1000000, vx = PlayX, vy = PlayY;
		float nTan = -tan(ra);
		
		if(ra > P2 && ra < P3)//Oias para esquerdiha
		{
			rx = (((int) PlayX >> 6) << 6) - 0.0001;
			ry = (PlayX - rx) * nTan + PlayY;
			xo = -64;
			yo = -xo * nTan;
		}
		if(ra < P2 || ra > P3)//Oias para direitas
		{
			rx = (((int) PlayX >> 6) << 6) + 64;
			ry = (PlayX - rx) * nTan + PlayY;
			xo = 64;
			yo = -xo * nTan;			
		}
		if(ra == 0 || ra == PI) //Oiando pra cima ou pra baixo
		{
			rx = PlayX;
			ry = PlayY;
			dof = 8;
		}
		while(dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6; 
			mp = my*mapX+mx;
			if(mp > 0 && mp < mapX*mapY && map[mp] == 1) //Encosto na parece patrão
			{
				vx = rx; 
				vy = ry;
				disV = dist(PlayX, PlayY, vx, vy, ra);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		if(disV < disH)
		{
			rx = vx;
			ry = vy;
			disT = disV;
			glColor3f(0.7, 0, 0);
		}
		if(disH < disV)
		{
			rx = hx;
			ry = hy;
			disT = disH;
			glColor3f(0.9, 0, 0);
		}
		
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(PlayX, PlayY);
		glVertex2i(rx, ry);
		glEnd();
		
		//Desenhar os 3d paredes loucas
		float ca = pAng - ra;
		if(ca < 0) 
		{
			ca += 2*PI;
		}
		if(ca > 2*PI)
		{
			ca -= 2*PI;
		}
		
		disT = disT * cos(ca);
		
		float lineH = (mapS * 320)/(disT);
		if(lineH > 320)
		{
			lineH = 320; //tamanho da linha
		}
		
		float lineO = 160 - lineH/2;
		
		glLineWidth(8);
		glBegin(GL_LINES);
		glVertex2i(r * 8 + 530, lineO);
		glVertex2i(r * 8 + 530, lineH + lineO);
		glEnd();
		
		ra += DR;
		if(ra < 0) 
		{
			ra += 2*PI;
		}
		if(ra > 2*PI)
		{
			ra -= 2*PI;
		}
	}	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawPlayer();
	DrawRays2D();
	glutSwapBuffers();
}

void buttons(unsigned char key, int X, int Y)
{
	if(key == 'a') 
	{
		pAng -= 0.1;
		if(pAng < 0)
		{
			pAng += 2*PI;
		}
		pDeltax = cos(pAng) * 5; pDeltay = sin(pAng) * 5;
	}
	if(key == 'd')
	{
		pAng += 0.1;
		if(pAng > 2 * PI)
		{
			pAng -= 2*PI;
		}
		pDeltax = cos(pAng) * 5; pDeltay = sin(pAng) * 5;
	}
	if(key == 'w')
	{
		PlayX += pDeltax;
		PlayY += pDeltay;
	}
	if(key == 's')
	{
		PlayX -= pDeltax;
		PlayY -= pDeltay;
	}
	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,1024,510,0);
	PlayX = 400, PlayY = 200; pDeltax = cos(pAng) * 5; pDeltay = sin(pAng) * 5;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024,510);
	glutCreateWindow("Peroca");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);	
	glutMainLoop();
}
