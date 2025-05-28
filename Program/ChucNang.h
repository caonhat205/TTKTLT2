#pragma once
#include"menu.h"
#include "NhanVien.h"
#include "ChuanHoa.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

// So sánh theo từng trường
bool soSanhMaPB(const NhanVien& a, const NhanVien& b);
bool soSanhHoTen(const NhanVien& a, const NhanVien& b);
bool soSanhNgaySinh(const NhanVien& a, const NhanVien& b);
bool soSanhLuong(const NhanVien& a, const NhanVien& b);

// So sánh động nhiều tiêu chí
struct SoSanhNhieuTieuChi {
    vector<int> tieuChi; // chỉ số trường ưu tiên
    bool operator()(const NhanVien& a, const NhanVien& b) const;
};

// Các thuật toán sắp xếp thủ công
void selectionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp);
void insertionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp);
void quickSort(vector<NhanVien>& ds, int low, int high, const function<bool(const NhanVien&, const NhanVien&)>& cmp);
void mergeSort(vector<NhanVien>& ds, int left, int right, const function<bool(const NhanVien&, const NhanVien&)>& cmp);

// Sắp xếp tổng quát
void sapXep(vector<NhanVien>& ds);
void timKiem(const vector<NhanVien>& ds);
void thongKe(const vector<NhanVien>& ds);

vector<NhanVien> timKiemChinhXac(const vector<NhanVien>& ds, int khoa, const string& giaTri);
vector<NhanVien> timKiemGanDung(const vector<NhanVien>& ds, int khoa, const string& giaTri);
vector<NhanVien> timKiemToanBo(const vector<NhanVien>& ds, const string& giaTri);
vector<NhanVien> timKiemTuanTu(const vector<NhanVien>& ds, int khoa, const string& giaTri);
vector<NhanVien> timKiemNhiPhan(const vector<NhanVien>& ds, int khoa, const string& giaTri);