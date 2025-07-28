#include <iostream>
#include <windows.h>
#include <conio.h>
#include <random>

std::vector<std::vector<int>> generateApples() {
    std::vector<std::vector<int>>applesPositions;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(3, 24); // szerokość konsoli
    std::uniform_int_distribution<> distribY(2, 79); // wysokość konsoli
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
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(hConsole, &screen);

    COORD start = { 0, 1 };  // <-- od drugiego wiersza
    DWORD lengthTotal = screen.dwSize.X * (screen.dwSize.Y - 1); // -1 bo pomijamy 1 linię

    FillConsoleOutputCharacter(hConsole, ' ', lengthTotal, start, &written);
    FillConsoleOutputAttribute(hConsole, screen.wAttributes, lengthTotal, start, &written);
}

void showStartScreen() {
    clearScreen();
    moveCursor(30, 10);
    std::cout << "             --- PRESS ENTER TO START ---";

    while (true) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 13) {  // 13 = Enter
                break;
            }
        }
        Sleep(10);
    }
    clearScreen();

}

void render() {

    for (int m = 0; m < 80; m++) {
        moveCursor(1 + m, 1);
        std::cout << "-";
    }
    for (int l = 0; l < 80; l++) {
        moveCursor(1 + l, 25);
        std::cout << "-";
    }
    for (int n = 0; n < 25; n++) {
        moveCursor(1, 1 + n);
        std::cout << "|";
    }
    for (int n = 0; n < 25; n++) {
        moveCursor(80, 1 + n);
        std::cout << "|";
    }




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
    int score = length - 1;



    moveCursor(0, 0);
    std::cout << "Score: " << score << "   ";
        
    
    if (x < 1 || x >= 80 || y < 2 || y >= 25) {
        clearScreen();
        moveCursor(0, 0);
        std::cout << "Game over!" << std::endl;
        exit(0);


    }

    if (score == 10) {
        clearScreen();
        moveCursor(0, 1);
        std::cout << "You win!" << std::endl;
        exit(0);
    }

    if (wasmoved) {
        clearScreen();
    }
}



int main() {

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(out, &cursorInfo);

    showStartScreen();

    while (true) {
		
        update();
        render();
        Sleep(100);
    }

}