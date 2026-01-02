#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KEUANGAN_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KEUANGAN_H


class Keuangan {
public:
    // Existing menu functions
    void menuKeuangan(), menuLaporan();
    int pilihMenu(), pilihLaporan();
    
    // Financial calculation functions
    double hitungTotalPemasukan();
    double hitungTotalPengeluaran();
    double hitungLabaBersih();
    
    // Display functions
    void tampilRingkasanKeuangan();
    void tampilRincianPemasukan();
    void tampilRincianPengeluaran();
    
    // Export function
    void exportToCSV();
    
private:
    int Pilihan;
};


#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_KEUANGAN_H