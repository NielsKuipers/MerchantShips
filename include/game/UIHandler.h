//
// Created by Niels on 13/12/21.
//

#ifndef MERCHANTSHIPS_UIHANDLER_H
#define MERCHANTSHIPS_UIHANDLER_H

#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_B 98
#define KEY_S 115
#define KEY_BACKSPACE 8

namespace UIHandler
{
    static int posX{0};
    static int posY{0};
    static std::string previousLine;
    static bool resetForInput{false};
    static long long t{std::time(nullptr)};
    static tm tm{*std::localtime(&t)};
    inline std::string filename{"../logs/gamelog from "};

    static void initLog()
    {
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        filename += oss.str() + ".txt";
    }

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

    static void outputText(const std::string &text)
    {
        std::ofstream out(filename, std::ios_base::app);

        char delim{';'};
        std::stringstream stream(text);
        std::string line;

        while (std::getline(stream, line, delim))
        {
            std::cout << line << std::endl;
            out << line << std::endl;
        }
    }

    static int getNumberInput()
    {
        int result;
        std::ofstream out(filename, std::ios_base::app);

        std::cin >> result;
        while (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            outputText("What are you saying? that's not a number, try again.");
            std::cin >> result;
        }

        out << result << std::endl;
        return result;
    }

    static std::string getConfirmInput()
    {
        std::string result;
        std::ofstream out(filename, std::ios_base::app);

        outputText("Are you sure? y/n: ");
        std::cin >> result;
        while (!std::cin.good() || (result != "y" && result != "n"))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            outputText("Uhm... It's a simple yes or no answer, try again.");
            std::cin >> result;
        }

        out << result << std::endl;
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
};

#endif //MERCHANTSHIPS_UIHANDLER_H
