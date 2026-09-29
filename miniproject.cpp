#include <iostream>
#include <string>
using namespace std;

const int MAX_USER = 100;
const int MAX_MOBIL = 100;
const int MAX_PESANAN = 100;
const int TAHUN_SKR = 2026;
const int BULAN_SKR = 9;
const int HARI_SKR = 29;
const int MIN_UMUR = 17;
const int MAX_UMUR = 70;

struct Mobil {
    string id;
    string nama;
    string model;
    string year;
    int kursi;
    double price;
    bool disewa;
};

struct User {
    string username;
    string password;
    string role;
};

struct Pesanan {
    string idPesanan;
    string username;
    string idMobil;
    string nik;
    int lamaSewa;
    double total;
};

Mobil daftarMobil[MAX_MOBIL];
User daftarUser[100];
Pesanan daftarPesanan[MAX_PESANAN];
int jumlahUser = 1;
int jumlahMobil = 0;
int jumlahPesanan = 0;
int userAktif = -1;

void registerUser(){
    string username;
    string password;
    bool duplikat = false;

    cout << "Username: ";
    cin >> username;

    for (int i = 0; i < jumlahUser; i++){
        if (daftarUser[i].username == username){
            duplikat = true;
            break;
        }
    }

    if (duplikat){
        cout << "Username sudah dipakai." << endl;
        return;
    }

    cout << "Password: ";
    cin >> password;

    if (jumlahUser >= MAX_USER){
        cout << "Kapasitas user penuh." << endl;
        return;
    }

    daftarUser[jumlahUser] = {
        username,
        password,
        "penyewa"
    };
    jumlahUser++;
    cout << "Registrasi berhasil." << endl;
}

string loginUser(){
    int percobaan = 0;

    while (percobaan < 3){
        string username;
        string password;
        int idx = -1;

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        for (int i = 0; i < jumlahUser; i++){
            if (daftarUser[i].username == username && daftarUser[i].password == password){
                idx = i;
                break;
            }
        }

        if (idx != -1){
            userAktif = idx;
            cout << "Login berhasil." << endl;
            return daftarUser[idx].role;
        }

        percobaan++;
        if (percobaan < 3){
            cout << "Username/password salah." << endl;
        }
    }

    cout << "Gagal 3x, kembali ke menu." << endl;
    return "";
}

bool cekUmur(string nik, bool& underage, bool& overage){
    underage = false;
    overage = false;

    if (nik.length() != 16)
        return false;

    for (char digit : nik){
        if (digit < '0' || digit > '9')
            return false;
    }

    int hari = stoi(nik.substr(6, 2));
    int bulan = stoi(nik.substr(8, 2));
    int thn2 = stoi(nik.substr(10, 2));

    if (hari > 40)
        hari -= 40;

    if (bulan < 1 || bulan > 12 || hari < 1 || hari > 31)
        return false;

    int thnLahir = thn2 <= TAHUN_SKR % 100
        ? 2000 + thn2
        : 1900 + thn2;
    int umur = TAHUN_SKR - thnLahir;

    if (bulan > BULAN_SKR || (bulan == BULAN_SKR && hari > HARI_SKR))
        umur--;

    if (umur < MIN_UMUR)
        underage = true;
    if (umur > MAX_UMUR)
        overage = true;

    return !underage && !overage;
}

void cariMobil(){
    string keyword;
    bool ketemu = false;

    cout << "Masukkan keyword mobil: ";
    cin >> keyword;

    for (int i = 0; i < jumlahMobil; i++){
        if (daftarMobil[i].model.find(keyword) != string::npos){
            cout << "ID: " << daftarMobil[i].id
                 << ", Model: " << daftarMobil[i].model
                 << ", Tahun: " << daftarMobil[i].year
                 << ", Harga: Rp" << daftarMobil[i].price
                 << ", Status: " << (daftarMobil[i].disewa ? "Disewa" : "Tersedia")
                 << endl;
            ketemu = true;
        }
    }

    if (!ketemu){
        cout << "Mobil tidak ditemukan." << endl;
    }
}

void detailMobil(){
    string idCari;
    int idx = -1;

    cout << "Masukkan ID mobil: ";
    cin >> idCari;

    for (int i = 0; i < jumlahMobil; i++){
        if (daftarMobil[i].id == idCari){
            idx = i;
            break;
        }
    }

    if (idx == -1){
        cout << "Mobil tidak ditemukan." << endl;
        return;
    }

    cout << "ID: " << daftarMobil[idx].id << endl;
    cout << "Nama: " << daftarMobil[idx].nama << endl;
    cout << "Model: " << daftarMobil[idx].model << endl;
    cout << "Tahun: " << daftarMobil[idx].year << endl;
    cout << "Kursi: " << daftarMobil[idx].kursi << endl;
    cout << "Harga: Rp" << daftarMobil[idx].price << "/hari" << endl;
    cout << "Status: " << (daftarMobil[idx].disewa ? "Disewa" : "Tersedia") << endl;
}

void sewaMobil(){
    string idCari;
    int idx = -1;
    cout << "Masukkan ID mobil: ";
    cin >> idCari;

    for (int i = 0; i < jumlahMobil; i++){
        if (daftarMobil[i].id == idCari){
            idx = i;
            break;
        }
    }

    if (idx == -1){
        cout << "Mobil tidak ditemukan." << endl;
        return;
    }

    if (daftarMobil[idx].disewa){
        cout << "Mobil sedang disewa." << endl;
        return;
    }

    int lamaSewa;
    cout << "Lama sewa (hari): ";
    cin >> lamaSewa;

    if (lamaSewa <= 0){
        cout << "Lama sewa harus lebih dari 0." << endl;
        return;
    }

    string nik;
    bool underage;
    bool overage;
    cout << "Masukkan NIK: ";
    cin >> nik;

    if (!cekUmur(nik, underage, overage)){
        if (underage)
            cout << "Umur belum memenuhi syarat." << endl;
        else if (overage)
            cout << "Umur melebihi batas syarat." << endl;
        else
            cout << "NIK tidak valid." << endl;
        return;
    }

    double total = daftarMobil[idx].price * lamaSewa;
    cout << "Total sewa: Rp" << total << endl;
    char konfirmasi;
    cout << "Konfirmasi sewa? [y/n]: ";
    cin >> konfirmasi;

    if (konfirmasi != 'y' && konfirmasi != 'Y'){
        cout << "Penyewaan dibatalkan." << endl;
        return;
    }

    daftarMobil[idx].disewa = true;

    daftarPesanan[jumlahPesanan] = {
        "P" + to_string(jumlahPesanan + 1),
        daftarUser[userAktif].username,
        daftarMobil[idx].id,
        nik,
        lamaSewa,
        total
    };
    jumlahPesanan++;
    cout << "Sewa mobil berhasil." << endl;
}

void tambahMobil(){
    if (jumlahMobil >= MAX_MOBIL){
        cout << "Data mobil penuh." << endl;
        return;
    }

    Mobil mobil;
    mobil.id = "M" + to_string(jumlahMobil + 1);
    mobil.disewa = false;

    cout << "Nama mobil: ";
    cin >> mobil.nama;
    cout << "Model mobil: ";
    cin >> mobil.model;
    cout << "Tahun mobil: ";
    cin >> mobil.year;
    cout << "Jumlah kursi: ";
    cin >> mobil.kursi;
    cout << "Harga sewa per hari: ";
    cin >> mobil.price;

    while (mobil.price <= 0){
        cout << "Harga harus > 0." << endl;
        cout << "Harga sewa per hari: ";
        cin >> mobil.price;
    }

    daftarMobil[jumlahMobil] = mobil;
    jumlahMobil++;
    cout << "Mobil berhasil ditambahkan." << endl;
}

void lihatPesanan(){
    if (jumlahPesanan == 0){
        cout << "Belum ada pesanan." << endl;
        return;
    }

    for (int i = 0; i < jumlahPesanan; i++){
        cout << "ID Pesanan: " << daftarPesanan[i].idPesanan << endl;
        cout << "Username: " << daftarPesanan[i].username << endl;
        cout << "ID Mobil: " << daftarPesanan[i].idMobil << endl;
        cout << "NIK: " << daftarPesanan[i].nik << endl;
        cout << "Lama Sewa: " << daftarPesanan[i].lamaSewa << " hari" << endl;
        cout << "Total: Rp" << daftarPesanan[i].total << endl;
        cout << endl;
    }
}

void menuAdmin(){
    int pilihan;

    do {
        cout << "=== MENU ADMIN ===" << endl;
        cout << "1. Tambah Mobil" << endl;
        cout << "2. Lihat Pesanan" << endl;
        cout << "0. Logout" << endl;
        cout << "Pilih Opsi: ";
        cin >> pilihan;

        switch (pilihan){
            case 1:
                tambahMobil();
                break;
            case 2:
                lihatPesanan();
                break;
            case 0:
                userAktif = -1;
                cout << "Logout berhasil." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 0);
}

void menuPenyewa(){
    int pilihan;

    do {
        cout << "=== MENU PENYEWA ===" << endl;
        cout << "1. Cari Mobil" << endl;
        cout << "2. Detail Mobil" << endl;
        cout << "3. Sewa Mobil" << endl;
        cout << "0. Logout" << endl;
        cout << "Pilih Opsi: ";
        cin >> pilihan;

        switch (pilihan){
            case 1:
                cariMobil();
                break;
            case 2:
                detailMobil();
                break;
            case 3:
                sewaMobil();
                break;
            case 0:
                userAktif = -1;
                cout << "Logout berhasil." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 0);
}

int main(){
    daftarUser[0] = {
        "admin",
        "123",
        "admin"
    };
    bool running = true;
    while (running){
        int option;
        cout << "=== SEWA MOBIL JOGJA ===" << endl;
        cout << "[1] Register" << endl;
        cout << "[2] Login" << endl;
        cout << "[3] Keluar" << endl;
        cout << "Pilih Opsi: ";
        cin >> option;
        switch (option) {
            case 1:
                registerUser();
                break;
            case 2:
                {
                    string role = loginUser();
                    if (role == "admin")
                        menuAdmin();
                    else if (role == "penyewa")
                        menuPenyewa();
                }
                break;
            case 3:
                running = false;
                break;
            default:
                cout << "Opsi " << option << " tidak ditemukan.";
                break;
        };
    }
    cout << "Terimakasih sudah mengunjungi kami";
}