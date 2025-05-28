#include"ChucNang.h"
#include"menu.h"
#include <iomanip>
#include <map>

string layTen(const string& hoTen) {
    size_t pos = hoTen.find_last_of(' ');
    if (pos != string::npos) {
        return hoTen.substr(pos + 1);
    }
    return hoTen;
}

bool soSanhMaNhanVien(const NhanVien& a, const NhanVien& b){
	return a.maNhanVien<b.maNhanVien;
}

bool soSanhMaPB(const NhanVien& a, const NhanVien& b) {
    return a.maPhongBan < b.maPhongBan;
}

bool soSanhHoTen(const NhanVien& a, const NhanVien& b) {
    string tenA = layTen(a.hoTen);
    string tenB = layTen(b.hoTen);
    if (tenA != tenB) return tenA < tenB;
    return a.hoTen < b.hoTen;
}

bool soSanhNgaySinh(const NhanVien& a, const NhanVien& b) {
    if (a.ngaySinh.year != b.ngaySinh.year) 
        return a.ngaySinh.year < b.ngaySinh.year;
    if (a.ngaySinh.month != b.ngaySinh.month) 
        return a.ngaySinh.month < b.ngaySinh.month;
    return a.ngaySinh.day < b.ngaySinh.day;
}
bool soSanhLuong(const NhanVien& a, const NhanVien& b){
    return a.luong < b.luong;
}

// So sánh động nhiều tiêu chí
bool SoSanhNhieuTieuChi::operator()(const NhanVien& a, const NhanVien& b) const {
    for (int idx : tieuChi) {
        switch (idx) {
            case 0: // Ma phong ban
                if (a.maPhongBan != b.maPhongBan)
                    return a.maPhongBan < b.maPhongBan;
                break;
            case 1: // Ho ten
                if (a.hoTen != b.hoTen)
                    return a.hoTen < b.hoTen;
                break;
            case 2: // Ngay sinh
                if (a.ngaySinh.year != b.ngaySinh.year)
                    return a.ngaySinh.year < b.ngaySinh.year;
                if (a.ngaySinh.month != b.ngaySinh.month)
                    return a.ngaySinh.month < b.ngaySinh.month;
                if (a.ngaySinh.day != b.ngaySinh.day)
                    return a.ngaySinh.day < b.ngaySinh.day;
                break;
            case 3: // Luong
                if (a.luong != b.luong)
                    return a.luong < b.luong;
                break;
        }
    }
    return false;
}

// Selection Sort
void selectionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    for (size_t i = 0; i < ds.size() - 1; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < ds.size(); ++j) {
            if (cmp(ds[j], ds[minIdx])) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(ds[i], ds[minIdx]);
        }
    }
}

// Insertion Sort
void insertionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    for (size_t i = 1; i < ds.size(); ++i) {
        NhanVien key = ds[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, ds[j])) {
            ds[j + 1] = ds[j];
            j--;
        }
        ds[j + 1] = key;
    }
}

// Quick Sort
int partition(vector<NhanVien>& ds, int low, int high, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    NhanVien pivot = ds[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (cmp(ds[j], pivot)) {
            i++;
            swap(ds[i], ds[j]);
        }
    }
    swap(ds[i + 1], ds[high]);
    return i + 1;
}

void quickSort(vector<NhanVien>& ds, int low, int high, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    if (low < high) {
        int pi = partition(ds, low, high, cmp);
        quickSort(ds, low, pi - 1, cmp);
        quickSort(ds, pi + 1, high, cmp);
    }
}

// Merge Sort
void merge(vector<NhanVien>& ds, int left, int mid, int right, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<NhanVien> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
        L[i] = ds[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = ds[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp(L[i], R[j])) {
            ds[k] = L[i];
            i++;
        } else {
            ds[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        ds[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        ds[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(vector<NhanVien>& ds, int left, int right, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(ds, left, mid, cmp);
        mergeSort(ds, mid + 1, right, cmp);
        merge(ds, left, mid, right, cmp);
    }
}

// Sắp xếp tổng quát
void sapXep(vector<NhanVien>& ds) {
    int algo = menuChon({
        "Selection Sort",
        "Insertion Sort",
        "Quick Sort",
        "Merge Sort"
    }, "=== CHON THUAT TOAN SAP XEP ===");

    int khoa = menuChon({
        "Ma phong ban",
        "Ho ten",
        "Ngay sinh",
        "Luong",
        "Ma nhan vien",
        "Nhieu tieu chi"
    }, "=== CHON KHOA SAP XEP ===");

    function<bool(const NhanVien&, const NhanVien&)> cmp;
    vector<int> chonTieuChi;

    if (khoa == 5) {
       
        vector<string> cacTieuChi = {
            "Ma phong ban",
            "Ho ten",
            "Ngay sinh",
            "Ma nhan vien",
            "Luong"
        };
      
        vector<bool> daChon(cacTieuChi.size(), false);

        for (int i = 0; i < 5; ++i) {
            vector<string> menuTieuChi;
            for (int j = 0; j < cacTieuChi.size(); ++j)
                if (!daChon[j]) menuTieuChi.push_back(cacTieuChi[j]);
            int chon = menuChon(menuTieuChi, "Chon tieu chi sap xep thu " + to_string(i + 1));
            if (chon < 0) break;
            int idx = 0, cnt = -1;
            for (; idx < cacTieuChi.size(); ++idx)
                if (!daChon[idx] && ++cnt == chon) break;
            chonTieuChi.push_back(idx);
            daChon[idx] = true;
        }
        SoSanhNhieuTieuChi soSanh{chonTieuChi};
        cmp = soSanh;
    } else {
        switch (khoa) {
            case 0: cmp = soSanhMaPB; break;
            case 1: cmp = soSanhHoTen; break;
            case 2: cmp = soSanhNgaySinh; break;
            case 3: cmp = soSanhMaNhanVien; break;
            case 4: cmp = soSanhLuong; break;
        }
    }

    // Gọi thuật toán đã chọn
    switch (algo) {
        case 0: selectionSort(ds, cmp); break;
        case 1: insertionSort(ds, cmp); break;
        case 2: quickSort(ds, 0, ds.size() - 1, cmp); break;
        case 3: mergeSort(ds, 0, ds.size() - 1, cmp); break;
    }
    string note = "Dang sap xep theo: ";
		if (khoa == 0) note += "Ma phong ban";
		else if (khoa == 1) note += "Ho ten";
		else if (khoa == 2) note += "Ngay sinh";
		else if (khoa == 3) note += "Ma nhan vien";
		else if (khoa == 4) note += "Luong";
		else if (khoa == 5) note += "Nhieu tieu chi (";
		if (khoa == 5) {
    for (int i = 0; i < chonTieuChi.size(); ++i) {
        if (chonTieuChi[i] == 0) note += "Ma phong ban";
        else if (chonTieuChi[i] == 1) note += "Ho ten";
        else if (chonTieuChi[i] == 2) note += "Ngay sinh";
        else if (chonTieuChi[i] == 4) note += "Ma nhan vien";
        else if (chonTieuChi[i] == 3) note += "Luong";
        if (i < chonTieuChi.size() - 1) note += " -> ";
    }
    note += ")";
}

    clearScreen();
    inDanhSach(ds, note);
}

vector<NhanVien> timKiemChinhXac(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = false;
        switch (khoa) {
            case 0: // Ma phong ban
                match = (nv.maPhongBan == inHoaChuoi(giaTriChuan));
                break;
            case 1: // Ma nhan vien
                match = (nv.maNhanVien == giaTriChuan);
                break;
            case 2: // Ho ten
                match = (nv.hoTen == chuanHoaTen(giaTriChuan));
                break;
            case 3: { // Ngay sinh
                stringstream ss(giaTriChuan);
                int day, month, year;
                if (ss >> day >> month >> year) {
                    match = (nv.ngaySinh.day == day && 
                            nv.ngaySinh.month == month && 
                            nv.ngaySinh.year == year);
                }
                break;
            }
            case 4: { // Luong
                try {
                    double luong = stod(giaTriChuan);
                    match = (nv.luong == luong);
                } catch (...) {}
                break;
            }
        }
        if (match) {
            ketQua.push_back(nv);
            break;
        }
    }
    return ketQua;
}

vector<NhanVien> timKiemGanDung(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = false;
        switch (khoa) {
            case 0: // Ma phong ban
                match = (nv.maPhongBan.find(inHoaChuoi(giaTriChuan)) != string::npos);
                break;
            case 1: // Ma nhan vien
                match = (nv.maNhanVien.find(giaTriChuan) != string::npos);
                break;
            case 2: // Ho ten
                match = (nv.hoTen.find(chuanHoaTen(giaTriChuan)) != string::npos);
                break;
            case 3: { // Ngay sinh
                stringstream ss;
                ss << setfill('0') << setw(2) << nv.ngaySinh.day << " "
                   << setfill('0') << setw(2) << nv.ngaySinh.month << " "
                   << setw(4) << nv.ngaySinh.year;
                string ngaySinhStr = ss.str();
                match = (ngaySinhStr.find(giaTriChuan) != string::npos);
                break;
            }
            case 4: { // Luong
                string luongStr = to_string(nv.luong);
                match = (luongStr.find(giaTriChuan) != string::npos);
                break;
            }
        }
        if (match) {
            ketQua.push_back(nv);
        }
    }
    return ketQua;
}

vector<NhanVien> timKiemToanBo(const vector<NhanVien>& ds, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = (nv.maPhongBan.find(giaTriChuan) != string::npos ||
                     nv.tenPhongBan.find(giaTriChuan) != string::npos ||
                     nv.maNhanVien.find(giaTriChuan) != string::npos ||
                     nv.hoTen.find(giaTriChuan) != string::npos ||
                     nv.chucVu.find(giaTriChuan) != string::npos);
        
        // Kiểm tra ngày sinh
        if (!match) {
            stringstream ss;
            ss << setfill('0') << setw(2) << nv.ngaySinh.day << " "
               << setfill('0') << setw(2) << nv.ngaySinh.month << " "
               << setw(4) << nv.ngaySinh.year;
            string ngaySinhStr = ss.str();
            match = (ngaySinhStr.find(giaTriChuan) != string::npos);
        }
        
        // Kiểm tra lương
        if (!match) {
            string luongStr = to_string(nv.luong);
            match = (luongStr.find(giaTriChuan) != string::npos);
        }
        
        if (match) {
            ketQua.push_back(nv);
        }
    }
    return ketQua;
}
vector<NhanVien> timKiemNhiPhan(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    
    // Tạo một bản sao đã sắp xếp của danh sách
    vector<NhanVien> dsSapXep = ds;
    
    // Sắp xếp theo khóa tương ứng
    function<bool(const NhanVien&, const NhanVien&)> cmp;
    switch (khoa) {
        case 0: // Ma phong ban
            sort(dsSapXep.begin(), dsSapXep.end(), soSanhMaPB);
            cmp = soSanhMaPB;
            break;
        case 1: // Ma nhan vien
            sort(dsSapXep.begin(), dsSapXep.end(), soSanhMaNhanVien);
            cmp = soSanhMaNhanVien;
            break;
        case 2: // Ho ten
            sort(dsSapXep.begin(), dsSapXep.end(), soSanhHoTen);
            cmp = soSanhHoTen;
            break;
        case 3: // Ngay sinh
            sort(dsSapXep.begin(), dsSapXep.end(), soSanhNgaySinh);
            cmp = soSanhNgaySinh;
            break;
        case 4: // Luong
            sort(dsSapXep.begin(), dsSapXep.end(), soSanhLuong);
            cmp = soSanhLuong;
            break;
    }

    // Tạo một nhân viên giả để so sánh
    NhanVien target;
    switch (khoa) {
        case 0: target.maPhongBan = giaTriChuan; break;
        case 1: target.maNhanVien = giaTriChuan; break;
        case 2: target.hoTen = giaTriChuan; break;
        case 3: {
            stringstream ss(giaTriChuan);
            ss >> target.ngaySinh.day >> target.ngaySinh.month >> target.ngaySinh.year;
            break;
        }
        case 4: {
            try {
                target.luong = stod(giaTriChuan);
            } catch (...) {
                return ketQua;
            }
            break;
        }
    }

    // Thực hiện tìm kiếm nhị phân
    auto lower = lower_bound(dsSapXep.begin(), dsSapXep.end(), target, cmp);
    auto upper = upper_bound(dsSapXep.begin(), dsSapXep.end(), target, cmp);

    for (auto it = lower; it != upper; ++it) {
        ketQua.push_back(*it);
    }

    return ketQua;
}

vector<NhanVien> timKiemTuanTu(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    return timKiemChinhXac(ds, khoa, giaTri);
}

void timKiem(const vector<NhanVien>& ds) {
	int phuongPhap = menuChon({
        "Tim kiem tuan tu",
        "Tim kiem nhi phan"
    }, "=== CHON PHUONG PHAP TIM KIEM ===");
    
    if (phuongPhap == -1) return;
    
    int mode = menuChon({
        "Tim kiem chinh xac",
        "Tim kiem gan dung",
        "Tim kiem toan bo"
    }, "=== CHON KIEU TIM KIEM ===");
    
    if (mode == -1) return;

    int khoa = -1;
    string giaTri;
    
    if (mode != 2) {
        khoa = menuChon({
            "Ma phong ban",
            "Ma nhan vien",
            "Ho ten",
            "Ngay sinh (dd mm yyyy)",
            "Luong"
        }, "=== CHON KHOA TIM KIEM ===");
        
        if (khoa == -1) return;
    }

    //cin.ignore();
    gotoxy(1,25);
    cout << "Nhap gia tri can tim: ";
    getline(cin, giaTri);

    vector<NhanVien> ketQua;
    if (phuongPhap == 0) {
        // Tìm kiếm tuần tự
        switch (mode) {
            case 0: ketQua = timKiemChinhXac(ds, khoa, giaTri); break;
            case 1: ketQua = timKiemGanDung(ds, khoa, giaTri); break;
            case 2: ketQua = timKiemToanBo(ds, giaTri); break;
        }
    } else {
        // Tìm kiếm nhị phân
        switch (mode) {
            case 0: ketQua = timKiemNhiPhan(ds, khoa, giaTri); break;
            case 1: 
                // Tìm kiếm gần đúng không hỗ trợ nhị phân, chuyển sang tuần tự
                ketQua = timKiemGanDung(ds, khoa, giaTri); 
                break;
            case 2: 
                // Tìm kiếm toàn bộ không hỗ trợ nhị phân, chuyển sang tuần tự
                ketQua = timKiemToanBo(ds, giaTri); 
                break;
        }
    }

    clearScreen();
    if (ketQua.empty()) {
        cout << "Khong tim thay ket qua nao!\n";
    } else {
        string note = "Ket qua tim kiem: ";
        switch (mode) {
            case 0: note += "Chinh xac "; break;
            case 1: note += "Gan dung "; break;
            case 2: note += "Toan bo "; break;
        }
        note += "(Phuong phap: ";
        note += (phuongPhap == 0) ? "Tuan tu" : "Nhi phan";
        note += ") ";
        
        if (mode != 2) {
            note += "theo ";
            switch (khoa) {
                case 0: note += "ma phong ban"; break;
                case 1: note += "ma nhan vien"; break;
                case 2: note += "ho ten"; break;
                case 3: note += "ngay sinh"; break;
                case 4: note += "luong"; break;
            }
        }
        inDanhSach(ketQua, note);
    }
    _getwch();
}

void thongKe(const vector<NhanVien>& ds) {
    int luaChon = menuChon({
        "So luong NV theo phong ban",
        "Ty le NV theo muc luong"
    }, "=== THONG KE ===");
    clearScreen();

    if (luaChon == 0) {
        map<string, int> thongKePB;
        for (const auto& nv : ds) {
            thongKePB[nv.maPhongBan]++;
        }
        cout << left << setw(20) << "Phong ban" << "So luong NV\n";
        for (auto& p : thongKePB) {
            cout << left << setw(20) << p.first << p.second << "\n";
        }
    } else {
        int duoi5 = 0, tu5den10 = 0, tren10 = 0;
        for (auto& nv : ds) {
            if (nv.luong < 5000000) duoi5++;
            else if (nv.luong <= 10000000) tu5den10++;
            else tren10++;
        }

        int tong = ds.size();
        cout << "Duoi 5tr: " << duoi5 << " NV (" << (100.0 * duoi5 / tong) << "%)\n";
        cout << "Tu 5tr - 10tr: " << tu5den10 << " NV (" << (100.0 * tu5den10 / tong) << "%)\n";
        cout << "Tren 10tr: " << tren10 << " NV (" << (100.0 * tren10 / tong) << "%)\n";
    }

    system("pause");
}