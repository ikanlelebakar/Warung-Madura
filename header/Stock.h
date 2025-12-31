#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H


class Stock {
    public:
        void menuStock(), menuEdit(), menuSort();
        int pilihMenu(), kembali(), pilihEditMenu(), ubahStock(), tambahBarang(), hapusBarang(), sortFunction();
    private:
        char pilihanKembali;
        int Pilihan;
};


#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H