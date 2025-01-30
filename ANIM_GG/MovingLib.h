#pragma once
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <iostream>

// ��������� ��� �������� ����� (�� ������������)
//struct color3d
//{
//public:
//	double r;
//	double g;
//	double b;
//
//	color3d() { r = 0; g = 0; b = 0; }
//	color3d(double r_, double g_, double b_) { r = r_; g = g_; b = b_; }
//};

// ��������� ��� �������� ��������� ������
struct point2d
{
public:
	double x;
	double y;

	point2d() { x = 0; y = 0; }
	point2d(double x_, double y_) : x(x_), y(y_) { }
};

// ������� ������������ ��������� ������� �����
void NormalizeMouseCords(point2d& point, int wid, int heg)
{
	point.x = ((point.x / wid) - 0.5) * 2;
	point.y = ((-point.y / heg) + 0.5) * 2;
}

// ����������� ������� ����� � ���������� �������� ���������
point2d FindMousePosition(GLFWwindow* window, int wid, int heg)
{
	point2d coord;
	double x, y;

	if (glfwGetMouseButton(window, 0)) 
	{
		glfwGetCursorPos(window, &x, &y);
		coord.x = x;
		coord.y = y;

		NormalizeMouseCords(coord, wid, heg);
	}
	else
	{
		coord.x = 0;
		coord.y = 0;
	}

	return coord;
}

// ������� �������� ���� �������� � ����������� �������
void MoveFace(float& x, float& y, point2d mousecord, float diff)
{
	x = x + diff * mousecord.x;
	y = y + diff * mousecord.y;
}

// ������� ������� ����������
point2d Gravity(point2d coord, float y_min, float speed)
{
	if (coord.y > y_min) coord.y += speed;
	return coord;
}

// ������� ����������� �������� �� ������
point2d Grab_Object(point2d& coord, const point2d& mousecoord)
{
	double x = nearbyint(coord.x * 10) / 10;
	double y = nearbyint(coord.y * 10) / 10;

	double m_x = nearbyint(mousecoord.x * 10) / 10;
	double m_y = nearbyint(mousecoord.y * 10) / 10;

	if ((x <= m_x+0.2 && y <= m_y+0.2) && (x >= m_x - 0.2 && y >= m_y - 0.2) && (x <= m_x + 0.2 && y >= m_y - 0.2) && (x >= m_x - 0.2 && y <= m_y + 0.2))
	{
		coord.x = mousecoord.x;
		coord.y = mousecoord.y;
	}

	return coord;
}

// �������� ��������� ��������� ��� ����������� ��������
void loopAnimation(int& timer, int time_limit, bool& t_check)
{
	if (timer < time_limit && !t_check)
	{
		timer++;
	}
	else if (!t_check) t_check = true;

	else if (timer > 0 && t_check)
	{
		timer--;
	}
	else if (t_check)
	{
		t_check = false;
	}
}