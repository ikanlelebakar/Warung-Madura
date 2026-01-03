/**
 * ============================================================
 * HEADER STOCK - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : Stock.h
 * Deskripsi   : Deklarasi class Stock untuk mengelola stok barang
 * ============================================================
 */

#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H

/**
 * Class Stock
 * Menangani operasi terkait stok barang seperti:
 * - Menampilkan daftar stok
 * - Menambah barang baru
 * - Mengubah data barang
 * - Menghapus barang
 * - Sorting berdasarkan jumlah
 */
class Stock {
    public:
        // Fungsi untuk menampilkan menu
        void menuStock();       // Menu utama stock
        void menuEdit();        // Menu edit stock
        void menuSort();        // Menu sorting/daftar
        
        // Fungsi untuk navigasi menu
        int pilihMenu();        // Handler menu utama stock
        int pilihEditMenu();    // Handler menu edit
        int sortFunction();     // Handler sorting & tampil daftar
        
        // Fungsi operasi CRUD barang
        int ubahStock();        // Ubah data barang existing
        int tambahBarang();     // Tambah barang baru
        int hapusBarang();      // Hapus barang
        
    private:
        int Pilihan;            // Menyimpan pilihan menu user
};

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_STOCK_H