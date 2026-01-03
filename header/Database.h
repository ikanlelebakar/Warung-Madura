#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_DATABASE_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_DATABASE_H

#include <string>
#include <vector>

class Database {
public:
    struct barang {
        std::string nama;
        int codeBarang, jumlahBarang;
        double hargaBarang;
    };

    struct Transaksi {
        std::string id;           // Unique ID (timestamp-based)
        std::string tanggal;      // Format: "DD-MM-YYYY"
        std::string waktu;        // Format: "HH:MM:SS"
        std::string jenis;        // "pemasukan" atau "pengeluaran"
        std::string keterangan;   // Deskripsi transaksi
        double jumlah;            // Nominal transaksi
        std::string metodePembayaran;  // "cash" atau "cashless"
    };

    // Barang functions
    void tampilBarang();
    void loadFromJson(const std::string& fileName);
    void saveToJson(const std::string& fileName);
    bool updateStok(int codeBarang, int stokBaru);
    void initDatabase(const std::string& fileName);

    // Transaksi functions
    void loadTransaksi(const std::string& fileName);
    void saveTransaksi(const std::string& fileName);
    void initTransaksi(const std::string& fileName);
    void tambahTransaksi(const Transaksi& t);
};

extern std::vector<Database::barang> datasetBarang;
extern std::vector<Database::Transaksi> datasetTransaksi;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_DATABASE_H