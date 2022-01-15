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
    static bool resetForInput{false};

    static void setCursor(const std::string &line, short x = posX, short y = posY)
    {
        static HANDLE h = nullptr;

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

    static void setCursorForText(short x = posX, short y = posY)
    {
        static HANDLE h = nullptr;

        if (!h)
            h = GetStdHandle(STD_OUTPUT_HANDLE);

        COORD c = {x, y};
        SetConsoleCursorPosition(h, c);
        resetForInput = true;
    }

    static void resetCursor()
    {
        previousLine = "";
        posX = posY = 0;
    }

    static std::tuple<int, int> handleInput(std::vector<std::string> &line, int key, int min, int max)
    {
        if (resetForInput)
        {
            setCursorForText();
            resetForInput = false;
        }

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
        while (!std::cin.good() || (result != "y" && result != "n"))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Uhm... It's a simple yes or no answer, try again.";
            std::cin >> result;
        }

        return result;
    }

    static void deleteLines(int lines, int amount)
    {
        HANDLE hConsoleOutput{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        BOOL ok{GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)};

        for (int i = 0; i < lines; ++i)
        {
            std::cout << std::string(amount, ' ');
            setCursorForText(0, csbi.dwCursorPosition.Y + i);
        }

        setCursorForText(0, csbi.dwCursorPosition.Y);
    }
}

#endif //MERCHANTSHIPS_MENUHANDLER_H
