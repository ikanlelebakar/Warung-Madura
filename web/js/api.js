/**
 * ============================================================
 * API HELPER - Warung Madura Web Frontend
 * ============================================================
 * File        : api.js
 * Deskripsi   : Helper functions untuk komunikasi dengan C++ backend
 *               Semua request ke server melalui file ini
 * ============================================================
 */

// Base URL untuk API
const API_BASE = 'http://localhost:8080/api';

// ============================================================
// BARANG API
// ============================================================

/**
 * Mendapatkan semua data barang dari server
 * @returns {Promise<Array>} - Array of barang objects
 */
async function getAllBarang() {
    try {
        const response = await fetch(`${API_BASE}/barang`);
        const data = await response.json();
        if (data.success) {
            return data.data;
        }
        throw new Error(data.error);
    } catch (error) {
        console.error('Error fetching barang:', error);
        throw error;
    }
}

/**
 * Menambah barang baru
 * @param {Object} barang - { nama, jumlahBarang, hargaBarang }
 * @returns {Promise<Object>} - Response dari server
 */
async function addBarang(barang) {
    try {
        const response = await fetch(`${API_BASE}/barang`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(barang)
        });
        return await response.json();
    } catch (error) {
        console.error('Error adding barang:', error);
        throw error;
    }
}

/**
 * Mengupdate data barang
 * @param {number} codeBarang - Code barang yang akan diupdate
 * @param {Object} data - Data yang akan diupdate
 * @returns {Promise<Object>} - Response dari server
 */
async function updateBarang(codeBarang, data) {
    try {
        const response = await fetch(`${API_BASE}/barang/${codeBarang}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        return await response.json();
    } catch (error) {
        console.error('Error updating barang:', error);
        throw error;
    }
}

/**
 * Menghapus barang
 * @param {number} codeBarang - Code barang yang akan dihapus
 * @returns {Promise<Object>} - Response dari server
 */
async function deleteBarang(codeBarang) {
    try {
        const response = await fetch(`${API_BASE}/barang/${codeBarang}`, {
            method: 'DELETE'
        });
        return await response.json();
    } catch (error) {
        console.error('Error deleting barang:', error);
        throw error;
    }
}

// ============================================================
// TRANSAKSI API
// ============================================================

/**
 * Mendapatkan semua transaksi
 * @returns {Promise<Array>} - Array of transaksi objects
 */
async function getAllTransaksi() {
    try {
        const response = await fetch(`${API_BASE}/transaksi`);
        const data = await response.json();
        if (data.success) {
            return data.data;
        }
        throw new Error(data.error);
    } catch (error) {
        console.error('Error fetching transaksi:', error);
        throw error;
    }
}

/**
 * Melakukan checkout (transaksi penjualan)
 * @param {Array} items - Array of { codeBarang, jumlah }
 * @param {string} metodePembayaran - 'CASH' atau 'CASHLESS'
 * @returns {Promise<Object>} - Response dari server
 */
async function checkout(items, metodePembayaran) {
    try {
        const response = await fetch(`${API_BASE}/transaksi`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ items, metodePembayaran })
        });
        return await response.json();
    } catch (error) {
        console.error('Error checkout:', error);
        throw error;
    }
}

// ============================================================
// KEUANGAN API
// ============================================================

/**
 * Mendapatkan ringkasan keuangan
 * @returns {Promise<Object>} - { totalPemasukan, totalPengeluaran, labaBersih }
 */
async function getKeuangan() {
    try {
        const response = await fetch(`${API_BASE}/keuangan`);
        const data = await response.json();
        if (data.success) {
            return data.data;
        }
        throw new Error(data.error);
    } catch (error) {
        console.error('Error fetching keuangan:', error);
        throw error;
    }
}

// ============================================================
// HELPER FUNCTIONS
// ============================================================

/**
 * Format angka ke format Rupiah
 * @param {number} angka - Angka yang akan diformat
 * @returns {string} - Format "Rp 10.000"
 */
function formatRupiah(angka) {
    return 'Rp ' + angka.toLocaleString('id-ID');
}

/**
 * Menampilkan notifikasi toast
 * @param {string} message - Pesan yang akan ditampilkan
 * @param {string} type - 'success', 'error', atau 'info'
 */
function showToast(message, type = 'info') {
    const toast = document.createElement('div');
    toast.className = `fixed bottom-4 right-4 px-6 py-3 rounded-lg text-white shadow-lg z-50 
        ${type === 'success' ? 'bg-green-500' : type === 'error' ? 'bg-red-500' : 'bg-blue-500'}`;
    toast.textContent = message;
    document.body.appendChild(toast);

    setTimeout(() => {
        toast.remove();
    }, 3000);
}
