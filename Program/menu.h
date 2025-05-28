#pragma once
#include<vector>
#include<iostream>
#include<string>
using namespace std;

void gotoxy(int x, int y); ;
void setColor(int color); 
void drawBox(int x, int y, int width, int height, int color = 11);
void clearScreen();
int menuChon(const vector<string>& options, const string& title);
void manHinhKhoiDong();
void veCombobox(int x, int y, const vector<string>& options, int selected);
void xoaVungConsole(short x1, short y1, short x2, short y2);
