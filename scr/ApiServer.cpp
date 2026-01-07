/**
 * ============================================================
 * API SERVER - Sistem Pencatatan Warung Madura (Web Version)
 * ============================================================
 * File        : ApiServer.cpp
 * Deskripsi   : Implementasi HTTP server REST API
 *               Menghubungkan frontend web dengan logic C++ backend
 * ============================================================
 */

// === HEADER MODUL INTERNAL ===
#include "../header/ApiServer.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"

// === EXTERNAL LIBRARY ===
#include "json.hpp"

// === STANDARD LIBRARY ===
#include <iostream>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

// ============================================================
// HELPER FUNCTIONS
// ============================================================

/**
 * Set header CORS untuk mengizinkan akses dari frontend
 * Diperlukan ketika frontend diakses dari file:// atau origin berbeda
 */
void ApiServer::setCorsHeaders(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

/**
 * Mengirim response error dalam format JSON
 */
void ApiServer::sendError(httplib::Response& res, int status, const std::string& message) {
    setCorsHeaders(res);
    res.status = status;
    json response;
    response["success"] = false;
    response["error"] = message;
    res.set_content(response.dump(), "application/json");
}

/**
 * Mengirim response sukses dalam format JSON
 */
void ApiServer::sendSuccess(httplib::Response& res, const std::string& jsonData) {
    setCorsHeaders(res);
    res.status = 200;
    res.set_content(jsonData, "application/json");
}

// ============================================================
// API HANDLERS - BARANG
// ============================================================

/**
 * GET /api/barang
 * Mengembalikan semua data barang dalam format JSON
 */
void ApiServer::handleGetBarang(const httplib::Request& req, httplib::Response& res) {
    Database database;
    database.loadFromJson(getDatabasePath());
    
    json response;
    response["success"] = true;
    response["data"] = json::array();
    
    for (const auto& b : datasetBarang) {
        json item;
        item["nama"] = b.nama;
        item["codeBarang"] = b.codeBarang;
        item["jumlahBarang"] = b.jumlahBarang;
        item["hargaBarang"] = b.hargaBarang;
        item["hargaJual"] = b.hargaJual;
        response["data"].push_back(item);
    }
    
    sendSuccess(res, response.dump());
}

/**
 * POST /api/barang
 * Menambah barang baru ke database
 * Body: { "nama": "...", "jumlahBarang": ..., "hargaBarang": ... }
 */
void ApiServer::handleAddBarang(const httplib::Request& req, httplib::Response& res) {
    try {
        json body = json::parse(req.body);
        
        Database database;
        database.loadFromJson(getDatabasePath());
        
        // Validasi input
        if (!body.contains("nama") || !body.contains("jumlahBarang") || !body.contains("hargaBarang")) {
            sendError(res, 400, "Field nama, jumlahBarang, dan hargaBarang wajib diisi");
            return;
        }
        
        // Cek duplikat nama
        std::string namaInput = body["nama"];
        for (const auto& b : datasetBarang) {
            if (b.nama == namaInput) {
                sendError(res, 400, "Barang dengan nama tersebut sudah ada");
                return;
            }
        }
        
        // Buat barang baru
        Database::barang barangBaru;
        barangBaru.nama = namaInput;
        barangBaru.jumlahBarang = body["jumlahBarang"];
        barangBaru.hargaBarang = body["hargaBarang"];
        barangBaru.hargaJual = hitungHargaJual(barangBaru.hargaBarang);
        
        // Generate code barang otomatis
        int maxCode = 999;
        for (const auto& b : datasetBarang) {
            if (b.codeBarang > maxCode) {
                maxCode = b.codeBarang;
            }
        }
        barangBaru.codeBarang = maxCode + 1;
        
        // Simpan ke dataset dan file
        datasetBarang.push_back(barangBaru);
        database.saveToJson(getDatabasePath());
        
        // Catat sebagai transaksi pengeluaran
        Database::Transaksi trans;
        trans.id = generateTransactionId();
        trans.tanggal = getCurrentDate();
        trans.waktu = getCurrentTime();
        trans.jenis = "pengeluaran";
        trans.keterangan = barangBaru.nama;
        trans.jumlah = barangBaru.hargaBarang * barangBaru.jumlahBarang;
        trans.metodePembayaran = "";
        
        database.loadTransaksi(getTransaksiPath());
        database.tambahTransaksi(trans);
        database.saveTransaksi(getTransaksiPath());
        
        // Response sukses
        json response;
        response["success"] = true;
        response["message"] = "Barang berhasil ditambahkan";
        response["data"]["codeBarang"] = barangBaru.codeBarang;
        response["data"]["nama"] = barangBaru.nama;
        response["data"]["hargaJual"] = barangBaru.hargaJual;
        
        sendSuccess(res, response.dump());
        
    } catch (const std::exception& e) {
        sendError(res, 400, std::string("Invalid JSON: ") + e.what());
    }
}

/**
 * PUT /api/barang/:code
 * Mengupdate data barang berdasarkan code
 */
void ApiServer::handleUpdateBarang(const httplib::Request& req, httplib::Response& res) {
    try {
        // Ambil code dari URL path
        int codeBarang = std::stoi(req.matches[1]);
        json body = json::parse(req.body);
        
        Database database;
        database.loadFromJson(getDatabasePath());
        
        bool ditemukan = false;
        for (auto& b : datasetBarang) {
            if (b.codeBarang == codeBarang) {
                ditemukan = true;
                
                // Update field yang ada di body
                if (body.contains("nama")) {
                    b.nama = body["nama"];
                }
                if (body.contains("jumlahBarang")) {
                    b.jumlahBarang = body["jumlahBarang"];
                }
                if (body.contains("hargaBarang")) {
                    b.hargaBarang = body["hargaBarang"];
                    b.hargaJual = hitungHargaJual(b.hargaBarang);
                }
                
                break;
            }
        }
        
        if (!ditemukan) {
            sendError(res, 404, "Barang tidak ditemukan");
            return;
        }
        
        database.saveToJson(getDatabasePath());
        
        json response;
        response["success"] = true;
        response["message"] = "Barang berhasil diupdate";
        sendSuccess(res, response.dump());
        
    } catch (const std::exception& e) {
        sendError(res, 400, std::string("Error: ") + e.what());
    }
}

/**
 * DELETE /api/barang/:code
 * Menghapus barang berdasarkan code
 */
void ApiServer::handleDeleteBarang(const httplib::Request& req, httplib::Response& res) {
    try {
        int codeBarang = std::stoi(req.matches[1]);
        
        Database database;
        database.loadFromJson(getDatabasePath());
        
        bool ditemukan = false;
        for (auto it = datasetBarang.begin(); it != datasetBarang.end(); ++it) {
            if (it->codeBarang == codeBarang) {
                ditemukan = true;
                datasetBarang.erase(it);
                
                // Renumber code barang
                for (int i = 0; i < (int)datasetBarang.size(); i++) {
                    datasetBarang[i].codeBarang = 1000 + i;
                }
                
                break;
            }
        }
        
        if (!ditemukan) {
            sendError(res, 404, "Barang tidak ditemukan");
            return;
        }
        
        database.saveToJson(getDatabasePath());
        
        json response;
        response["success"] = true;
        response["message"] = "Barang berhasil dihapus";
        sendSuccess(res, response.dump());
        
    } catch (const std::exception& e) {
        sendError(res, 400, std::string("Error: ") + e.what());
    }
}

// ============================================================
// API HANDLERS - TRANSAKSI
// ============================================================

/**
 * GET /api/transaksi
 * Mengembalikan semua data transaksi
 */
void ApiServer::handleGetTransaksi(const httplib::Request& req, httplib::Response& res) {
    Database database;
    database.loadTransaksi(getTransaksiPath());
    
    json response;
    response["success"] = true;
    response["data"] = json::array();
    
    for (const auto& t : datasetTransaksi) {
        json item;
        item["id"] = t.id;
        item["tanggal"] = t.tanggal;
        item["waktu"] = t.waktu;
        item["jenis"] = t.jenis;
        item["keterangan"] = t.keterangan;
        item["jumlah"] = t.jumlah;
        item["metodePembayaran"] = t.metodePembayaran;
        response["data"].push_back(item);
    }
    
    sendSuccess(res, response.dump());
}

/**
 * POST /api/transaksi
 * Melakukan checkout (menambah transaksi penjualan)
 * Body: { "items": [{"codeBarang": ..., "jumlah": ...}], "metodePembayaran": "..." }
 */
void ApiServer::handleCheckout(const httplib::Request& req, httplib::Response& res) {
    try {
        json body = json::parse(req.body);
        
        if (!body.contains("items") || !body.contains("metodePembayaran")) {
            sendError(res, 400, "Field items dan metodePembayaran wajib diisi");
            return;
        }
        
        Database database;
        database.loadFromJson(getDatabasePath());
        database.loadTransaksi(getTransaksiPath());
        
        double totalHarga = 0;
        std::string keterangan = "";
        
        // Proses setiap item
        for (const auto& item : body["items"]) {
            int code = item["codeBarang"];
            int jumlah = item["jumlah"];
            
            // Cari barang dan kurangi stok
            for (auto& b : datasetBarang) {
                if (b.codeBarang == code) {
                    if (b.jumlahBarang < jumlah) {
                        sendError(res, 400, "Stok " + b.nama + " tidak mencukupi");
                        return;
                    }
                    b.jumlahBarang -= jumlah;
                    totalHarga += b.hargaJual * jumlah;
                    
                    if (!keterangan.empty()) keterangan += ", ";
                    keterangan += b.nama + " x" + std::to_string(jumlah);
                    break;
                }
            }
        }
        
        // Simpan perubahan stok
        database.saveToJson(getDatabasePath());
        
        // Catat transaksi pemasukan
        Database::Transaksi trans;
        trans.id = generateTransactionId();
        trans.tanggal = getCurrentDate();
        trans.waktu = getCurrentTime();
        trans.jenis = "pemasukan";
        trans.keterangan = keterangan;
        trans.jumlah = totalHarga;
        trans.metodePembayaran = body["metodePembayaran"];
        
        database.tambahTransaksi(trans);
        database.saveTransaksi(getTransaksiPath());
        
        json response;
        response["success"] = true;
        response["message"] = "Checkout berhasil";
        response["data"]["totalHarga"] = totalHarga;
        response["data"]["transaksiId"] = trans.id;
        
        sendSuccess(res, response.dump());
        
    } catch (const std::exception& e) {
        sendError(res, 400, std::string("Error: ") + e.what());
    }
}

// ============================================================
// API HANDLERS - KEUANGAN
// ============================================================

/**
 * GET /api/keuangan
 * Mengembalikan ringkasan keuangan
 */
void ApiServer::handleGetKeuangan(const httplib::Request& req, httplib::Response& res) {
    Database database;
    database.loadTransaksi(getTransaksiPath());
    
    double totalPemasukan = 0;
    double totalPengeluaran = 0;
    
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            totalPemasukan += t.jumlah;
        } else if (t.jenis == "pengeluaran") {
            totalPengeluaran += t.jumlah;
        }
    }
    
    json response;
    response["success"] = true;
    response["data"]["totalPemasukan"] = totalPemasukan;
    response["data"]["totalPengeluaran"] = totalPengeluaran;
    response["data"]["labaBersih"] = totalPemasukan - totalPengeluaran;
    response["data"]["jumlahTransaksi"] = datasetTransaksi.size();
    
    sendSuccess(res, response.dump());
}

// ============================================================
// SETUP ROUTES
// ============================================================

/**
 * Setup semua route API
 * Harus dipanggil sebelum start()
 */
void ApiServer::setupRoutes() {
    // Handle preflight CORS requests
    server.Options(R"(/api/.*)", [this](const httplib::Request& req, httplib::Response& res) {
        setCorsHeaders(res);
        res.status = 204;
    });
    
    // ===== BARANG ROUTES =====
    server.Get("/api/barang", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetBarang(req, res);
    });
    
    server.Post("/api/barang", [this](const httplib::Request& req, httplib::Response& res) {
        handleAddBarang(req, res);
    });
    
    server.Put(R"(/api/barang/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleUpdateBarang(req, res);
    });
    
    server.Delete(R"(/api/barang/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleDeleteBarang(req, res);
    });
    
    // ===== TRANSAKSI ROUTES =====
    server.Get("/api/transaksi", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetTransaksi(req, res);
    });
    
    server.Post("/api/transaksi", [this](const httplib::Request& req, httplib::Response& res) {
        handleCheckout(req, res);
    });
    
    // ===== KEUANGAN ROUTES =====
    server.Get("/api/keuangan", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetKeuangan(req, res);
    });
}

// ============================================================
// START SERVER
// ============================================================

/**
 * Memulai HTTP server
 * Server akan listen pada port yang ditentukan
 * Juga serve static files dari folder 'web'
 */
void ApiServer::start(int port) {
    // Setup static file serving untuk frontend
    server.set_mount_point("/", "./web");
    
    // Setup routes
    setupRoutes();
    
    std::cout << "============================================" << std::endl;
    std::cout << "  WARUNG MADURA - Web Server" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "  Server berjalan di: http://localhost:" << port << std::endl;
    std::cout << "  Tekan Ctrl+C untuk menghentikan server" << std::endl;
    std::cout << "============================================" << std::endl;
    
    // Start listening
    server.listen("0.0.0.0", port);
}
