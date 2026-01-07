#ifndef API_SERVER_H
#define API_SERVER_H

/**
 * ============================================================
 * API SERVER - Sistem Pencatatan Warung Madura (Web Version)
 * ============================================================
 * File        : ApiServer.h
 * Deskripsi   : Header untuk HTTP server yang menyediakan REST API
 *               Menghubungkan frontend web dengan logic C++ backend
 * ============================================================
 */

// === HTTP SERVER LIBRARY ===
#include "httplib.h"

// === STANDARD LIBRARY ===
#include <string>

// Forward declaration
class Database;

/**
 * Class ApiServer
 * Mengelola HTTP server dan routing REST API
 * Semua request dari frontend akan dihandle oleh class ini
 */
class ApiServer {
public:
    /**
     * Memulai server pada port tertentu
     * @param port - port number (default: 8080)
     */
    void start(int port = 8080);
    
    /**
     * Setup semua route API
     * Dipanggil sebelum start()
     */
    void setupRoutes();
    
private:
    httplib::Server server;
    
    // ============================================================
    // API HANDLERS - BARANG
    // ============================================================
    
    /**
     * GET /api/barang - Mendapatkan semua data barang
     */
    void handleGetBarang(const httplib::Request& req, httplib::Response& res);
    
    /**
     * POST /api/barang - Menambah barang baru
     * Body: { "nama": "...", "jumlahBarang": ..., "hargaBarang": ... }
     */
    void handleAddBarang(const httplib::Request& req, httplib::Response& res);
    
    /**
     * PUT /api/barang/:code - Mengupdate barang berdasarkan code
     * Body: { "nama": "...", "jumlahBarang": ..., "hargaBarang": ... }
     */
    void handleUpdateBarang(const httplib::Request& req, httplib::Response& res);
    
    /**
     * DELETE /api/barang/:code - Menghapus barang berdasarkan code
     */
    void handleDeleteBarang(const httplib::Request& req, httplib::Response& res);
    
    // ============================================================
    // API HANDLERS - TRANSAKSI
    // ============================================================
    
    /**
     * GET /api/transaksi - Mendapatkan semua transaksi
     */
    void handleGetTransaksi(const httplib::Request& req, httplib::Response& res);
    
    /**
     * POST /api/transaksi - Menambah transaksi baru (checkout)
     * Body: { "items": [...], "metodePembayaran": "..." }
     */
    void handleCheckout(const httplib::Request& req, httplib::Response& res);
    
    // ============================================================
    // API HANDLERS - KEUANGAN
    // ============================================================
    
    /**
     * GET /api/keuangan - Mendapatkan ringkasan keuangan
     */
    void handleGetKeuangan(const httplib::Request& req, httplib::Response& res);
    
    // ============================================================
    // HELPER FUNCTIONS
    // ============================================================
    
    /**
     * Set CORS headers untuk development
     */
    void setCorsHeaders(httplib::Response& res);
    
    /**
     * Kirim response JSON error
     */
    void sendError(httplib::Response& res, int status, const std::string& message);
    
    /**
     * Kirim response JSON sukses
     */
    void sendSuccess(httplib::Response& res, const std::string& jsonData);
};

#endif // API_SERVER_H
