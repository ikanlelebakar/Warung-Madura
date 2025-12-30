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

    void loadFromJson(const std::string& fileName);
    void saveToJson(const std::string& fileName);
    void initDatabase(const std::string& fileName);
    void reindexCodes();
};

extern std::vector<Database::barang> datasetBarang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_DATABASE_H