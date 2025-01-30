#pragma once
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include "MovingLib.h"

#define p 3.14

//����� ����
class Fish_Food
{
private:
	point2d coord; // ���������� ����
	int t_angle;   // ����, �� ������� ��� ��������������
	int angle;     // ����, �� ������� ������ �����������
	bool t_check;  // �������� ��������� ���� ��������

public:
	bool isExist;  // ���������� �� ����

	Fish_Food() : isExist(false), coord(point2d(0, 1.2)), t_angle(0), angle(0), t_check(false) { }  // ����������� ������

	void Fish_render(point2d MouseCord, int angle_)     // ����� ���������� ����
	{
		if (MouseCord.x != NULL || MouseCord.y != NULL)  // �������� ������� ����� �� �����
		{
			coord = Grab_Object(coord, MouseCord);       // ����������� ���� �����
			t_angle = -90;                               // ������� ���� �� -90 �������� (�������� �������)
		}
		else
		{
			coord = Gravity(coord, -0.6, -0.03);         // ����� ������� ����������
			t_angle = angle_;                            // ������� ���� �� angle_
		}

		loopAnimation(angle, angle_ / 2, t_check);       // ����������� �������� �������� ����

		// ������ ������� ����
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(coord.x, coord.y, 0);
		glRotatef(t_angle + angle, 0, 0, 1);
		glBegin(GL_TRIANGLES);

		glColor3f(0, 0.4, 0.4);
		glVertex2f(-0.1, 0);
		glVertex2f(-0.25, -0.07);
		glVertex2f(-0.25, 0.07);

		glVertex2f(0.08, 0.08);
		glVertex2f(-0.07, 0.1);
		glVertex2f(-0.02, -0.02);

		glVertex2f(0.08, -0.08);
		glVertex2f(-0.07, -0.1);
		glVertex2f(-0.02, -0.02);

		glColor3f(0, 0.5, 0.4);
		glVertex2f(0.1, -0.1);
		glVertex2f(0.1, 0.1);
		glVertex2f(0.2, 0);

		glVertex2f(0.1, -0.1);
		glVertex2f(0.1, 0.1);
		glVertex2f(-0.2, 0);

		glEnd();
		// ����� ������� ����

		// ������ ������� ����� ����
		glColor3f(0, 0, 0);
		glPointSize(15);
		glBegin(GL_POINTS);

		glVertex2f(0.12, 0);

		glEnd();
		glPopMatrix();
		// ����� ������� ����� ����
	}

	// �����, ��� ���������� ������� ���� � ������, ���� �� �����
	void fish_Eated()
	{
		coord = point2d(0, 1.2);
		isExist = false;
	}

	// ��������� ��������� �������������� ����
	point2d fish_GetPoint()
	{
		return coord;
	}

	// ��������� ���� � ��������� ����� � ����
	void fish_RandomSpawn()
	{
		if (!isExist)
		{
			coord.x = (rand() % 20 - 10) * 0.1;
			isExist = true;
		}
	}
};

// ����� �������
class PENGUIN
{
private:
	point2d coord;  // ���������� �������������� ��������
	int cnt;        // ���������� �������������, �� ������� �� ��������� �����
	bool t_check;   // �������� ��������� ���� ��������
	int timer, time_limit;   // �������� ���������� ��� �������� � �� ������������ ��������
	float a;        // ���� ��������, �� ������� ��������� ��� ��������� �����

	int fat_body_m;    // ��������� ������� ���������
	double fat;        // ������� ���������� �������� ��������

public:
	PENGUIN() : coord(point2d(0, 0)), cnt(30), timer(0), fat_body_m(0), fat(0)  // ����������� ������ �� ���������
	{ 
		a = 2 * p / cnt; 
		t_check = false;
	}
	PENGUIN(point2d cord, int c) : coord(cord), cnt(c), timer(0), fat_body_m(0), fat(0)  // ����������� ������ � �����������
	{
		a = 2 * p / cnt;
		t_check = false;
	}

	void Pengu_Render(int t_limit, point2d MouseCord)       // ����� ������� ��������
	{
		time_limit = t_limit;

		if (fat < fat_body_m) fat += 0.01;      // ��������� ��������

		loopAnimation(timer, time_limit, t_check);     // �������� ���������� ��� ����������� ���������

		P_Black_Render(0.5, -timer * 0.0003);                        // ������ ������ ����� �������� (������ ����)
		P_White_Render(0.4, -timer * 0.0003, 0.02, MouseCord);		 // ������ ����� ����� �������� (�������� ����)
		P_Face_Render(0.05, -timer * 0.0003, 0.05, MouseCord);		 // ������ ���� ��������
		P_Arms_Render(0.05, -timer * 0.0003);						 // ������ ��� ��������
	}

	// ����� ������� ����� ����� ��������
	void P_White_Render(float rad, float delta, float diff, point2d MousePoint)
	{
		float x = coord.x, y = coord.y;

		// ������ ������� �����
		glColor3f(0.9, 0.9, 0.9);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, -0.3 + coord.y, 0);

		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(x, y);
		for (int i = -2; i < cnt; i++)
		{
			x = sin(a * i) * (rad + fat * 0.05);
			y = cos(a * i) * (rad + fat * 0.05);

			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();

		float x_c = 0, y_c = 0;

		MoveFace(x_c, y_c, MousePoint, diff);
		x = x_c, y = y_c;
		rad -= 0.1;

		// ������ �������� �����
		glColor3f(0.9, 0.9, 0.9);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, 0.2 + delta + coord.y, 0);

		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(x, y);
		for (int i = -2; i < cnt; i++)
		{
			x = x_c + sin(a * i) * rad;
			y = y_c + cos(a * i) * rad;

			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();
	}

	// ����� ������� ������ ����� ��������
	void P_Black_Render(float rad, float delta)
	{
		float x = 0, y = 0;

		// ������ ������� �����
		glColor3f(0.1, 0.1, 0.1);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, -0.3 + coord.y, 0);

		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(x, y);
		for (int i = -2; i < cnt; i++)
		{
			x = sin(a * i) * (rad + fat * 0.05);
			y = cos(a * i) * (rad + fat * 0.05);

			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();

		x = 0, y = 0;
		rad -= 0.1;

		// ������ �������� �����
		glColor3f(0.1, 0.1, 0.1);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, 0.2 + delta + coord.y, 0);

		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(x, y);
		for (int i = -2; i < cnt; i++)
		{
			x = sin(a * i) * rad;
			y = cos(a * i) * rad;

			glVertex2f(x, y);
		}

		glEnd();
		glPopMatrix();

		// ������ ������� �� ������
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, 0.2 + delta + coord.y, 0);
		glBegin(GL_TRIANGLES);

		glColor3f(0.1, 0.1, 0.1);
		glVertex2f(0, rad);
		glVertex2f(-0.01, rad + 0.1);
		glVertex2f(-0.05, rad + 0.1);

		glEnd();
		glPopMatrix();
	}

	// ����� ������� ���� ��������
	void P_Face_Render(float rad, float delta, float diff, point2d MousePoint)
	{
		float x_c = 0, y_c = 0;
		MoveFace(x_c, y_c, MousePoint, diff);  // ����� ������� �������� ��������� � ������� ������� �����

		// ������ ���� (� ����� �������� ��������)
		if (timer < 9 * time_limit / 10)     // �������� �����
		{
			float x = x_c, y = y_c;

			glColor3f(0.1, 0.1, 0.1);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(-0.1 + coord.x, 0.25 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad;
				y = y_c + cos(a * i) * rad;

				glVertex2f(x, y);
			}

			glEnd();
			glPopMatrix();

			x = x_c, y = y_c;

			glColor3f(1, 1, 1);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(-0.08 + coord.x, 0.27 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad * 0.3;
				y = y_c + cos(a * i) * rad * 0.3;

				glVertex2f(x, y);
			}

			glEnd();
			glPopMatrix();

			x = x_c, y = y_c;

			glColor3f(0.1, 0.1, 0.1);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(0.1 + coord.x, 0.25 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad;
				y = y_c + cos(a * i) * rad;

				glVertex2f(x, y);
			}

			glEnd();
			glPopMatrix();

			x = x_c, y = y_c;

			glColor3f(1, 1, 1);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(0.12 + coord.x, 0.27 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad * 0.3;
				y = y_c + cos(a * i) * rad * 0.3;

				glVertex2f(x, y);
			}
		}
		else if (timer > 9 * time_limit / 10)  // �������� �����
		{
			float x = x_c, y = y_c;

			glColor3f(0.2, 0.2, 0.2);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(-0.1 + coord.x, 0.25 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad;
				y = y_c + cos(a * i) * rad;

				glVertex2f(x, y);
			}

			glEnd();
			glPopMatrix();

			x = x_c, y = y_c;

			glColor3f(0.2, 0.2, 0.2);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(0.1 + coord.x, 0.25 + delta + coord.y, 0);

			glBegin(GL_TRIANGLE_FAN);

			glVertex2f(x, y);
			for (int i = -2; i < cnt; i++)
			{
				x = x_c + sin(a * i) * rad;
				y = y_c + cos(a * i) * rad;

				glVertex2f(x, y);
			}

			glEnd();
			glPopMatrix();
		}

		glEnd();
		glPopMatrix();

		float x = x_c, y = y_c;

		// ������ �����
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, 0.2 + delta + coord.y, 0);
		glBegin(GL_TRIANGLES);

		glColor3f(0.75, 0.75, 0);
		glVertex2f(-0.05 + x_c, -0.05 + y_c);
		glVertex2f(x_c / 2, -0.2 + y_c / 2 + 0.05);
		glVertex2f(0.05 + x_c, -0.05 + y_c);

		glColor3f(0.8, 0.8, 0);
		glVertex2f(-0.05 + x_c, -0.05 + y_c);
		glVertex2f(x_c, -0.2 + y_c);
		glVertex2f(0.05 + x_c, -0.05 + y_c);

		glEnd();
	}

	// ����� ������� ��� ��������
	void P_Arms_Render(float rad, float delta)
	{
		glColor3f(0.1, 0.1, 0.1);

		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0 + coord.x, 0.1 + coord.y, 0);
		glBegin(GL_QUADS);

		glVertex2f(-0.35 - fat * 0.05, -0.05);
		glVertex2f(-0.4 - fat * 0.05, -0.25);
		glVertex2f(-0.7 - fat * 0.05, -0.55 + delta);
		glVertex2f(-0.7 - fat * 0.05, -0.5 + delta);

		glVertex2f(0.35 + fat * 0.05, -0.05);
		glVertex2f(0.4 + fat * 0.05, -0.25);
		glVertex2f(0.7 + fat * 0.05, -0.55 + delta);
		glVertex2f(0.7 + fat * 0.05, -0.5 + delta);

		glEnd();
		glPopMatrix();
	}

	// ����� �������� ����
	void Pengu_Eat(Fish_Food& fish)
	{
		point2d FishCord = fish.fish_GetPoint();

		if (FishCord.x < 0.1 && FishCord.x > -0.1 && FishCord.y < 0.1 && FishCord.y > -0.1)
		{
			Pengu_fatter();
			fish.fish_Eated();
		}
	}

	// ����� ��������� ��������
	void Pengu_fatter()
	{
		if (fat_body_m < 8)
			fat_body_m++;
	}
};

// ����� ������� ��������� �� ������ ����
class IceBerg   
{
private:
	point2d coord;          // ���������� ��������
	point2d pattern[4];     // ������ ������ ��� ������������ ���������
	int pattern_num;        // ����� ������

	bool pattern_created;   // �������� �������� ��������
public:
	IceBerg() : coord(point2d(-1.5, -0.1)), pattern_created(false), pattern_num(1) { }        // ����������� ������

	void IceBerg_Render()      // ����� ������� ��������
	{
		flow_toRiver_Anim(0.001);    // �������� �������� �� �������

		switch (pattern_num)         // ����������� ����� ���������
		{
		case 1:
			pattern_1();           // 1 ��� ��������
			break;
		case 2:
			pattern_2();           // 2 ��� ��������
			break;
		case 3:
			pattern_3();           // 3 ��� ��������
			break;
		}

		// ������ �������� �� ������� ������������ ��������
		glBegin(GL_TRIANGLE_STRIP);
		
		glColor3f(0.9, 0.9, 0.9);
		for (int l = 0; l < 4; l++)
		{
			glVertex2f(pattern[l].x, pattern[l].y);
		}

		glEnd();
	}

	void flow_toRiver_Anim(double speed)   // ����� ��������� ���������� ��� �������� ������ �� ������
	{
		if (coord.x < 1.5)
		{
			coord.x += speed;
		}
		else if (coord.x > 1.5)
		{
			coord.x = -1.5;
			if (pattern_num < 3)
			{
				pattern_num++;
			}
			else pattern_num = 1;
		}
	}

	void pattern_1()                         // ������ ����� ������ ��������
	{
		pattern[0].x = coord.x - 0.2;
		pattern[0].y = coord.y + 0.3;

		pattern[1].x = coord.x - 0.3;
		pattern[1].y = coord.y - 0.1;

		pattern[2].x = coord.x + 0.3;
		pattern[2].y = coord.y - 0.1;

		pattern[3].x = coord.x + 0.1;
		pattern[3].y = coord.y + 0.2;
	}
	void pattern_2()						 // ������ ����� ������ ��������
	{
		pattern[0].x = coord.x - 0.35;
		pattern[0].y = coord.y + 0.3;

		pattern[1].x = coord.x - 0.3;
		pattern[1].y = coord.y;

		pattern[2].x = coord.x + 0.3;
		pattern[2].y = coord.y;

		pattern[3].x = coord.x;
		pattern[3].y = coord.y + 0.2;
	}
	void pattern_3()						 // ������ ����� ������ ��������
	{
		pattern[0].x = coord.x - 0.2;
		pattern[0].y = coord.y + 0.3;

		pattern[1].x = coord.x - 0.3;
		pattern[1].y = coord.y - 0.2;

		pattern[2].x = coord.x + 0.3;
		pattern[2].y = coord.y - 0.2;

		pattern[3].x = coord.x + 0.2;
		pattern[3].y = coord.y + 0.2;
	}
};

// ����� ���� ���������� (���� + ������ + �����)
class BACKGROUND
{
private:
	float R, G, B, light;           // �������� ������ ��� �������� ���������

public:
	BACKGROUND(float r, float g, float b, float l) : R(r), G(g), B(b), light(l) { }         // ����������� ������
	
	void BackGround_Render()             // ����� ������� ����
	{
		// ������ ���� � ����������
		glBegin(GL_QUADS);

		glColor3f(R, G, B);		

		glVertex2f(-1, 1);
		glVertex2f(1, 1);

		glColor3f(R + light, G + light, B + light);

		glVertex2f(1, -1);
		glVertex2f(-1, -1);

		// ������ ������
		glColor3f(0.3, 0.3, 1);

		glVertex2f(-1, -0.5);
		glVertex2f(-1, 0);
		glVertex2f(1, 0);
		glVertex2f(1, -0.5);

		glEnd();

		// ������ �������
		glBegin(GL_QUADS);

		glColor3f(0.9, 0.9, 0.9);

		glVertex2f(-1, -1);
		glVertex2f(-1, -0.45);
		glVertex2f(-0.6, -0.4);
		glVertex2f(-0.6, -1);

		glVertex2f(-0.6, -1);
		glVertex2f(-0.6, -0.4);	
		glVertex2f(-0.4, -0.5);
		glVertex2f(-0.4, -1);

		glVertex2f(-0.4, -1);
		glVertex2f(-0.4, -0.5);
		glVertex2f(0.3, -0.5);
		glVertex2f(0.3, -1);

		glVertex2f(0.3, -1);
		glVertex2f(0.3, -0.5);
		glVertex2f(0.6, -0.4);
		glVertex2f(0.6, -1);

		glVertex2f(0.6, -1);
		glVertex2f(0.6, -0.4);
		glVertex2f(1, -0.5);
		glVertex2f(1, -1);

		glEnd();

	}

};

