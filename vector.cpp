#include <iostream>
#include <windows.h>
#include <conio.h>
#include <random>

std::vector<std::vector<int>> generateApples() {
    std::vector<std::vector<int>>applesPositions;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(0, 24); // szerokość konsoli
    std::uniform_int_distribution<> distribY(0, 79); // wysokość konsoli
    for (int i = 0; i < 10; i++) {
        int jakas = distribX(gen);
        int losowa = distribY(gen);
        applesPositions.push_back({ jakas, losowa });
    }
    return applesPositions;
}

int x = 10;
std::vector<std::vector<int>> jablka = generateApples();
int y = 5;
bool wasmoved = true;
int length = 1;

void moveCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(hConsole, &screen);
    DWORD length = screen.dwSize.X * screen.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', length, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, screen.wAttributes, length, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

void render() {
    if (wasmoved) {
        moveCursor(x, y);
        for (int i = 0; i < length; i++) {
            
            std::cout << "$\b\b";
        }

        for (int j = 0; j < jablka.size(); j++) {
            moveCursor(jablka[j][1], jablka[j][0]);
            std::cout << "@";
        }
    }

}
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction direction = RIGHT;  // domyślnie idzie w prawo

void update() {
    wasmoved = false;
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            switch (arrow) {
            case 72: direction = UP; break;
            case 80: direction = DOWN; break;
            case 75: direction = LEFT; break;
            case 77: direction = RIGHT; break;
            }
        }
    }

    // Ruch automatyczny w zadanym kierunku
    switch (direction) {
    case UP: y--; break;
    case DOWN: y++; break;
    case LEFT: x--; break;
    case RIGHT: x++; break;
    }

    wasmoved = true;
    bool ateapple = false;
    for (int i = 0; i < jablka.size() ; i++) {
        if (x == jablka[i][1] && y == jablka[i][0] ){
            ateapple = true;
            jablka.erase(jablka.begin() + i);
            length++;
            break;
        }
        
    }

    if (wasmoved) {
        clearScreen();
    }
    //
}



int main() {

    while (true) {

        update();
        render();
        Sleep(100);


    }

}