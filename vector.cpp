#include <iostream>
#include <windows.h>
#include <conio.h>
#include <random>
#include <deque>

std::vector<std::vector<int>> generateApples() {
    std::vector<std::vector<int>> applesPositions;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(3, 24);
    std::uniform_int_distribution<> distribY(2, 79);

    for (int i = 0; i < 10; i++) {
        applesPositions.push_back({ distribX(gen), distribY(gen) });
    }
    return applesPositions;
}

std::deque<std::pair<int, int>> snake;  // nowy: przechowuje ciało węża
int x = 10, y = 5;
std::vector<std::vector<int>> jablka = generateApples();
bool wasmoved = true;
int length = 1;

void moveCursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(hConsole, &screen);
    COORD start = { 0, 1 };
    DWORD lengthTotal = screen.dwSize.X * (screen.dwSize.Y - 1);
    FillConsoleOutputCharacter(hConsole, ' ', lengthTotal, start, &written);
    FillConsoleOutputAttribute(hConsole, screen.wAttributes, lengthTotal, start, &written);
}

void showStartScreen() {
    clearScreen();
    moveCursor(30, 10);
    std::cout << "             --- PRESS ENTER TO START ---";
    while (true) {
        if (_kbhit() && _getch() == 13) break;
        Sleep(10);
    }
    clearScreen();
}

void render() {
    for (int m = 0; m < 80; m++) {
        moveCursor(1 + m, 1); std::cout << "-";
        moveCursor(1 + m, 25); std::cout << "-";
    }
    for (int n = 0; n < 25; n++) {
        moveCursor(1, 1 + n); std::cout << "|";
        moveCursor(80, 1 + n); std::cout << "|";
    }

    if (wasmoved) {
        // rysuj ciało
        for (auto& segment : snake) {
            moveCursor(segment.first, segment.second);
            std::cout << "$";
        }

        // rysuj jabłka
        for (auto& apple : jablka) {
            moveCursor(apple[1], apple[0]);
            std::cout << "@";
        }
    }
}

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction direction = RIGHT;

void update() {
    wasmoved = false;

    if (_kbhit()) {
        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            switch (arrow) {
            case 72: if (direction != DOWN) direction = UP; break;
            case 80: if (direction != UP) direction = DOWN; break;
            case 75: if (direction != RIGHT) direction = LEFT; break;
            case 77: if (direction != LEFT) direction = RIGHT; break;
            }
        }
    }

    // nowa głowa
    switch (direction) {
    case UP:    y--; break;
    case DOWN:  y++; break;
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    }

    // kolizja ze ścianą
    if (x < 1 || x >= 80 || y < 2 || y >= 25) {
        clearScreen();
        moveCursor(0, 0);
        std::cout << "Game over! (ściana)";
        exit(0);
    }

    // kolizja z samym sobą
    for (auto& segment : snake) {
        if (segment.first == x && segment.second == y) {
            clearScreen();
            moveCursor(0, 0);
            std::cout << "Game over! (sam siebie)";
            exit(0);
        }
    }

    snake.push_front({ x, y });  // dodaj nową głowę
    wasmoved = true;

    // sprawdź czy zjedzone jabłko
    bool ateApple = false;
    for (int i = 0; i < jablka.size(); i++) {
        if (x == jablka[i][1] && y == jablka[i][0]) {
            jablka.erase(jablka.begin() + i);
            length++;
            ateApple = true;
            break;
        }
    }

    if (!ateApple) {
        snake.pop_back(); // usuń ogon tylko jeśli nie zjedzono
    }

    // wynik
    int score = length - 1;
    moveCursor(0, 0);
    std::cout << "Score: " << score << "    ";

    if (score == 10) {
        clearScreen();
        moveCursor(0, 1);
        std::cout << "You win!";
        exit(0);
    }

    if (wasmoved) clearScreen();
}

int main() {
    // ukryj kursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(out, &cursorInfo);

    showStartScreen();

    snake.push_back({ x, y });  // inicjalna pozycja węża

    while (true) {
        update();
        render();
        Sleep(100);
    }
}