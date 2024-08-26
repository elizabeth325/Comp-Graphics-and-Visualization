#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

void processInput(GLFWwindow *window);

enum BRICKTYPE
{
	REFLECTIVE,
	DESTRUCTABLE
};
enum ONOFF
{
	ON,
	OFF
};

class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	ONOFF onoff;
	int hitsTaken;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt;
		x = xx;
		y = yy, width = ww;
		red = rr, green = gg, blue = bb;
		onoff = ON;
		hitsTaken = 0;
	};

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
};

class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.03;
	ONOFF onoff;
	int hitsTaken;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left

	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
		onoff = ON;
		hitsTaken = 0;
	}

	//circle detection
	//Added another function for detecting if circles touch each other
	//param circle - is the circle its looking at
	void CheckCollisionCircle(Circle *cir)
	{
		// for detecting if it touches
		if((x > cir->x - cir->radius && x <= cir->x + cir->radius) && (y > cir->y - cir->radius && y <= cir->y + cir->radius) && (onoff == ON && cir->onoff == ON))
		{	
			//if hit that many times get rid of cirlce
			if(hitsTaken > 50){
				cir->onoff = OFF;
			}
			//Count the fact it got touched
			else{
				hitsTaken++;
			}
		}
	}

	void CheckCollision(Brick *brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.03;
				y = y + 0.04;
			}
		}
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				if (brk->hitsTaken >= 3)
				{
					brk->onoff = OFF;
				}
				else
				{
					brk->hitsTaken++;
					direction = GetRandomDirection();
					x = x + 0.03;
					y = y + 0.04;
				}
			}
		}
	}

	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	void MoveOneStep()
	{
		if (direction == 1 || direction == 5 || direction == 6) // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
				speed += 0.1;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 2 || direction == 5 || direction == 7) // right
		{
			if (x < 1 - radius)
			{
				x += speed;
				speed += 0.1;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 3 || direction == 7 || direction == 8) // down
		{
			if (y < 1 - radius)
			{
				y += speed;
				speed += 0.1;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 4 || direction == 6 || direction == 8) // left
		{
			if (x > -1 + radius)
			{
				x -= speed;
				speed += 0.1;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
	}

	void DrawCircle()
	{
		if(onoff == ON){
			glColor3f(red, green, blue);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				float degInRad = i * DEG2RAD;
				glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
			}
			glEnd();
		}
	}
};

vector<Circle> world;

int main(void)
{
	srand(time(NULL));

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(480, 480, "8-2 Assignment", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// brick that are being drawn attributes
	Brick brick(REFLECTIVE, 0.5, -0.33, 0.2, 1, 1, 0);
	Brick brick2(DESTRUCTABLE, -0.5, 0.33, 0.2, 0, 1, 0);
	Brick brick3(DESTRUCTABLE, -0.5, -0.33, 0.2, 0, 1, 1);
	Brick brick4(REFLECTIVE, 0, 0, 0.2, 1, 0.5, 0.5);

	Brick brick5(REFLECTIVE, 0.8, -0.33, 0.2, 0.54, 0.24, 0.34);
	Brick brick6(DESTRUCTABLE, -0.8, 0.33, 0.2, 0.43, 0.56, 0.23);
	Brick brick7(DESTRUCTABLE, -0.6, -0.33, 0.2, 0.12, 0.11, 0.98);
	Brick brick8(REFLECTIVE, 0.3, 0.5, 0.2, 0.78, 0.45, 0.57);

	Brick brick9(REFLECTIVE, 0.1, -0.33, 0.2, 0.1, 0.1, 0);
	Brick brick10(DESTRUCTABLE, -0.1, 0.33, 0.2, 0.1, 0.1, 0);
	Brick brick11(DESTRUCTABLE, -0.7, -0.33, 0.2, 0, 0.1, 0.1);
	Brick brick12(REFLECTIVE, 0, 1, 0.2, 0.1, 0.1, 0.1);

	while (!glfwWindowShouldClose(window))
	{
		// Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);
		for (int i = 0; i < world.size(); i++)
		{
			//world[] is the circles on the screen
			//this is for checking for the collision for these circles
			world[i].CheckCollision(&brick);
			world[i].CheckCollision(&brick2);
			world[i].CheckCollision(&brick3);
			world[i].CheckCollision(&brick4);

			world[i].CheckCollision(&brick5);
			world[i].CheckCollision(&brick6);
			world[i].CheckCollision(&brick7);
			world[i].CheckCollision(&brick8);

			world[i].CheckCollision(&brick9);
			world[i].CheckCollision(&brick10);
			world[i].CheckCollision(&brick11);
			world[i].CheckCollision(&brick12);

			for(int j = 0; j < world.size(); j++)
			{
				if(i != j){
					world[i].CheckCollisionCircle(&world[j]);
				}
			}


			world[i].MoveOneStep();
			world[i].DrawCircle();
		}

		// brick that are being drawn
		brick.drawBrick();
		brick2.drawBrick();
		brick3.drawBrick();
		brick4.drawBrick();
		
		brick5.drawBrick();
		brick6.drawBrick();
		brick7.drawBrick();
		brick8.drawBrick();

		brick9.drawBrick();
		brick10.drawBrick();
		brick11.drawBrick();
		brick12.drawBrick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		double r, g, b;
		r = rand() / 10000;
		g = rand() / 10000;
		b = rand() / 10000;
		Circle B(0, 0, 02, 2, 0.05, r, g, b);
		world.push_back(B);
	}
}