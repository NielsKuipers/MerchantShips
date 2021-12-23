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
#define KEY_B 98
#define KEY_S 115
#define KEY_BACKSPACE 8

namespace menuHandler
{
    static int posX{0};
    static int posY{0};
    static std::string previousLine;

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

    static void resetCursor()
    {
        previousLine = "";
        posX = posY = 0;
    }

    static std::tuple<int, int> handleInput(std::vector<std::string> &line, int key, int min, int max)
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
            default:
                return std::make_tuple(posX, (posY - min));
        }

        if (posY > max - 1)
            posY = max - 1;
        else if (posY < min)
            posY = min;

        setCursor(line[posY - min]);
        return std::make_tuple(posX, (posY - min));
    }

    static int getNumberInput()
    {
        int result;

        std::cin >> result;
        while (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "What are you saying? that's not a number, try again.";
            std::cin >> result;
        }

        return result;
    }

    static std::string getConfirmInput()
    {
        std::string result;

        std::cout << "Are you sure? y/n: ";
        std::cin >> result;
        while(!std::cin.good() || (result != "y" && result != "n"))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Uhm... It's a simple yes or no answer, try again.";
            std::cin >> result;
        }

        return result;
    }
}

#endif //MERCHANTSHIPS_MENUHANDLER_H
