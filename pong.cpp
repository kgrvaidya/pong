#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"
#include<ctype.h>
#pragma comment(lib, "OpenGL32.lib")

int flag=0;

// window size and update rate (60 fps)
int width = 500;
int height = 200;
int interval = 1000 / 60;
int x,y;

float tx=0.0,ty=0.0,tz=0.0;
float ball_x=0.5,ball_y=0.0,ball_z=0.0;



int score_left = 0;
int score_right = 0;


// rackets
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// right racket position
float racket_right_x = width - racket_width - 10.0f;
float racket_right_y = 50;

// keycodes
#define VK_W 0x57
#define VK_S 0x53
#define VK_Q 0x51
#define VK_ENT 0x0D
#define VK_LMB 0x02

#define PI 3.1416
// ball
float ball_pos_x = width / 2.0f;
float ball_pos_y = height / 2.0f;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
float ball_speed = 2.0f;
float ball_radius = 6.0f;




void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawBall(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawText(float x, float y,float z, std::string text) {
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text.c_str());
}

std::string int2str(int x) {
    // converts int to string
    std::stringstream ss;
    ss << x;
    return ss.str( );
}


void drawball(float r, float g, float b){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(ball_pos_x,ball_pos_y,0.0f);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(r,g,b);
	glVertex2f(0.0,0.0);
	int numsegments=100;
	float angle;
	for(int i=0;i<=numsegments;i++){
		angle=i*2.0f*PI/numsegments;
		glVertex2f(cos(angle)*ball_radius,sin(angle)*ball_radius);
	}
	glEnd();

}




void frontscreen(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glClearColor(1,1,0.4,1);

	glColor3f(1,0,0);
	drawText(width/2.2,height-100,0.0,"PONG");
	glColor3f(1,0.1,1);
	drawText(width/3.25,height-180,0.0,"PRESS RIGHT MOUSE BUTTON TO START");
	glutSwapBuffers();


}


void draw2(){

// clear (has to be done at the beginning)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glClearColor(0,0,0,1);

    // draw rackets
	glColor3f(1.0f,1.0f,1.0f);
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);


   // draw score
	glColor3f(1.0f,1.0f,1.0f);
	drawText(width / 2.35 - 10, height - 15,0,"Player 1 - " +int2str(score_left) + " :: " + int2str(score_right)+" - Player 2");



   // draw ball

    if(ball_pos_x<(width/2))
	{
		drawball(1,1,0);
	}
	else

	{
		drawball(1,0,1);
	}






    // swap buffers (has to be done at the end)
    glutSwapBuffers();

	}




void draw() {

	switch(flag){
		case 0 : frontscreen();  break;
		case 1 : ;
		case 2 : draw2(); break;
		default : exit(0);
	}

}



void keyboard() {
    // left racket
    if (GetAsyncKeyState(VK_W)) racket_left_y += racket_speed;
    if (GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;

    // right racket
    if (GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
    if (GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;

	//Quit
	 if (GetAsyncKeyState(VK_Q)) exit(0);
	 if (GetAsyncKeyState(VK_LMB)) flag=2;
	 if (GetAsyncKeyState(VK_ENT)) flag=1;
}

void vec2_norm(float& x, float &y) {
        // sets a vectors length to 1 (which means that x + y == 1)
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
    }

void updateBall() {

	//Wait till user presses Enter to start the Game
	if(flag==1){
	// fly a bit
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;

    // hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = t;
		ball_speed+=0.4f;
    }

    // hit by right racket?
    if (ball_pos_x > racket_right_x &&
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = t;
		ball_speed+=0.4f;
    }

    // hit left wall?
    if (ball_pos_x < 0) {
        ++score_right;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = 0;
		ball_speed=2.0f;
//		left_live-=1;

    }

    // hit right wall?
    if (ball_pos_x > width) {
        ++score_left;
        ball_pos_x = width / 2.0;
        ball_pos_y = height / 2.0f;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = 0;
		ball_speed=2.0f;
//		right_live-=1;
    }

    // hit top wall?
    if (ball_pos_y > height) {
        ball_dir_y = -fabs(ball_dir_y); // force it to be negative
    }

    // hit bottom wall?
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y); // force it to be positive
    }

    // make sure that length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);
	}

}


void winner(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	if(score_left==5)
		drawText(width/2.5,height/2,0,"Player 1 is the winner ");
	   else
	    drawText(width/2.5,height/2,0,"Player 2 is the winner ");

	drawText(width/2.4,height/2.25,0, "To Quit, press Q\n");
	glutSwapBuffers();
	while(true)
	{
		if (GetAsyncKeyState(VK_Q)) exit(0);
	}
}



void update(int value) {

  // input handling
   keyboard();

   // update ball
   if(score_left<5 && score_right<5)
   {	   updateBall();
    // Call update() again in 'interval' milliseconds
   glutTimerFunc(interval, update, 0);


   // Redisplay frame
   glutPostRedisplay();
   }

   else
      {
		  glClear(GL_COLOR_BUFFER_BIT);
		  glClearColor(0,0,0,0);
	      winner();
		  if(flag==-1) exit(0);
	   }
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//  glutInitWindowSize(width, height);
	glutInitWindowSize(1280,760);
    glutCreateWindow("PONG");

    // Register callback functions
    glutDisplayFunc(draw);
    glutTimerFunc(interval, update, 0);

    // setup scene to 2d mode and set draw color to white
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);

    // start the whole thing
    glutMainLoop();
    return 0;
}
