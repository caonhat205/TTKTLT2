#pragma once
#include"menu.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Date {
    int day, month, year;
};

struct NhanVien {
    string maPhongBan;
    string tenPhongBan;
    string maNhanVien;
    string hoTen;
    string chucVu;
    Date ngaySinh;
    double luong;
};

string nhapVaChinhSuaTrenConsole(const vector<string>& label, vector<string>& data, 
                                const vector<bool>& invalidFields, const vector<NhanVien>& ds);
void themMoiNhanVien(vector<NhanVien>& ds);
void inDanhSach(const vector<NhanVien>& ds, const string& note = "");
void xuLyHoSo(vector<NhanVien>& ds);
void suaHoSo(vector<NhanVien>& ds);
void xoaHoSo(vector<NhanVien>& ds);
bool ghiFile(const vector<NhanVien>& ds, const string& filename);
bool docFile(vector<NhanVien>& ds, const string& filename);