#pragma once
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include "MovingLib.h"

#define p 3.14

//класс РЫБА
class Fish_Food
{
private:
	point2d coord; // координаты рыбы
	int t_angle;   // угол, на который она поворачивается
	int angle;     // угол, на который должна повернуться
	bool t_check;  // проверка обратного хода анимации

public:
	bool isExist;  // существует ли рыба

	Fish_Food() : isExist(false), coord(point2d(0, 1.2)), t_angle(0), angle(0), t_check(false) { }  // конструктор класса

	void Fish_render(point2d MouseCord, int angle_)     // метод рендеринга рыбы
	{
		if (MouseCord.x != NULL || MouseCord.y != NULL)  // проверка нажатия мышью на экран
		{
			coord = Grab_Object(coord, MouseCord);       // схватывание рыбы мышью
			t_angle = -90;                               // поворот рыбы на -90 градусов (имитация падения)
		}
		else
		{
			coord = Gravity(coord, -0.6, -0.03);         // вызов функции гравитации
			t_angle = angle_;                            // поворот рыбы на angle_
		}

		loopAnimation(angle, angle_ / 2, t_check);       // зацикленная анимация дергания рыбы

		// начало рендера рыбы
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
		// конец рендера рыбы

		// начало рендера глаза рыбы
		glColor3f(0, 0, 0);
		glPointSize(15);
		glBegin(GL_POINTS);

		glVertex2f(0.12, 0);

		glEnd();
		glPopMatrix();
		// конец рендера глаза рыбы
	}

	// метод, для отключения рендера рыбы в случае, если ее съели
	void fish_Eated()
	{
		coord = point2d(0, 1.2);
		isExist = false;
	}

	// получение координат местоположения рыбы
	point2d fish_GetPoint()
	{
		return coord;
	}

	// появление рыбы в случайном месте в небе
	void fish_RandomSpawn()
	{
		if (!isExist)
		{
			coord.x = (rand() % 20 - 10) * 0.1;
			isExist = true;
		}
	}
};

// класс ПИНГВИН
class PENGUIN
{
private:
	point2d coord;  // координаты местоположения пингвина
	int cnt;        // количество треугольников, на которые мы разбиваем круги
	bool t_check;   // проверка оюратного хода анимации
	int timer, time_limit;   // смещение координаты при анимации и ее максисальное значение
	float a;        // угол поворота, на который смещаемся при разбиении круга

	int fat_body_m;    // насколько пингвин потолстел
	double fat;        // плавное увеличение размеров пингвина

public:
	PENGUIN() : coord(point2d(0, 0)), cnt(30), timer(0), fat_body_m(0), fat(0)  // конструктор класса по умолчанию
	{ 
		a = 2 * p / cnt; 
		t_check = false;
	}
	PENGUIN(point2d cord, int c) : coord(cord), cnt(c), timer(0), fat_body_m(0), fat(0)  // конструктор класса с параметрами
	{
		a = 2 * p / cnt;
		t_check = false;
	}

	void Pengu_Render(int t_limit, point2d MouseCord)       // метод рендера пингвина
	{
		time_limit = t_limit;

		if (fat < fat_body_m) fat += 0.01;      // толстение пингвина

		loopAnimation(timer, time_limit, t_check);     // смещение координаты при зацикленных анимациях

		P_Black_Render(0.5, -timer * 0.0003);                        // рендер черной части пингвина (задний план)
		P_White_Render(0.4, -timer * 0.0003, 0.02, MouseCord);		 // рендер белой части пингвина (передний план)
		P_Face_Render(0.05, -timer * 0.0003, 0.05, MouseCord);		 // рендер лица пингвина
		P_Arms_Render(0.05, -timer * 0.0003);						 // рендер рук пингвина
	}

	// метод рендера белой части пингвина
	void P_White_Render(float rad, float delta, float diff, point2d MousePoint)
	{
		float x = coord.x, y = coord.y;

		// рендер нижнего круга
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

		// рендер верхнего круга
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

	// метод рендера черной части пингвина
	void P_Black_Render(float rad, float delta)
	{
		float x = 0, y = 0;

		// рендер нижнего круга
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

		// рендер верхнего круга
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

		// рендер хохолка на голове
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

	// метод рендера лица пингвина
	void P_Face_Render(float rad, float delta, float diff, point2d MousePoint)
	{
		float x_c = 0, y_c = 0;
		MoveFace(x_c, y_c, MousePoint, diff);  // вызов функции смещения координат в сторону нажания мышью

		// рендер глаз (а также анимации моргания)
		if (timer < 9 * time_limit / 10)     // открытые глаза
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
		else if (timer > 9 * time_limit / 10)  // закрытые глаза
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

		// рендер клюва
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

	// метод рендера рук пингвина
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

	// метод поедания рыбы
	void Pengu_Eat(Fish_Food& fish)
	{
		point2d FishCord = fish.fish_GetPoint();

		if (FishCord.x < 0.1 && FishCord.x > -0.1 && FishCord.y < 0.1 && FishCord.y > -0.1)
		{
			Pengu_fatter();
			fish.fish_Eated();
		}
	}

	// метод толстения пингвина
	void Pengu_fatter()
	{
		if (fat_body_m < 8)
			fat_body_m++;
	}
};

// метод рендера айсбергов на заднем фоне
class IceBerg   
{
private:
	point2d coord;          // координаты айсберга
	point2d pattern[4];     // наборы вершин для кастомизации айсбергов
	int pattern_num;        // номер набора

	bool pattern_created;   // проверка создания айсберга
public:
	IceBerg() : coord(point2d(-1.5, -0.1)), pattern_created(false), pattern_num(1) { }        // конструктор класса

	void IceBerg_Render()      // метод рендера айсберга
	{
		flow_toRiver_Anim(0.001);    // анимация движения по течению

		switch (pattern_num)         // чередование видов айсбергов
		{
		case 1:
			pattern_1();           // 1 тип айсберга
			break;
		case 2:
			pattern_2();           // 2 тип айсберга
			break;
		case 3:
			pattern_3();           // 3 тип айсберга
			break;
		}

		// рендер айсберга по заданее определенным вершинам
		glBegin(GL_TRIANGLE_STRIP);
		
		glColor3f(0.9, 0.9, 0.9);
		for (int l = 0; l < 4; l++)
		{
			glVertex2f(pattern[l].x, pattern[l].y);
		}

		glEnd();
	}

	void flow_toRiver_Anim(double speed)   // метод сммещения координаты для имитации плытия по океану
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

	void pattern_1()                         // первый набор вершин айсберга
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
	void pattern_2()						 // второй набор вершин айсберга
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
	void pattern_3()						 // третий набор вершин айсберга
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

// класс фона приложения (небо + ледник + океан)
class BACKGROUND
{
private:
	float R, G, B, light;           // смещение цветов для создания градиента

public:
	BACKGROUND(float r, float g, float b, float l) : R(r), G(g), B(b), light(l) { }         // конструктор класса
	
	void BackGround_Render()             // метод рендера фона
	{
		// рендер неба с градиентом
		glBegin(GL_QUADS);

		glColor3f(R, G, B);		

		glVertex2f(-1, 1);
		glVertex2f(1, 1);

		glColor3f(R + light, G + light, B + light);

		glVertex2f(1, -1);
		glVertex2f(-1, -1);

		// рендер океана
		glColor3f(0.3, 0.3, 1);

		glVertex2f(-1, -0.5);
		glVertex2f(-1, 0);
		glVertex2f(1, 0);
		glVertex2f(1, -0.5);

		glEnd();

		// рендер ледника
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

