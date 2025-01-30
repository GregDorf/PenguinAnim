#include <iostream>
#include <GLFW/glfw3.h>
#include "ObjectsLib.h"
#include <math.h>

// инициализация констант
#define p 3.14

int main(void)
{
    GLFWwindow* window;
    int wid = 1080, heg = 1080;                 // размер окна

    BACKGROUND bg(0.2f, 0.3f, 1.0f, 1.0f);      // объект класса ФОН
    PENGUIN pengu;                              // объект класса ПИНГВИН
    IceBerg ice;                                // объект класса АЙСБЕРГ
    Fish_Food fish;                             // объект класса РЫБА

    point2d point;                              // переменная, хранящая координаты нажатия мышью

    if (!glfwInit())                            // инициализация glfw библиотеки
        return -1;

    window = glfwCreateWindow(wid, heg, "", NULL, NULL);            // создание окна
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);                 // задаем текущему окну контекст устройства

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);                     // очистка буфера

        point = FindMousePosition(window, wid, heg);      // обнарудение нажатия мышью на экран
        if (glfwGetKey(window, 32))
        {
            fish.fish_RandomSpawn();                      // создание рыбы
        }

        bg.BackGround_Render();                           // рендер фона

        ice.IceBerg_Render();                             // рендер айсберга

        if (fish.isExist) pengu.Pengu_Render(120, fish.fish_GetPoint());   // рендер пингвина (смотрит на рыбу)
        else pengu.Pengu_Render(120, point);                               // рендер пингвина (не смотрит на рыбу)

        if (fish.isExist)                                                  // съедение рыбы пингвином
        {
            fish.Fish_render(point, 25);
            pengu.Pengu_Eat(fish);
        }

        glfwSwapBuffers(window);                                           // смена буфера

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}