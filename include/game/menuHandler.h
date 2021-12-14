//
// Created by Niels on 13/12/21.
//

#ifndef MERCHANTSHIPS_MENUHANDLER_H
#define MERCHANTSHIPS_MENUHANDLER_H

#include <conio.h>
#include <iomanip>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32

namespace menuHandler
{
    static int posX{0};
    static int posY{0};
    static std::string previousLine;

    static int handleInput(int key, int max)
    {
        switch (key)
        {
            case KEY_UP:
                posY -= 1;
                break;
            case KEY_DOWN:
                posY += 1;
                break;
            case KEY_LEFT:
                posX -= 1;
                break;
            case KEY_RIGHT:
                posX += 1;
                break;
            case KEY_SPACE:
                posX = posY = 0;
                previousLine = "";
            default:
                break;
        }

        if (posY > max - 1)
            posY = max - 1;
        else if (posY < 0)
            posY = 0;

        return posY;
    }

    static void setCursor(const std::string &line, short x = posX, short y = posY)
    {
        static HANDLE h = nullptr;
        static CONSOLE_CURSOR_INFO cursor;

        if (!h)
            h = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!previousLine.empty())
            std::cout << previousLine;

        COORD c = {x, y};
        SetConsoleCursorPosition(h, c);
        SetConsoleTextAttribute(h, 3);

        previousLine = line;
        std::cout << line << '\r';

        SetConsoleTextAttribute(h, 7);
    }
}

#endif //MERCHANTSHIPS_MENUHANDLER_H
