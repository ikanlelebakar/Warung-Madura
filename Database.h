//
// Created by User on 12/18/2025.
//

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
        void tampilBarang();
};

extern std::vector<Database::barang> datasetBarang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_DATABASE_H