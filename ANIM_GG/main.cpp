#include <iostream>
#include <GLFW/glfw3.h>
#include "ObjectsLib.h"
#include <math.h>

// ������������� ��������
#define p 3.14

int main(void)
{
    GLFWwindow* window;
    int wid = 1080, heg = 1080;                 // ������ ����

    BACKGROUND bg(0.2f, 0.3f, 1.0f, 1.0f);      // ������ ������ ���
    PENGUIN pengu;                              // ������ ������ �������
    IceBerg ice;                                // ������ ������ �������
    Fish_Food fish;                             // ������ ������ ����

    point2d point;                              // ����������, �������� ���������� ������� �����

    if (!glfwInit())                            // ������������� glfw ����������
        return -1;

    window = glfwCreateWindow(wid, heg, "", NULL, NULL);            // �������� ����
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);                 // ������ �������� ���� �������� ����������

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);                     // ������� ������

        point = FindMousePosition(window, wid, heg);      // ����������� ������� ����� �� �����
        if (glfwGetKey(window, 32))
        {
            fish.fish_RandomSpawn();                      // �������� ����
        }

        bg.BackGround_Render();                           // ������ ����

        ice.IceBerg_Render();                             // ������ ��������

        if (fish.isExist) pengu.Pengu_Render(120, fish.fish_GetPoint());   // ������ �������� (������� �� ����)
        else pengu.Pengu_Render(120, point);                               // ������ �������� (�� ������� �� ����)

        if (fish.isExist)                                                  // �������� ���� ���������
        {
            fish.Fish_render(point, 25);
            pengu.Pengu_Eat(fish);
        }

        glfwSwapBuffers(window);                                           // ����� ������

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}