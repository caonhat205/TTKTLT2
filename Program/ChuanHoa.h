#pragma once
#include "NhanVien.h"
#include <string>
#include <vector>
#include<map>
#include<fstream>
using namespace std;

string trim(const string& s);
bool laNamNhuan(int y);
bool ngayHopLe(const Date& d);
bool tuoiHopLe(const Date& d);
bool luongHopLe(double luong);
string chuanHoaTen(const string& ten);
bool maNhanVienHopLe(const NhanVien& nv);
bool isDuplicateID(const vector<NhanVien>& ds, const string& maNhanVien);
string inHoaChuoi(const string& s);
map<string, string> docDanhSachPhongBan(const string& filename);