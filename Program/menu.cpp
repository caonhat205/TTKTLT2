#include"menu.h"
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
using namespace std;

void clearScreen() {
    system("cls");
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void drawBox(int x, int y, int width, int height, int color) {
    SetConsoleOutputCP(437); // ASCII mở rộng (chứa mấy ký tự viền)

    setColor(color);

    // Ký tự viền dạng 1 đường thẳng kín
    const char TL = 218; // ┌
    const char TR = 191; // ┐
    const char BL = 192; // └
    const char BR = 217; // ┘
    const char HZ = 196; // ─
    const char VT = 179; // │

    // Vẽ dòng đầu (trên)
    gotoxy(x, y);
    cout << TL;
    for (int i = 0; i < width - 2; ++i) cout << HZ;
    cout << TR;

    // Vẽ các dòng giữa
    for (int i = 1; i < height - 1; ++i) {
        gotoxy(x, y + i);
        cout << VT;
        for (int j = 0; j < width - 2; ++j) cout << ' ';
        cout << VT;
    }

    // Vẽ dòng cuối (dưới)
    gotoxy(x, y + height - 1);
    cout << BL;
    for (int i = 0; i < width - 2; ++i) cout << HZ;
    cout << BR;

    setColor(7); // Trả lại màu mặc định
}
int menuChon(const vector<string>& options, const string& title) {
    if (options.empty()) return -1;
    int selected = 0;
    const int boxWidth = 30;
    const int boxHeight = 3;
    const int spacing = 1;
    int startX = (80 - boxWidth) / 2;
    int startY = 5;

    clearScreen();
    gotoxy((80 - title.length()) / 2, startY - 2); 
    setColor(14); cout << title; setColor(7);
    
    for (int i = 0; i < options.size(); i++) {
        int yPos = startY + i * (boxHeight + spacing);
        drawBox(startX, yPos, boxWidth, boxHeight, i == selected ? 10 : 11);
        int textX = startX + (boxWidth - options[i].length()) / 2;
        int textY = yPos + boxHeight / 2;
        gotoxy(textX, textY);
        if (i == selected) { setColor(240); cout << options[i]; setColor(7); }
        else { cout << options[i]; }
    }

    while (true) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            int prevSelected = selected;
            
            if (key == 72) selected = max(0, selected - 1);
            if (key == 80) selected = min((int)options.size() - 1, selected + 1);
            
            if (prevSelected != selected) {
                for (int i : {prevSelected, selected}) {
                    int yPos = startY + i * (boxHeight + spacing);
                    drawBox(startX, yPos, boxWidth, boxHeight, i == selected ? 10 : 11);
                    int textX = startX + (boxWidth - options[i].length()) / 2;
                    int textY = yPos + boxHeight / 2;
                    gotoxy(textX, textY);
                    if (i == selected) { setColor(240); cout << options[i]; setColor(7); }
                    else { cout << options[i]; }
                }
            }
        }
        else if (key == 13) {
            return selected;
        }
        else if (key == 27) {
            return -1;
        }
    }
}
void manHinhKhoiDong() {
    // Kích thước vùng màn hình khởi động
    const int width = 60;
    const int height = 15;
    const int x = 10;
    const int y = 3;

    // Vẽ khung chính
    drawBox(x, y, width, height, 11);

    // Tiêu đề
    setColor(14);
    string title = "THUC HANH MON KY THUAT LAP TRINH";
    gotoxy(x + (width - title.length()) / 2, y + 2);
    cout << title;

    setColor(13);
    string author = "Cao Viet Nhat";
    gotoxy(x + (width - author.length()) / 2, y + 4);
    cout << author;

    setColor(3);
    string note = "(Mo fullScreen de co trai nghiem tot nhat)";
    gotoxy(x + (width - note.length()) / 2, y + height - 4);
    cout << note;

    setColor(7);
    string guide = "Dung mui ten trai/phai de chon, Enter de xac nhan";
    gotoxy(x + (width - guide.length()) / 2, y + height - 2);
    cout << guide;

    // Button setup
    vector<string> buttons = { " START ", " EXIT " };
    int selected = 0;
    int btnY = y + height - 7;
    int btn1X = x + width / 2 - 15;
    int btn2X = x + width / 2 + 5;

    auto drawButtons = [&]() {
        for (int i = 0; i < buttons.size(); ++i) {
            int btnX = (i == 0) ? btn1X : btn2X;
            setColor((selected == i) ? 240 : 15);
            drawBox(btnX, btnY, 10, 3, (selected == i) ? 10 : 8);
            gotoxy(btnX + 2, btnY + 1);
            cout << buttons[i];
            setColor(7);
        }
    };

    drawButtons();

    // Vòng lặp chọn button
    while (true) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int key = _getch();
            if (key == 75) { // Left
                selected = (selected + buttons.size() - 1) % buttons.size();
                drawButtons();
            } else if (key == 77) { // Right
                selected = (selected + 1) % buttons.size();
                drawButtons();
            }
        } else if (ch == 13) { // Enter
            if (selected == 1) { // EXIT
                clearScreen();
                exit(0);
            } else { // START
                clearScreen();
                return;
            }
        }
    }
}
void veCombobox(int x, int y, const vector<string>& options, int selected) {
    drawBox(x, y, 40, options.size() + 2, 11);
    for (int i = 0; i < options.size(); i++) {
        gotoxy(x + 2, y + 1 + i);
        if (i == selected) {
            setColor(240);
            cout << "> " << options[i];
        } else {
            setColor(7);
            cout << "  " << options[i];
        }
    }
}
void xoaVungConsole(short x1, short y1, short x2, short y2) {
    for (short y = y1; y <= y2; ++y) {
        gotoxy(x1, y);
        for (short x = x1; x <= x2; ++x) {
        setColor(0);
        cout << ' ';
        }
    }
}

