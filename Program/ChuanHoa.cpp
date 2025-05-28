#include "ChuanHoa.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <ctime>
using namespace std;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

bool laNamNhuan(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool ngayHopLe(const Date& d) {
    if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > 31 || d.year < 1900 || d.year > 2025) {
        return false;
    }
    if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) {
        return d.day <= 30;
    }
    if (d.month == 2) {
        return laNamNhuan(d.year) ? d.day <= 29 : d.day <= 28;
    }
    return true;
}

bool tuoiHopLe(const Date& d) {
    time_t now = time(nullptr);
    tm* current = localtime(&now);
    int currentYear = current->tm_year + 1900;
    int age = currentYear - d.year;
    if (age < 18) return false;
    if (age == 18) {
        if (current->tm_mon + 1 < d.month) return false;
        if (current->tm_mon + 1 == d.month && current->tm_mday < d.day) return false;
    }
    return true;
}

bool luongHopLe(double luong) {
    return luong >= 0;
}

string chuanHoaTen(const string& ten) {
    stringstream ss(trim(ten));
    string result, word;
    while (ss >> word) {
        for (char& c : word) c = tolower(c);
        word[0] = toupper(word[0]);
        result += word + " ";
    }
    if (!result.empty()) result.pop_back();
    return result;
}

string inHoaChuoi(const string& s) {
    string res = s;
    for (char& c : res) {
        c = toupper(c);
    }
    return res;
}

bool maNhanVienHopLe(const NhanVien& nv) {
    if (nv.maNhanVien.length() != 8) return false;
    for (char c : nv.maNhanVien) {
        if (!isalnum(c)) return false;
    }
    return true;
}

bool isDuplicateID(const vector<NhanVien>& ds, const string& maNhanVien) {
    for (const auto& nv : ds) {
        if (nv.maNhanVien == maNhanVien) return true;
    }
    return false;
}
map<string, string> docDanhSachPhongBan(const string& filename) {
    map<string, string> danhSach;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            string ma = line.substr(0, pos);
            string ten = line.substr(pos + 1);
            danhSach[ma] = ten;
        }
    }
    
    return danhSach;
}