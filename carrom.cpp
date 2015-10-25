#include<iostream>
#include<stdio.h>
#include<GL/glut.h>
#include<cmath>
#include<sys/time.h>
#include<string.h>

using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

void drawScene();
void update();
void initRendering();
void handleResize(int w, int h);
void handleKeyPress1(unsigned char key, int x, int y);
void handleKeyPress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void myMouse(int b, int s, int x, int y) ;
void motion( int x, int y );
void checkTime(int value);
void writeText(char* text, float x, float y, float z);

class Coins
{
	public:
		double x;
		double y;
		double vx;
		double vy;
		double v;
		Coins(double vel_x, double vel_y);
		void update_pos(double x1, double y1);
		void update_vel(double vel_x, double vel_y);
};

Coins::Coins(double vel_x, double vel_y)
{
	vx = vel_x;
	vy = vel_y;
	v = sqrt(vx * vx + vy * vy);
}

void Coins::update_pos(double x1, double y1)
{
	x = x1;
	y = y1;
}

void Coins::update_vel(double vel_x, double vel_y)
{
	vx = vel_x;
	vy = vel_y;
	v = sqrt(vx * vx + vy * vy);
}

class striker : public Coins
{
	public:
		double rad;
		double fvel;
		double angle;
		striker(double vel_x, double vel_y, double radius):Coins(vx, vy),rad(radius)
		{
			vx = vel_x;
			vy= vel_y;
			angle = 0.0f;
			fvel = 0.01f;
			rad = radius;
			x = 0;
			y = -0.96f;
		}
		void reinit(void);
		void set_fake(double v);
		void set_ang(double ang);
		void show_ang(void);
		void showv(void);
		void shoot(void);
		void update();
};

void striker::reinit(void)
{
	update_pos(0.0f, -0.96f);
	update_vel(0.0f, 0.0f);
	fvel = 0.01f;
	angle = 0.0f;
}

void striker::set_fake(double v)
{
	fvel = v;
}

void striker::set_ang(double ang)
{
	angle = ang;
}

void striker::show_ang(void)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glColor3f(0.8, 0.0, 0.0);
	glLineWidth(2.5);
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(x, y, -6.0f);
	glVertex3f(x + 2.0f * sin(DEG2RAD(angle)), y + 2.0f * cos(DEG2RAD(angle)), -6.0f);
	glEnd();
	glutSwapBuffers();
}

void striker::showv(void)
{
	glColor3f(0.8, 0.0, 0.0);
	glLineWidth(2.5);
	double i;
	i = fvel;

	writeText("Speed", -3.40f, 1.0f, -8.0f);

	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(2.5);

	glBegin(GL_LINES);
	while(i > 0.0f)
	{
		glVertex3f(-1.5f, -0.0f + i * 10, -5.0f);
		glVertex3f(-2.0f, -0.0f + i * 10, -5.0f);
		i -= 0.01f;
	}
	glEnd();
}

void striker::shoot(void)
{
	update_vel(fvel * sin(DEG2RAD(angle)), fvel * cos(DEG2RAD(angle)));
}

striker* Striker = new striker(0.0f, 0.0f, 0.12f);

class Board
{
	protected:
		double box_len;
		double theta;
	public:
		double rad;
		Board();
		void drawSquare(float len);
		void drawBall(float rad);
		void drawCircle(float rad);
		void drawBoard();
};

Board::Board()
{
	box_len = 8.0f;
	theta = 0;
	rad = 0.075f;
}

void Board::drawSquare(float len)
{
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
}

void Board::drawBall(float rad)
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++)
	{
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
	}
	glEnd();
}

void Board::drawCircle(float rad)
{
	glBegin(GL_LINE_STRIP);
	for (int i=0; i < (360); i++)
	{
		float degInRad = DEG2RAD(i);
		glVertex2f(cos(degInRad) * rad, sin(degInRad) * rad);
	}
	glEnd();
}

void Board::drawBoard()
{
	int i,j;
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glColor3f((139.0f/255.0f), (69.0f/255.0f), (19.0f/255.0f));
	drawSquare(6.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f , -8.0f);
	glColor3f((245.0f/255.0f), (222.0f/255.0f), (179.0f/255.0f));
	drawSquare(4.0f);
	glPopMatrix();
	// Pockets

	for(i=-1;i<=1;i+=2)
	{
		for(j=-1;j<=1;j+=2)
		{
			glPushMatrix();
			glTranslatef(1.35f * i, 1.35f * j, -6.0f);
			glColor3f((139.0f/255.0f), (119.0f/255.0f), (101.0f/255.0f));
			drawBall(2.10f * rad);
			glPopMatrix();
			glPushMatrix();	
			glColor3f((139.0f/255.0f), (69.0f/255.0f), (19.0f/255.0f));
			glTranslatef(-1.15f * i, -0.95f * j, -7.0f);
			drawBall(3 * rad / 2);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-0.95f * i, -1.15f * j, -7.0f);	
			drawBall(3 * rad / 2);
			glPopMatrix();
		}
	}

	//     Line Decorn
	glColor3f((139.0f/255.0f), (69.0f/255.0f), (19.0f/255.0f));
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(-1.25f, -1.13f + 3 * rad, -7.0f);
	glVertex3f(-1.25f, 1.13f - 3 * rad, -7.0f);
	glVertex3f(1.25f, -1.13f + 3 * rad, -7.0f);
	glVertex3f(1.25f, 1.13f - 3 * rad, -7.0f);
	glVertex3f( 1.13f - 3 * rad, -1.25f, -7.0f);
	glVertex3f(-1.13f + 3 * rad, -1.25f, -7.0f);
	glVertex3f(1.13f - 3 * rad, 1.25f, -7.0f);
	glVertex3f(-1.13f + 3 * rad, 1.25f, -7.0f);
	glEnd();

	//center decorn
	glPushMatrix();
	glTranslatef(-0.0f, 0.0f, -7.0f);
	glColor3f((139.0f/255.0f), (69.0f/255.0f), (19.0f/255.0f));
	glLineWidth(2.5);
	drawCircle(5 * rad);
	drawCircle(4 * rad);
	drawCircle(9 * rad);
	drawCircle(10 * rad);
	glPopMatrix();
}

class game : public Board
{
	public:
		int n_coins;
		int score;
		double friction;
		int color;
		int count;
		Coins* queen;
		Coins* white[10];
		Coins* black[10];
		game(int coins, double frict, int points);
		void initialiseCoins();
		void loadCoins();
};

game::game(int coins, double frict, int points)
{
	Board();
	n_coins = coins;
	score = points;
	friction = frict;
	queen = new Coins(0, 0);
	int i;
	for(i=0;i<10;++i)
		white[i] = new Coins(0, 0);
	for(i=0;i<10;++i)
		black[i] = new Coins(0, 0);
	color = 0;
	count = 0;
	initialiseCoins();
}

void game::initialiseCoins()
{
	int i,rAngle=0;
	
	queen->update_pos(0,0);
	for(i=0;i<3;i++)
	{
		white[i]->update_pos(2.1 * rad * sin(DEG2RAD(rAngle)), 2.1 * rad * cos(DEG2RAD(rAngle)));
		rAngle += 60;	
		black[i]->update_pos(2.1 * rad *sin(DEG2RAD(rAngle)), 2.1 * rad * cos(DEG2RAD(rAngle)));
		rAngle += 60;
	}
	rAngle = 30;
	for(;i<6;i++)
	{
		white[i]->update_pos(3.8 * rad * sin(DEG2RAD(rAngle)), 3.8 * rad * cos(DEG2RAD(rAngle)));
		rAngle += 60;
		black[i]->update_pos(3.8 * rad * sin(DEG2RAD(rAngle)), 3.8 * rad * cos(DEG2RAD(rAngle)));
		rAngle += 60;
	}
}

void game::loadCoins()
{
	int i;

	glPushMatrix();
	glTranslatef(queen->x, queen->y, -6.0f);
	glColor3f(1.0f, 0.0f, 0.1f);
	drawBall(rad);
	glPopMatrix();

	for(i=0;i<n_coins;i++)
	{
		glPushMatrix();
		glTranslatef(white[i]->x, white[i]->y, -6.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		drawBall(rad);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(black[i]->x, black[i]->y, -6.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawBall(rad);
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(Striker->x, Striker->y, -6.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawBall(Striker->rad);
	glPopMatrix();
}

game* Game = new game(0, 0.0001, 30);
int active = 0, up = 0, yes = 0, rememqueen = 0, reinitialise = 0;
struct timeval tv_prev, tv_now, tv_init;
long long int last = 0;


void striker::update()
{
	if(v != 0.0f)
	{
		if(fabs(vx) > fabs(Game->friction * vx / v))
			vx -= Game->friction * vx / v;
		else 
			vx = 0.0f;

		if(fabs(vy) > fabs(Game->friction * vy / v))
			vy -= Game->friction * vy / v;
		else
			vy = 0.0f;

		v = sqrt(vx * vx + vy * vy);
		if(v < 0.01f)
			update_vel(0.0f, 0.0f);
		if(v)
			reinitialise = 1;
		x += vx;
		y += vy;
		if(x > 1.39f || x < -1.39f)
			vx *= -1;

		if(y > 1.39f || y < -1.39f)
			vy *= -1;
	}
	if(((fabs(x) - 1.35f) * (fabs(x) - 1.35f)) + ((fabs(y) - 1.35f) * (fabs(y) - 1.35f)) <= (4 * Game->rad * Game->rad))
	{
		update_pos(5.0f, 5.0f);
		update_vel(0.0f, 0.0f);
		Game->score -= 10;
	}
}

void writeText(char* text, float x, float y, float z)
{
	char* a;
	glPushMatrix();
	glTranslatef(x, y ,z);
	glLineWidth(2.0);
	glScalef(0.45/152.38, 0.45/152.38, 1/152.38);
	for (a=text;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,*a);
	glPopMatrix();
}

void disp_score(void)
{
	long long int i = 0, sc = Game->score;
	int j = 0;
	char* a;
	char score[100] = "Score: ";
	char s[1000];
	glColor3f(01.0, 0.30, 0.00);
	if(Game->n_coins != Game->count || Game->n_coins == 0)
		gettimeofday(&tv_now, NULL);
	glPushMatrix();
	
	glLineWidth(1.5);
	glTranslatef(-7.0f, -1.0f , -10.0f);
	glScalef(0.30/152.38, 0.30/152.38, 1/152.38);
	for(a=score;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);
	if(sc<0)
	{
		sc *= -1;
		a = "-\0";
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);
	}

	while(sc)
	{
		score[i] = sc % 10 + '0';
		sc /= 10;
		i++;
	}

	if(i == 0)
		score[i++] = '0';
	while(i--)
		s[j++] = score[i];
	s[j] = '\0';	
	score[i] = '\0';
	for(a=s;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);
	glPopMatrix();
	glPushMatrix();
	char x[19]= "Time Elapsed: ";
	glTranslatef(-7.0f, -1.5f, -10.0f);
	glScalef(0.30/152.38, 0.30/152.38, 1/152.38);
	for (a=x;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);

	i = 0;
	sc = tv_now.tv_sec - tv_init.tv_sec;
	while(sc)
	{
		score[i] = sc % 10 + '0';
		sc /= 10;
		i++;
	}
	j = 0;
	if(i == 0)
		score[i++] = '0';
	while(i--)
		s[j++] = score[i];
	s[j++] = 's';	
	s[j] = '\0';	

	for(a=s;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *a);
	glPopMatrix();

	glPushMatrix();
	char y[19]= "Coins pocketed: ";
	glTranslatef(-7.0f, -2.0f, -10.0f);
	glScalef(0.30/152.38, 0.30/152.38, 1/152.38);
	for(a=y;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);

	i = 0;
	sc = Game->count;

	while(sc)
	{
		score[i] = sc % 10 + '0';
		sc /= 10;
		i++;
	}
	j = 0;
	if(i == 0)
		score[i++] = '0';
	while(i--)
		s[j++] = score[i];
	s[j] = '\0';	

	for(a=s;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *a);
	glPopMatrix();
	if(rememqueen == -5 || rememqueen == 1)
	{
		glPushMatrix();
		char z[19]= "Queen pocketed!";
		glTranslatef(-7.0f, -2.5f, -10.0f);
		glScalef(0.30/152.38, 0.30/152.38, 1/152.38);
		for(a=z;*a!='\0';a++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);
		glPopMatrix();
	}

	glPushMatrix();
	char* b;
	if(Game->color == 1)
		b = "Your color: White";
	else
		b = "Your color: Black";
	glTranslatef(-6.0f, 2.5f, -10.0f);
	glScalef(0.30/152.38, 0.30/152.38, 1/152.38);
	for(a=b;*a!='\0';a++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *a);
	glPopMatrix();
}

void start(void)
{
	if(Game->n_coins == 0)
	{
		writeText("Enter the number of coins:", -3.40f, 1.0f, -8.0f);
		writeText("3", -3.20f, 0.0f, -8.0f);
		writeText("4", -3.20f, -0.50f, -8.0f);
		writeText("5", -3.20f, -1.0f, -8.0f);
		writeText("6", -3.20f, -1.50f, -8.0f);
	}
	else if(Game->color == 0)
	{
		writeText("Choose a Color:", -3.40f, 1.0f, -8.0f);
		writeText("b : Black", -3.20f, 0.0f, -8.0f);
		writeText("w : White", -3.20f, -0.50f, -8.0f);
	}
	else
	{
		writeText("Surface Friction:", -3.40f, 1.0f, -8.0f);
		writeText("r : Rough", -3.20f, 0.0f, -8.0f);
		writeText("m : Medium", -3.20f, -0.50f, -8.0f);
		writeText("s : Smooth", -3.20f, -1.0f, -8.0f);
	}
}

void result(void)
{
	writeText("Game Over!", 1.0, 0.0, -10.0);
	disp_score();
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	if(!yes)
		start();
	else if(Game->count == Game->n_coins)
		result();
	else
	{
		Game->drawBoard();
		Game->loadCoins();
		disp_score();
	}
	glutSwapBuffers();
}

void friction_apply(Coins* c)
{
	if(c->v)
	{
		if(fabs(c->vx) > fabs(Game->friction * (c->vx / c->v)) )
			c->vx -= Game->friction * (c->vx / c->v);
		else
			c->vx = 0.0f;
		if(fabs(c->vy) > fabs(Game->friction * (c->vy / c->v)) )
			c->vy -= Game->friction * (c->vy / c->v);
		else
			c->vy = 0.0f;
		c->v = sqrt((c->vx * c->vx) + (c->vy * c->vy));
		if(c->v < 0.01f)
			c->update_vel(0.0f, 0.0f);
	}
}

void check_pocket(int type, Coins* c)
{
	if(((fabs(c->x) - 1.35f) * (fabs(c->x) - 1.35f)) + ((fabs(c->y) - 1.35f) * (fabs(c->y) - 1.35f)) <= (4 * Game->rad * Game->rad))
	{	
		c->update_vel(0.0f,0.0f);
		c->update_pos(5.0f, 5.0f);
		if(Game->color == type)
		{
			if(Game->count == Game->n_coins - 1 && rememqueen != -5)
				c->update_pos(0.0f, 0.0f);
			else
			{
				Game->score += 10;
				Game->count++;
			}
		}
		else if(type == 1 || type == 2)
		{
			Game->score -= 5;
			c->update_pos(0.0f, 0.0f);
		}
		else if(type == 3)
		{
			rememqueen = -5;
			Game->score += 50;
		}
	}
}

void change(Coins* c)
{
	c->x += c->vx;
	c->y += c->vy;
	if(fabs(c->x) > 1.39f)
	{
		c->vx *= -1;
		c->x += c->vx;
	}
	if(fabs(c->y) > 1.39f)
	{
		c->vy *= -1;
		c->y += c->vy;	
	}
}

void collision(Coins* c1, Coins* c2, double m1, double m2)
{
	float lx, ly, u1, u2, v1, v2, q11, q22, z, e = 1;
    lx = c1->x - c2->x;
    ly = c1->y - c2->y;
    z = sqrt(lx * lx + ly * ly);
    u1 = (lx * c1->vx + ly * c1->vy) / z;
    u2 = (lx * c2->vx + ly * c2->vy) / z;
    v2 = (u2 * (m2 - e * m1) + (1 + e) * m1 * u1) / (m1 + m2);
    v1 = (u1 * (m1 - e * m2) + (1 + e) * m2 * u2) / (m1 + m2);
    q11 = (ly * c1->vx - lx * c1->vy) / z;
    q22 = (ly * c2->vx - lx * c2->vy) / z;
    c1->vx = (v1 * lx + q11 * ly) / z;
    c1->vy = (v1 * ly - q11 * lx) / z;
    c2->vx = (v2 * lx + q22 * ly) / z;
    c2->vy = (v2 * ly - q22 * lx) / z;
    c1->update_vel(-c1->vx, -c1->vy);
    c2->update_vel(-c2->vx, -c2->vy);
}

void reinit_check(void)
{
	int i = 0;

	for(i=0;i<Game->n_coins;i++)
	{
		if((fabs(Game->white[i]->x) > 1.39f && Game->white[i]->x != 5.0f) || (fabs(Game->white[i]->y) > 1.39f  && Game->white[i]->y != 5.0f))
		{
			Game->white[i]->update_pos(0.0f, 0.0f);
			Game->white[i]->update_vel(0.0f, 0.0f);
		}
		if((fabs(Game->black[i]->x) > 1.39f && Game->black[i]->x != 5.0f ) || (fabs(Game->black[i]->y) > 1.39f  && Game->black[i]->y != 5.0f))
		{
			Game->black[i]->update_pos(0.0f, 0.0f);
			Game->black[i]->update_vel(0.0f, 0.0f);
		}
	}
	for(i=0;i<Game->n_coins;i++)
	{
		if(Game->white[i]->v || Game->black[i]->v)
		{
			reinitialise = 1;
			break;
		}
	}

	if(i == Game->n_coins && reinitialise && !Game->queen->v && !Striker->v)
	{
		reinitialise = 0;
		if(rememqueen > 0  )
			rememqueen -= 1;
		Striker->reinit();
	}
}

int check_collision(Coins* c1, Coins* c2, int i)
{
	double dist = sqrt(pow(c1->x - c2->x, 2) + pow(c1->y - c2->y, 2));
	double dist2 = sqrt(pow(c1->x - c2->x + c1->vx - c2->vx, 2) + pow(c1->y - c2->y + c1->vy - c2->vy, 2));
	double rad;
	if(i == 2)
		rad = 2 * Game->rad;
	else
		rad = Game->rad + Striker->rad;
	if(dist < dist2 && dist <= rad  && (c1->v || c2->v))
		return 1;
	else
		return 0;
}

void collide(void)
{
	int i, j;
	for(i=0;i<Game->n_coins;i++)
	{
		if(check_collision(Game->white[i], Striker, 1))
			collision(Game->white[i], Striker, 1, 2);
		if(check_collision(Game->black[i], Striker, 1))
			collision(Game->black[i], Striker, 1, 2);
	}
	if(check_collision(Game->queen, Striker, 1))
		collision(Game->queen, Striker, 1, 2);

	for(i=0;i<Game->n_coins;i++)
	{
		if(check_collision(Game->white[i], Game->queen, 2))
			collision(Game->white[i], Game->queen, 1, 1);
		if(check_collision(Game->queen, Game->black[i], 2))
			collision(Game->queen, Game->black[i], 1, 1);
	
		for(j=0;j<Game->n_coins;j++)
		{
			if(i < j)
			{
				if(check_collision(Game->white[i], Game->white[j], 2))
					collision(Game->white[i], Game->white[j], 1, 1);
				if(check_collision(Game->black[i], Game->black[j], 2))
					collision(Game->black[i], Game->black[j], 1, 1);
			}
			if(check_collision(Game->white[i], Game->black[j], 2))
				collision(Game->white[i], Game->black[j], 1, 1);
		}
	}
}

void update()
{
	int i;
	for(i=0;i<Game->n_coins;i++)
	{
		friction_apply(Game->white[i]);
		change(Game->white[i]);
		check_pocket(1, Game->white[i]);
		friction_apply(Game->black[i]);
	    change(Game->black[i]);
		check_pocket(2, Game->black[i]);
	}
	friction_apply(Game->queen);
	change(Game->queen);
	check_pocket(3, Game->queen);

	Striker->update();
	
	reinit_check();
	collide();
}

void checkTime(int val)
{
	gettimeofday(&tv_now, NULL);
	if(abs(tv_now.tv_usec - tv_prev.tv_usec) >= 15)
	{
		tv_prev.tv_usec = tv_now.tv_usec;
		if((tv_now.tv_sec - tv_init.tv_sec) % 5 == 0 && tv_now.tv_sec - tv_init.tv_sec != last)	
		{	
			Game->score -= 1;
			last = tv_now.tv_sec - tv_init.tv_sec;
		}
		update();
	}
	if(Game->n_coins > Game->count  || Game->n_coins == 0)
		glutTimerFunc(15, checkTime, 0);
}

void handleKeyPress1(unsigned char key, int x, int y)		
{
	if(yes)
	{
		if (key == 27)
			Striker->reinit();
		else if (key == ' ' && Striker->vx == 0 && Striker->vy == 0 && Striker->y == -0.96f)
			Striker->shoot();
		else if (key == 'd' && Striker->vx == 0 && Striker->vy == 0 && Striker->y == -0.96f)
		{
			Striker->angle += 10;
			if(Striker->angle > 90)
				Striker->angle = 90;
			Striker->show_ang();
		}
		else if (key == 'a' && Striker->vx == 0 && Striker->vy == 0 && Striker->y == -0.96f)
		{
			Striker->angle -= 10;;
			if(Striker->angle < -90)
				Striker->angle = -90;
			Striker->show_ang();
		}
	}
	else
	{
		if(key >= '3' && key <= '6')
			Game->n_coins = key - '0';
		else if(key == 'w' || key == 'b' && Game->n_coins)
		{
			gettimeofday(&tv_now, NULL);
			int t = tv_now.tv_sec;
			while(abs(tv_now.tv_sec - t ) < 2)
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				if(key == 'w')
				{
					Game->color = 1;
					writeText("You took White", -3.40f, -1.0f, -8.0f);
				}
				else
				{
					Game->color = 2;
					writeText("You took Black", -3.40f, -1.0f, -8.0f);
				}	
				gettimeofday(&tv_now, NULL);
				glutSwapBuffers();
			}
		}
		else if(key == 's')
			yes = 1;
		else if(key == 'm')
		{
			yes = 1;
			Game->friction *= 2;
		}
		else if(key == 'r')
		{
			yes = 1;
			Game->friction *= 4;
		}
		gettimeofday(&tv_prev, NULL);
		gettimeofday(&tv_init, NULL); 
	}
}

double dist(Coins* c)
{
	return sqrt(pow(c->x - Striker->x, 2) + pow(c->y - Striker->y, 2));
}

void handleKeyPress2(int key, int x, int y)
{
	int i;
	if(Striker->vx == 0 && Striker->vy == 0 && Striker->y == -0.96f)
	{
		if(key == GLUT_KEY_LEFT)
		{
			Striker->x -= 0.1;
			for(i=0;i<Game->n_coins;i++)
			{
				if(dist(Game->white[i]) < Game->rad + Striker->rad && fabs(Game->white[i]->y) + Game->rad > 0.96f &&  fabs(Game->white[i]->y) - Game->rad < 0.96f )
					Striker->x -= 0.1;
				if(dist(Game->black[i]) < Game->rad + Striker->rad && fabs(Game->black[i]->y) + Game->rad > 0.96f &&  fabs(Game->black[i]->y) - Game->rad < 0.96)
					Striker->x -= 0.1;
			}

			if(Striker->x < -0.96f + Striker->rad)
				Striker->x = -0.96f + Striker->rad;
		}
		if(key == GLUT_KEY_RIGHT)
		{
			Striker->x += 0.1;
			for(i=0;i<Game->n_coins;i++)
			{
				if(dist(Game->white[i]) < Game->rad + Striker->rad && fabs(Game->white[i]->y) + Game->rad > 0.96f &&  fabs(Game->white[i]->y) - Game->rad < 0.96f)
					Striker->x += 0.1;
				if(dist(Game->black[i]) < Game->rad + Striker->rad && fabs(Game->black[i]->y) + Game->rad > 0.96f &&  fabs(Game->black[i]->y) - Game->rad < 0.96)
					Striker->x += 0.1;
			}
			if(Striker->x > 0.96f - Striker->rad)
				Striker->x = 0.96f - Striker->rad;
		}
		if(key == GLUT_KEY_UP)
		{
			Striker->fvel += 0.01;
			if(Striker->fvel > 0.06f)
				Striker->fvel = 0.06f;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			Striker->showv();
			glutSwapBuffers();
		}
		if(key == GLUT_KEY_DOWN)
		{
			Striker->fvel -= 0.01;
			if(Striker->fvel < 0.01f )
				Striker->fvel = 0.01f;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			Striker->showv();
			glutSwapBuffers();

		}
	}
}

void myMouse(int b, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		if (b == GLUT_RIGHT_BUTTON)
			active=1;
		if (b == GLUT_LEFT_BUTTON && Striker->vx == 0 && Striker->vy == 0 && Striker->y == -0.96f )
		{
			active = -1;
			up = y; 
			Striker->angle = atan(((float)(x-455) / 100.0f - Striker->x) / ((float)(256.5 - y) / 100.0f - Striker->y)) * 180.0f / PI;
			Striker->fvel += 0.010;
		}
	}
	if(s == GLUT_UP)
	{
		if(active == -1)
			Striker->shoot();
		active = 0;
	}	
}

void handleMouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			;
		else if (button == GLUT_RIGHT_BUTTON)
			Striker->x = (float)(x - 455) / (float)85;
	}
}

void motion(int x, int y)
{
	if(active == 1)
	{
		Striker->x =  (float)(x - 456.5) / (float)105;
		if(x > 541)
			Striker->x = (float)(541 - 465.5) / float(90);
		else if (x <373)
			Striker->x = (float)(373 - 465.5) / float(110);
	}

	if(active == -1)
	{
		if(y < up)
			Striker->fvel += 0.010;
		else
			Striker->fvel -= 0.010;

		if(Striker->fvel > 0.08f)
			Striker->fvel = 0.08f;

		if(Striker->fvel < 0.01f)
			Striker->fvel = 0.01f;

		up = y;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Striker->showv();
		Striker->show_ang();
		glutSwapBuffers();
		glutPostRedisplay();
	}
}

void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initRendering()
{
	glEnable(GL_DEPTH_TEST);   
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Carrom");  // Setup the window
	initRendering();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeyPress1);
	glutSpecialFunc(handleKeyPress2);
	glutMouseFunc(myMouse);
	glutMotionFunc(motion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(15, checkTime, 0);
	glutMainLoop();
	return 0;
}
