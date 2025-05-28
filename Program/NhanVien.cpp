#include "NhanVien.h"
#include"ChuanHoa.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <map>
using namespace std;

string nhapVaChinhSuaTrenConsole(const vector<string>& label, vector<string>& data, 
                                const vector<bool>& invalidFields, const vector<NhanVien>& ds) {
    const int formWidth = 70;
    const int fieldHeight = 3;
    const int startX = 25;
    const int startY = 4;
    int cur = 0;
    bool firstDraw = true;
    int comboBoxSelection = 0;

    // Đọc danh sách phòng ban từ file
    map<string, string> danhSachPhong;
    ifstream file("PhongBan.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            string ma = line.substr(0, pos);
            string ten = line.substr(pos + 1);
            danhSachPhong[ma] = ten;
        }
    }
    file.close();

    vector<string> dsMaPhong, dsTenPhong;
    for (const auto& pb : danhSachPhong) {
        dsMaPhong.push_back(pb.first);
        dsTenPhong.push_back(pb.second);
    }

    if (invalidFields.size() == label.size()) {
        for (int i = 0; i < invalidFields.size(); ++i) {
            if (invalidFields[i]) {
                cur = i;
                //data[i] = "";
                break;
            }
        }
    }

    int idxMaPB = -1, idxTenPB = -1;
    for (int i = 0; i < label.size(); ++i) {
        if (label[i] == "Ma phong ban") idxMaPB = i;
        if (label[i] == "Ten phong ban") idxTenPB = i;
    }

    vector<int> inputPositions(label.size());
    int labelWidth = 24;
    for (int i = 0; i < label.size(); ++i) {
        inputPositions[i] = startX + 4 + labelWidth + 2;
    }

    auto drawFormBox = [&](int height) {
        setColor(8);
        gotoxy(startX, startY - 2);
        cout << char(201) << string(formWidth - 2, char(205)) << char(187);
        for (int i = 0; i < height; ++i) {
            gotoxy(startX, startY - 1 + i);
            cout << char(186) << string(formWidth - 2, ' ') << char(186);
        }
        gotoxy(startX, startY - 1 + height);
        cout << char(200) << string(formWidth - 2, char(205)) << char(188);
        setColor(7);
    };

    auto drawField = [&](int i, bool isSelected) {
        int y = startY + i * fieldHeight;

        int fieldBoxX = startX + 2, fieldBoxW = formWidth - 4;
        setColor(isSelected ? 11 : 7);
        // Viền mỗi trường
        gotoxy(fieldBoxX, y);
        cout << char(218) << string(fieldBoxW - 2, char(196)) << char(191);
        gotoxy(fieldBoxX, y + 1);
        cout << char(179) << string(fieldBoxW - 2, ' ') << char(179);
        gotoxy(fieldBoxX, y + 2);
        cout << char(192) << string(fieldBoxW - 2, char(196)) << char(217);

        // Label căn trái, gọn
        gotoxy(fieldBoxX + 2, y + 1);
        cout << left << setw(labelWidth) << label[i] << ": ";

        // Data
        string showData = data[i].substr(0, fieldBoxW - (labelWidth + 8));
        cout << showData;

        // Nếu là trường mã phòng ban và đang chọn thì hiện combobox
        if (i == idxMaPB && isSelected) {
            vector<string> options;
            for (int k = 0; k < dsMaPhong.size(); ++k)
                options.push_back(dsMaPhong[k] + " - " + dsTenPhong[k]);
            veCombobox(fieldBoxX + fieldBoxW + 2, y, options, comboBoxSelection);
        }
        setColor(7);
    };

    auto drawAll = [&]() {
        int formHeight = label.size() * fieldHeight + 5;
        clearScreen();
        // Khung ngoài cho form
        drawFormBox(formHeight);

        // Tiêu đề
        setColor(10);
        gotoxy(startX + (formWidth - 22) / 2, startY - 2);
        cout << "NHAP/CHINH SUA NHAN VIEN";
        setColor(8);
        gotoxy(startX + 2, startY - 1);
        cout << "DUNG MUI TEN DE DI CHUYEN | ENTER DE CHON | ESC DE HUY";
        setColor(7);

        for (int i = 0; i < label.size(); ++i) {
            drawField(i, i == cur);
        }

        // Vẽ dòng lỗi
        if (invalidFields.size() == label.size()) {
            setColor(12);
            gotoxy(startX + 2, startY + label.size() * fieldHeight + 1);
            cout << "THONG BAO LOI:";
            int line = 2;
            for (int i = 0; i < invalidFields.size(); ++i) {
                if (invalidFields[i]) {
                    gotoxy(startX + 4, startY + label.size() * fieldHeight + line);
                    if (data[i].empty()) {
                        cout << "- " << label[i] << " khong duoc de trong!";
                        line++;
                    } else if (label[i] == "Ngay sinh (dd mm yyyy)") {
                        cout << "- Ngay sinh khong hop le hoac nhan vien chua du 18 tuoi!";
                        line++;
                    } else if (label[i] == "Luong (nghin VND)") {
                        try {
                            stod(data[i]);
                            cout << "- Luong khong hop le (phai >= 0)!";
                        } catch (const std::invalid_argument&) {
                            cout << "- Luong phai la so hop le!";
                        } catch (const std::out_of_range&) {
                            cout << "- Luong qua lon!";
                        }
                        line++;
                    } else if (label[i] == "Ma nhan vien (8 chu so)") {
                        if (data[i].empty()) {
                            cout << "- Ma nhan vien khong duoc de trong!";
                        } else if (isDuplicateID(ds, trim(data[i]))) {
                            cout << "- Ma nhan vien da ton tai!";
                        } else {
                            cout << "- Ma nhan vien khong hop le (8 ky tu, chi bao gom chu va so)!";
                        }
                        line++;
                    } else {
                        cout << "- " << label[i] << " khong hop le!";
                        line++;
                    }
                }
            }
        }
        setColor(7);
    };

    // Field bỏ qua
    auto nextField = [&](int cur, int dir) {
        int n = label.size();
        int newCur = cur;
        do {
            newCur = (newCur + dir + n) % n;
        } while (newCur == idxTenPB); // Bỏ qua trường tên phòng ban
        return newCur;
    };

    // Tự động điền tên phòng ban khi mã phòng ban thay đổi
    auto autoFillTenPhongBan = [&]() {
        if (idxMaPB != -1 && idxTenPB != -1) {
            string ma = data[idxMaPB];
            auto it = danhSachPhong.find(ma);
            if (it != danhSachPhong.end())
                data[idxTenPB] = it->second;
            else
                data[idxTenPB] = "";
        }
    };

    // Nếu đang ở trường mã phòng ban, xác định selection initial
    if (idxMaPB != -1 && !data[idxMaPB].empty()) {
        for (int i = 0; i < dsMaPhong.size(); ++i)
            if (data[idxMaPB] == dsMaPhong[i]) comboBoxSelection = i;
    }

    drawAll();

    while (true) {
        if (cur == idxMaPB) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int key = _getch();
                if (key == 72) { // Up
                    comboBoxSelection = (comboBoxSelection - 1 + dsMaPhong.size()) % dsMaPhong.size();
                    drawField(cur, true);
                } else if (key == 80) { // Down
                    comboBoxSelection = (comboBoxSelection + 1) % dsMaPhong.size();
                    drawField(cur, true);
                }
            } else if (ch == 13) { // Enter chọn mã phòng ban
                data[idxMaPB] = dsMaPhong[comboBoxSelection];
                autoFillTenPhongBan();
                drawField(idxMaPB, true);
                drawField(idxTenPB, false);
                cur = nextField(cur, 1);
                drawAll();
            } else if (ch == 27) {
                clearScreen();
                return "";
            }
        } else {
            int inputX = inputPositions[cur] + data[cur].length();
            int inputY = startY + cur * fieldHeight + 1;
            gotoxy(inputX, inputY);
            setColor(14);

            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int key = _getch();
                int prevSelected = cur;
                if (key == 72) cur = nextField(cur, -1);
                if (key == 80) cur = nextField(cur, 1);
                if (prevSelected != cur) {
                    for (int i : {prevSelected, cur}) drawField(i, i == cur);
                }
            } else if (ch == 13) {
                if (cur == label.size() - 1) {
                    clearScreen();
                    return "OK";
                } else {
                    drawField(cur, false);
                    cur = nextField(cur, 1);
                    drawField(cur, true);
                }
            } else if (ch == 27) {
                clearScreen();
                return "";
            } else if (ch == 8) {
                if (!data[cur].empty()) {
                    data[cur].pop_back();
                    gotoxy(inputPositions[cur], startY + cur * fieldHeight + 1);
                    cout << string(data[cur].length() + 1, ' ');
                    gotoxy(inputPositions[cur], startY + cur * fieldHeight + 1);
                    cout << data[cur];
                    gotoxy(inputPositions[cur] + data[cur].length(), startY + cur * fieldHeight + 1);
                }
            } else if (ch >= 32 && ch <= 126) {
                if (data[cur].length() < 50) {
                    if (label[cur] == "Luong (nghin VND)" && !isdigit(ch) && ch != '.') continue;
                    if (label[cur] == "Ma nhan vien (8 chu so)" && !isalnum(ch)) continue;
                    data[cur].push_back((char)ch);
                    cout << (char)ch;
                }
            }
        }
    }
}

void themMoiNhanVien(vector<NhanVien>& ds) {
    vector<string> label = {
        "Ma phong ban", "Ten phong ban", "Ma nhan vien (8 chu so)", "Ho ten", "Chuc vu",
        "Ngay sinh (dd mm yyyy)", "Luong (nghin VND)"
    };
    vector<string> data(7);
    vector<bool> invalidFields;

    while (true) {
        string result = nhapVaChinhSuaTrenConsole(label, data, invalidFields, ds);
        if (result.empty()) return;

        NhanVien nv;
        nv.maPhongBan = inHoaChuoi(trim(data[0]));
        nv.tenPhongBan = chuanHoaTen(trim(data[1]));
        nv.maNhanVien = trim(data[2]);
        nv.hoTen = chuanHoaTen(trim(data[3]));
        nv.chucVu = chuanHoaTen(trim(data[4]));

        invalidFields.clear();
        invalidFields.resize(label.size(), false);
        bool hasError = false;

        for (int i = 0; i < data.size(); i++) {
            if (trim(data[i]).empty()) {
                invalidFields[i] = true;
                hasError = true;
            }
        }

        if (isDuplicateID(ds, nv.maNhanVien)) {
            invalidFields[2] = true;
            hasError = true;
        }

        stringstream ss(data[5]);
        if (!(ss >> nv.ngaySinh.day >> nv.ngaySinh.month >> nv.ngaySinh.year) || !ss.eof()) {
            invalidFields[5] = true;
            hasError = true;
        } else if (!ngayHopLe(nv.ngaySinh) || !tuoiHopLe(nv.ngaySinh)) {
            invalidFields[5] = true;
            hasError = true;
        }

        try {
            nv.luong = stod(trim(data[6]));
            if (!luongHopLe(nv.luong)) {
                invalidFields[6] = true;
                hasError = true;
            }
        } catch (const std::invalid_argument&) {
            invalidFields[6] = true;
            hasError = true;
        } catch (const std::out_of_range&) {
            invalidFields[6] = true;
            hasError = true;
        }

        if (!maNhanVienHopLe(nv)) {
            invalidFields[2] = true;
            hasError = true;
        }

        if (!hasError) {
            ds.push_back(nv);
            if (!ghiFile(ds, "nhanvien.dat")) {
                cout << "Lỗi khi ghi file!\n";
                system("pause");
                return;
            }
            clearScreen();
            cout << "Da them thanh cong!\n";
            system("pause");
            return;
        }
    }
}

void inDanhSach(const vector<NhanVien>& ds, const string& note) {
    const int perPage = 10;
    int totalPage = (ds.size() + perPage - 1) / perPage;
    int currentPage = 0;

    // Các chiều rộng cột đã tăng để đủ cho dữ liệu tiếng Việt
    const int widthMa = 12;
    const int widthTen = 25;
    const int widthPhong = 15;
    const int widthTenPhong = 25;
    const int widthChucVu = 18;
    const int widthNgaySinh = 14;
    const int widthLuong = 20;

    auto printSeparator = [&]() {
        cout << "+";
        cout << string(widthMa, '-') << "+";
        cout << string(widthTen, '-') << "+";
        cout << string(widthPhong, '-') << "+";
        cout << string(widthTenPhong, '-') << "+";
        cout << string(widthChucVu, '-') << "+";
        cout << string(widthNgaySinh, '-') << "+";
        cout << string(widthLuong, '-') << "+\n";
    };

    auto formatSalary = [](double salary) {
        double salaryInThousands = salary * 1000;
        stringstream ss;
        ss.imbue(locale(""));
        ss << fixed << setprecision(0) << salaryInThousands << " VND";
        return ss.str();
    };

    while (true) {
        clearScreen();

        if (ds.empty()) {
            cout << "Danh sách nhân viên trống!\n";
            system("pause");
            return;
        }

        // Tiêu đề bảng căn giữa
        string title = "=== DANH SACH NHAN VIEN (Trang " + to_string(currentPage + 1) + "/" + to_string(max(totalPage, 1)) + ") ===";
        int tableWidth = widthMa + widthTen + widthPhong + widthTenPhong + widthChucVu + widthNgaySinh + widthLuong + 8;
        int titleX = max(0, (tableWidth - (int)title.length()) / 2);
        gotoxy(titleX, 1);
        setColor(14);
        cout << title << endl;
        setColor(7);

        if (!note.empty()) {
            setColor(11);
            cout << note << endl;
            setColor(7);
        }

        printSeparator();

        // Header nổi bật
        setColor(11);
        cout << "|"
             << setw(widthMa)      << left << "Ma NV"      << "|"
             << setw(widthTen)     << left << "Ho Ten"     << "|"
             << setw(widthPhong)   << left << "Ma Phong"   << "|"
             << setw(widthTenPhong)<< left << "Ten Phong"  << "|"
             << setw(widthChucVu)  << left << "Chuc Vu"    << "|"
             << setw(widthNgaySinh)<< left << "Ngay Sinh"  << "|"
             << setw(widthLuong)   << right << "Luong"     << "|\n";
        setColor(7);

        printSeparator();

        int start = currentPage * perPage;
        int end = min((int)ds.size(), start + perPage);

        for (int i = start; i < end; ++i) {
            const auto& nv = ds[i];
            stringstream ss;
            ss << setfill('0') << setw(2) << nv.ngaySinh.day << "/"
               << setfill('0') << setw(2) << nv.ngaySinh.month << "/"
               << setw(4) << nv.ngaySinh.year;

            cout << "|"
                 << setw(widthMa)      << left  << nv.maNhanVien
                 << "|"
                 << setw(widthTen)     << left  << nv.hoTen
                 << "|"
                 << setw(widthPhong)   << left  << nv.maPhongBan
                 << "|"
                 << setw(widthTenPhong)<< left  << nv.tenPhongBan
                 << "|"
                 << setw(widthChucVu)  << left  << nv.chucVu
                 << "|"
                 << setw(widthNgaySinh)<< left  << ss.str()
                 << "|"
                 << setw(widthLuong)   << right << formatSalary(nv.luong)
                 << "|\n";
        }

        printSeparator();
        cout << "[A] Trang truoc  |  [D] Trang sau  |  [ESC] Thoat\n";

        char ch = _getch();
        if (ch == 27) break;
        else if ((ch == 'a' || ch == 'A') && totalPage > 1) {
            currentPage = (currentPage - 1 + totalPage) % totalPage;
        } else if ((ch == 'd' || ch == 'D') && totalPage > 1) {
            currentPage = (currentPage + 1) % totalPage;
        }
    }
}

bool ghiFile(const vector<NhanVien>& ds, const string& filename) {
    ofstream f(filename, ios::binary);
    if (!f) return false;
    int size = ds.size();
    f.write((char*)&size, sizeof(int));
    for (const auto& nv : ds) {
        vector<string> fields = {
            nv.maPhongBan, nv.tenPhongBan, nv.maNhanVien,
            nv.hoTen, nv.chucVu
        };
        for (auto& s : fields) {
            int len = s.size();
            f.write((char*)&len, sizeof(int));
            f.write(s.c_str(), len);
        }
        f.write((char*)&nv.ngaySinh, sizeof(Date));
        f.write((char*)&nv.luong, sizeof(double));
    }
    return f.good();
}

bool docFile(vector<NhanVien>& ds, const string& filename) {
    ds.clear();
    ifstream f(filename, ios::binary);
    if (!f) return false;
    int size;
    f.read((char*)&size, sizeof(int));
    for (int i = 0; i < size && f; ++i) {
        NhanVien nv;
        for (int fi = 0; fi < 5; ++fi) {
            int len;
            f.read((char*)&len, sizeof(int));
            string str(len, '\0');
            f.read(&str[0], len);
            switch (fi) {
                case 0: nv.maPhongBan = str; break;
                case 1: nv.tenPhongBan = str; break;
                case 2: nv.maNhanVien = str; break;
                case 3: nv.hoTen = str; break;
                case 4: nv.chucVu = str; break;
            }
        }
        f.read((char*)&nv.ngaySinh, sizeof(Date));
        f.read((char*)&nv.luong, sizeof(double));
        ds.push_back(nv);
    }
    return true;
}

void xuLyHoSo(vector<NhanVien>& ds) {
    int choice = menuChon({
        "Them ho so",
        "Sua ho so",
        "Xoa ho so",
        "Quay lai"
    }, "=== XU LY HO SO ===");

    switch (choice) {
        case 0: themMoiNhanVien(ds); break;
        case 1: suaHoSo(ds); break;
        case 2: xoaHoSo(ds); break;
        default: break;
    }
}

void suaHoSo(vector<NhanVien>& ds) {
    if (ds.empty()) {
        cout << "Danh sach nhan vien trong!\n";
        system("pause");
        return;
    }

    while (true) {
        clearScreen();
        cout << "=== CHON NHAN VIEN CAN SUA ===\n";
        for (int i = 0; i < ds.size(); ++i) {
            cout << i + 1 << ". " << ds[i].maNhanVien << " - " << ds[i].hoTen << " - " << ds[i].chucVu << endl;
        }
        cout << "Nhap so thu tu nhan vien (0 de thoat): ";
        int chon;
        cin >> chon;
        cin.ignore();
        if (chon <= 0 || chon > ds.size()) break;

        NhanVien& nv = ds[chon - 1];
        vector<string> label = {
            "Ma phong ban", "Ten phong ban", "Ma nhan vien (8 chu so)", "Ho ten", "Chuc vu",
            "Ngay sinh (dd mm yyyy)", "Luong (nghin VND)"
        };
        // Dữ liệu gốc để sửa
        vector<string> data = {
            nv.maPhongBan, nv.tenPhongBan, nv.maNhanVien,
            nv.hoTen, nv.chucVu,
            to_string(nv.ngaySinh.day) + " " + to_string(nv.ngaySinh.month) + " " + to_string(nv.ngaySinh.year),
            to_string((int)nv.luong)
        };
        vector<bool> invalidFields;

        while (true) {
            string result = nhapVaChinhSuaTrenConsole(label, data, invalidFields, ds);
            if (result.empty()) return; // ESC hoặc hủy thì quay lại danh sách

            // Kiểm tra dữ liệu
            NhanVien temp = nv;
            temp.maPhongBan = inHoaChuoi(trim(data[0]));
            temp.tenPhongBan = chuanHoaTen(trim(data[1]));
            temp.maNhanVien = trim(data[2]);
            temp.hoTen = chuanHoaTen(trim(data[3]));
            temp.chucVu = chuanHoaTen(trim(data[4]));
            stringstream ss(data[5]);
            ss >> temp.ngaySinh.day >> temp.ngaySinh.month >> temp.ngaySinh.year;
            try { temp.luong = stod(trim(data[6])); }
            catch (...) { temp.luong = -1; }

            invalidFields.clear();
            invalidFields.resize(label.size(), false);
            bool hasError = false;
            for (int i = 0; i < data.size(); i++) {
                if (trim(data[i]).empty()) {
                    invalidFields[i] = true;
                    hasError = true;
                }
            }
            // Kiểm tra mã nhân viên trùng (trừ chính mình)
            for (int i = 0; i < ds.size(); ++i) {
                if (i != chon - 1 && ds[i].maNhanVien == temp.maNhanVien) {
                    invalidFields[2] = true;
                    hasError = true;
                    break;
                }
            }
            if (!ngayHopLe(temp.ngaySinh) || !tuoiHopLe(temp.ngaySinh)) {
                invalidFields[5] = true; hasError = true;
            }
            if (!luongHopLe(temp.luong)) {
                invalidFields[6] = true; hasError = true;
            }
            if (!maNhanVienHopLe(temp)) {
                invalidFields[2] = true; hasError = true;
            }

            if (!hasError) {
                // Xác nhận lưu
                clearScreen();
                cout << "Ban co muon luu thay doi khong? [Y/N]: ";
                char c = _getch();
                if (c == 'y' || c == 'Y') {
                    nv = temp;
                    if (ghiFile(ds, "nhanvien.dat")) {
                        cout << "\nDa cap nhat thanh cong!\n";
                        system("pause");
                    } else {
                        cout << "\nLoi khi luu file!\n";
                        system("pause");
                    }
                }
                break; // Quay về danh sách chọn nhân viên
            }
        }
    }
}

void showMessageBox(const string& msg, int color = 10) {
    int boxWidth = max(30, (int)msg.length() + 8);
    int boxX = 40, boxY = 12;
    setColor(7);
    gotoxy(boxX, boxY);     cout << char(201) << string(boxWidth, char(205)) << char(187);
    gotoxy(boxX, boxY + 1); cout << char(186) << string(boxWidth, ' ') << char(186);
    gotoxy(boxX, boxY + 2); cout << char(186) << string(boxWidth, ' ') << char(186);
    gotoxy(boxX, boxY + 3); cout << char(200) << string(boxWidth, char(205)) << char(188);
    setColor(color);
    gotoxy(boxX + 4, boxY + 1); cout << msg;
    setColor(7);
    gotoxy(boxX + 4, boxY + 2); cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

bool confirmBox(const string& msg) {
    int boxWidth = max(40, (int)msg.length() + 12);
    int boxX = 40, boxY = 10;
    int selected = 0; // 0: CÓ, 1: KHÔNG
    while (true) {
        // Vẽ box
        setColor(7);
        gotoxy(boxX, boxY);     cout << char(201) << string(boxWidth, char(205)) << char(187);
        gotoxy(boxX, boxY + 1); cout << char(186) << string(boxWidth, ' ') << char(186);
        gotoxy(boxX, boxY + 2); cout << char(186) << string(boxWidth, ' ') << char(186);
        gotoxy(boxX, boxY + 3); cout << char(200) << string(boxWidth, char(205)) << char(188);

        gotoxy(boxX + 4, boxY + 1); setColor(12); cout << msg;
        gotoxy(boxX + 4, boxY + 2);
        if (selected == 0) {
            setColor(240); cout << "[ CO ]"; setColor(7); cout << "   ";
            cout << "[ KHONG ]";
        } else {
            cout << "[ CO ]   "; setColor(240); cout << "[ KHONG ]"; setColor(7);
        }

        int ch = _getch();
        if (ch == 9 || ch == 0 || ch == 224) { // Tab hoặc mũi tên
            int k = (ch == 9) ? 77 : _getch(); // Tab = phím phải
            if (k == 77 || k == 9) selected = 1 - selected;
            if (k == 75) selected = 1 - selected;
        } else if (ch == 13) {
            return selected == 0;
        } else if (ch == 27) {
            return false; // ESC = KHÔNG
        }
    }
}

void xoaHoSo(vector<NhanVien>& ds) {
    if (ds.empty()) {
        showMessageBox("Danh sach nhan vien trong!", 12);
        return;
    }

    const int perPage = 10;
    int totalPage = (ds.size() + perPage - 1) / perPage;
    int currentPage = 0;
    int selectedRow = 0;

    const int widthMa = 12;
    const int widthTen = 20;
    const int widthPhong = 15;
    const int widthTenPhong = 20;
    const int widthChucVu = 15;
    const int widthNgaySinh = 12;
    const int widthLuong = 15;

    auto printSeparator = [&]() {
        cout << "+";
        cout << string(widthMa, '-') << "+";
        cout << string(widthTen, '-') << "+";
        cout << string(widthPhong, '-') << "+";
        cout << string(widthTenPhong, '-') << "+";
        cout << string(widthChucVu, '-') << "+";
        cout << string(widthNgaySinh, '-') << "+";
        cout << string(widthLuong, '-') << "+\n";
    };

    auto formatSalary = [](double salary) {
        double salaryInThousands = salary * 1000;
        stringstream ss;
        ss.imbue(locale(""));
        ss << fixed << setprecision(0) << salaryInThousands << " VND";
        return ss.str();
    };

    while (true) {
        system("cls");
        cout << "=== XOA HO SO NHAN VIEN (Trang " << (currentPage + 1) << "/" << totalPage << ") ===\n";
        cout << "Su dung phim mui ten de di chuyen, Enter de xoa, ESC de thoat\n";
        printSeparator();
        cout << "|"
             << setw(widthMa)      << left << "Ma NV"      << "|"
             << setw(widthTen)     << left << "Ho Ten"     << "|"
             << setw(widthPhong)   << left << "Ma Phong"   << "|"
             << setw(widthTenPhong)<< left << "Ten Phong"  << "|"
             << setw(widthChucVu)  << left << "Chuc Vu"    << "|"
             << setw(widthNgaySinh)<< left << "Ngay Sinh"  << "|"
             << setw(widthLuong)   << right << "Luong"     << "|\n";
        printSeparator();

        int start = currentPage * perPage;
        int end = min((int)ds.size(), start + perPage);

        for (int i = start; i < end; ++i) {
            const auto& nv = ds[i];
            stringstream ss;
            ss << setfill('0') << setw(2) << nv.ngaySinh.day << "/"
               << setfill('0') << setw(2) << nv.ngaySinh.month << "/"
               << setw(4) << nv.ngaySinh.year;

            if (i - start == selectedRow) {
                setColor(240); // Inverted colors for selection
            }

            cout << "|"
                 << setw(widthMa)     << left << nv.maNhanVien
                 << "|"
                 << setw(widthTen)    << left << nv.hoTen
                 << "|"
                 << setw(widthPhong)  << left << nv.maPhongBan
                 << "|"
                 << setw(widthTenPhong)<< left << nv.tenPhongBan
                 << "|"
                 << setw(widthChucVu) << left << nv.chucVu
                 << "|"
                 << setw(widthNgaySinh)<< left << ss.str()
                 << "|"
                 << setw(widthLuong)  << right << formatSalary(nv.luong)
                 << "|\n";
            setColor(7); // Reset color
        }

        printSeparator();
        cout << "[A] Trang truoc  |  [D] Trang sau  |  [ESC] Thoat\n";

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int key = _getch();
            if (key == 72) { // Up
                selectedRow = max(0, selectedRow - 1);
            } else if (key == 80) { // Down
                selectedRow = min(perPage - 1, selectedRow + 1);
                if (start + selectedRow >= (int)ds.size()) {
                    selectedRow = ds.size() - start - 1;
                }
            }
        } else if (ch == 13) { // Enter
            if (start + selectedRow >= ds.size()) continue;
            // Hộp xác nhận xóa
            string info = "Xoa nhan vien: " + ds[start + selectedRow].maNhanVien + " - " + ds[start + selectedRow].hoTen + "?";
            if (confirmBox(info)) {
                ds.erase(ds.begin() + start + selectedRow);
                ghiFile(ds, "nhanvien.dat");
                showMessageBox("DA XOA THANH CONG!", 10);
                // Cập nhật lại trạng thái phân trang và dòng chọn
                totalPage = (ds.size() + perPage - 1) / perPage;
                if (ds.empty()) return;
                if (currentPage >= totalPage) currentPage = max(0, totalPage - 1);
                if (selectedRow > 0 && (start + selectedRow) >= (int)ds.size()) {
                    selectedRow--;
                }
            } else {
                showMessageBox("DA HUY XOA!", 12);
            }
        } else if (ch == 27) { // ESC
            break;
        } else if (ch == 'a' || ch == 'A') {
            if (totalPage > 1) {
                currentPage = (currentPage - 1 + totalPage) % totalPage;
                selectedRow = 0;
            }
        } else if (ch == 'd' || ch == 'D') {
            if (totalPage > 1) {
                currentPage = (currentPage + 1) % totalPage;
                selectedRow = 0;
            }
        }
    }
}