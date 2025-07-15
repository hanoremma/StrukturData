#include <iostream>
#include <string>
using namespace std;

struct NodeQueue {
    string namaBarang;
    NodeQueue* next;
};

NodeQueue* front = NULL;
NodeQueue* rear = NULL;

struct NodeList {
    string namaBarang;
    int harga;
    int jumlah;
    bool selesaiDibeli;
    NodeList* next;
};

NodeList* makanan = NULL;
NodeList* minuman = NULL;
NodeList* kebutuhanLain = NULL;

// Queue Functions
bool isEmptyQueue() {
    return front == NULL;
}

void enqueue(string nama) {
    NodeQueue* baru = new NodeQueue;
    baru->namaBarang = nama;
    baru->next = NULL;

    if (isEmptyQueue()) {
        front = baru;
        rear = baru;
    }
    else {
        rear->next = baru;
        rear = baru;
    }
    cout << "Barang '" << nama << "' berhasil ditambahkan ke antrian.\n";
}

string dequeue() {
    if (isEmptyQueue()) {
        return ""; 
    }
    NodeQueue* temp = front;
    string namaBarang = temp->namaBarang;
    front = front->next;
    if (front == NULL) { 
        rear = NULL;
    }
    delete temp;
    return namaBarang;
}

string peekQueue() {
    if (isEmptyQueue()) {
        return ""; 
    }
    return front->namaBarang;
}

void displayQueue() {
    if (isEmptyQueue()) {
        cout << "Antrian barang kosong.\n";
        return;
    }
    NodeQueue* cur = front;
    cout << "--- Antrian Barang yang Dibutuhkan ---\n";
    int i = 1;
    while (cur != NULL) {
        cout << i++ << ". " << cur->namaBarang << endl;
        cur = cur->next;
    }
    cout << "--------------------------------------\n";
}

//Linked List Function (terdapat insertion sort berdasarkan harga (termahal otomatis diatas))
void tambahListSorted(NodeList*& head, string nama, int harga, int jumlah, bool selesaiDibeli = false) {
    NodeList* baru = new NodeList;
    baru->namaBarang = nama;
    baru->harga = harga;
    baru->jumlah = jumlah;
    baru->next = NULL;
    baru->selesaiDibeli = selesaiDibeli;
    int subtotal = harga * jumlah;

    if (head == NULL || (head->harga * head->jumlah) < subtotal) {
        baru->next = head;
        head = baru;
    }
    else {
        NodeList* cur = head;
        while (cur->next != NULL && (cur->next->harga * cur->next->jumlah) >= subtotal) {
            cur = cur->next;
        }
        baru->next = cur->next;
        cur->next = baru;
    }
    cout << "Barang '" << nama << "' ditambahkan secara terurut ke daftar.\n";
}

int totalHargaList(NodeList* head) {
    int total = 0;
    NodeList* cur = head;
    while (cur != NULL) {
        total += cur->harga * cur->jumlah;
        cur = cur->next;
    }
    return total;
}

void tampilList(NodeList* head) {
    NodeList* cur = head;
    if (cur == NULL) {
        cout << "Kosong.\n";
        return;
    }
    while (cur != NULL) {
        cout << "- " << cur->namaBarang
            << " | Harga: " << cur->harga
            << " | Jumlah: " << cur->jumlah
            << " | Status: " << (cur->selesaiDibeli ? "Selesai" : "Belum")
            << " | Subtotal: " << cur->harga * cur->jumlah << endl;
        cur = cur->next;
    }
}

void tampilkanSemuaDaftarBelanja() {
    cout << "\n--- DAFTAR BELANJA ANDA ---\n";
    cout << "Daftar Makanan:\n";
    tampilList(makanan);
    cout << "Daftar Minuman:\n";
    tampilList(minuman);
    cout << "Daftar Kebutuhan Lain:\n";
    tampilList(kebutuhanLain);
    cout << "---------------------------\n";
    cout << "Estimasi Total Harga Seluruh Belanja: Rp"
        << totalHargaList(makanan) + totalHargaList(minuman) + totalHargaList(kebutuhanLain)
        << endl << endl;
}

void hapusList(NodeList*& head, string barang) {
    if (head == NULL) {
        cout << "Daftar kosong.\n";
        return;
    }
    NodeList* cur = head;
    NodeList* prev = NULL;

    while (cur != NULL && cur->namaBarang != barang) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        cout << "Barang tidak ditemukan.\n";
        return;
    }

    if (prev == NULL) {
        head = head->next;
    }
    else {
        prev->next = cur->next;
    }

    delete cur;
    cout << "Barang '" << barang << "' dihapus dari daftar.\n";
}

void updateNode(NodeList*& head, string namaBarang, int hargaBaru, int jumlahBaru) {
    if (head == NULL) {
        cout << "Daftar kosong.\n";
        return;
    }

    NodeList* cur = head;
    NodeList* prev = NULL;

    while (cur != NULL && cur->namaBarang != namaBarang) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        cout << "Barang tidak ditemukan.\n";
        return;
    }

    string namaBaru = cur->namaBarang; 
    if (prev == NULL) { 
        head = cur->next;
    }
    else {
        prev->next = cur->next;
    }
    delete cur;

    tambahListSorted(head, namaBaru, hargaBaru, jumlahBaru);
    cout << "Data berhasil diupdate & disusun ulang!\n";
}

NodeList * cariBarang(NodeList * head, string namaCari) {
    NodeList* cur = head;
    while (cur != NULL) {
        if (cur->namaBarang == namaCari) {
            return cur; 
        }
        cur = cur->next;
    }
    return NULL; 
}

// Status selesai dibeli atau belum
void toggleStatus(NodeList* head, string nama) {
    NodeList* item = cariBarang(head, nama);
    if (!item) cout << "Barang tidak ditemukan.\n";
    else {
        item->selesaiDibeli = !item->selesaiDibeli;
        cout << "Status '" << nama << "' diubah menjadi: " << (item->selesaiDibeli ? "Selesai" : "Belum") << endl;
    }
}

//khusus case 6
NodeList*& getListByType(int tipe) {
    if (tipe == 1) return makanan;
    if (tipe == 2) return minuman;
    return kebutuhanLain; 
}

int main() {
    int pilihan, harga, jumlah, tipe;
    string barang;

    do {
        cout << "\n========== MENU DAFTAR BELANJA ==========\n\n";
        cout << "1. Tambah Antrian Barang yang Dibutuhkan\n";
        cout << "2. Lihat/hapus Antrian Barang yang Dibutuhkan\n";
        cout << "3. Pindahkan Barang dari Antrian ke Daftar Belanja\n";
        cout << "4. Tambah Barang di Daftar Belanja\n";
        cout << "5. Lihat Barang di Daftar Belanja + Total Harga\n";
        cout << "6. Update/Ubah status/Hapus Barang di Daftar Belanja\n";
        cout << "7. Cari Barang di Daftar Belanja\n";
        cout << "8. Keluar.\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
        case 1:
            system("cls");
            cout << "Masukkan nama barang yang akan diantri: ";
            getline(cin, barang);
            enqueue(barang);
            break;
        case 2:
            system("cls");
            if (isEmptyQueue()) {
                cout << "Antrian kosong.\n";
            }
            else {
                displayQueue();  
                cout << "Hapus barang pertama dari antrian? (y/n): ";
                char h;
                cin >> h;
                cin.ignore();
                if (h == 'y' || h == 'Y') {
                    cout << "Barang '" << dequeue() << "' berhasil dihapus dari antrian.\n";
                }
            }
            break;
        case 3: {
            system("cls");
            if (isEmptyQueue()) {
                cout << "Antrian kosong, tidak ada barang yang bisa dipindahkan.\n";
                break;
            }
            string barangDariAntrian = peekQueue();
            if (barangDariAntrian.empty()) {
                cout << "Gagal memindahkan barang dari antrian.\n";
                break;
            }

            cout << "Barang '" << barangDariAntrian << "' diambil dari antrian.\n";
            do {
                cout << "Pilih kategori untuk '" << barangDariAntrian << "':\n";
                cout << "1. Makanan\n2. Minuman\n3. Kebutuhan Lain\n0. Batal\nPilihan: ";
                cin >> tipe;

                if (tipe == 0) {
                    cout << "Pemindahan barang dibatalkan. Barang tetap dalam antrian.\n";
                    break;
                }

                if (tipe < 1 || tipe > 3) {
                    cout << "Tipe salah, coba lagi.\n";
                }
            } while (tipe < 1 || tipe > 3);

            if (tipe == 0) break;

            cout << "Masukkan harga barang '" << barangDariAntrian << "': ";
            cin >> harga;
            cout << "Masukkan jumlah barang '" << barangDariAntrian << "': ";
            cin >> jumlah;
            cin.ignore(); 

            dequeue();

            if (tipe == 1) tambahListSorted(makanan, barangDariAntrian, harga, jumlah);
            else if (tipe == 2) tambahListSorted(minuman, barangDariAntrian, harga, jumlah);
            else if (tipe == 3) tambahListSorted(kebutuhanLain, barangDariAntrian, harga, jumlah);
            break;
        }
            
        case 4: 
            system("cls");
            tampilkanSemuaDaftarBelanja();

            do {
                cout << "1. Makanan\n2. Minuman\n3. Kebutuhan Lain\nPilihan: ";
                cin >> tipe;

                if (tipe < 1 || tipe > 3) {
                    cout << "Tipe salah, coba lagi.\n";
                }
            } while (tipe < 1 || tipe > 3);

            cin.ignore();
            cout << "Masukkan nama barang: ";
            getline(cin, barang);
            cout << "Masukkan harga barang: ";
            cin >> harga;
            cout << "Masukkan jumlah barang: ";
            cin >> jumlah;

            if (tipe == 1) tambahListSorted(makanan, barang, harga, jumlah);
            else if (tipe == 2) tambahListSorted(minuman, barang, harga, jumlah);
            else if (tipe == 3) tambahListSorted(kebutuhanLain, barang, harga, jumlah);
            else cout << "Tipe salah.\n";
            break;
        case 5:
            system("cls");
            tampilkanSemuaDaftarBelanja();
            break;
        case 6:
        {
            system("cls");
            tampilkanSemuaDaftarBelanja();

            do {
                cout << "1. Makanan\n2. Minuman\n3. Kebutuhan Lain\nPilihan: ";
                cin >> tipe;
                if (tipe < 1 || tipe > 3)
                    cout << "Tipe salah, coba lagi.\n";
            } while (tipe < 1 || tipe > 3);

            cin.ignore();
            NodeList*& listTarget = getListByType(tipe);

            if (listTarget == NULL) {
                cout << "Daftar kosong.\n";
                break;
            }

            cout << "Nama barang: ";
            getline(cin, barang);

            NodeList* ditemukan = cariBarang(listTarget, barang);

            if (ditemukan == NULL) {
                cout << "Barang '" << barang << "' tidak ditemukan di kategori yang dipilih.\n";
                break;
            }

            cout << "1. Ubah Status\n2. Hapus Barang\n3. Update Barang\n0. Batal\nPilihan: ";
            int opsi;
            cin >> opsi;
            cin.ignore();

            switch (opsi) {
            case 1:
                toggleStatus(listTarget, barang);
                break;
            case 2:
                hapusList(listTarget, barang);
                break;
            case 3:
                cout << "Masukkan harga baru: ";
                cin >> harga;
                cout << "Masukkan jumlah baru: ";
                cin >> jumlah;
                cin.ignore();
                updateNode(listTarget, barang, harga, jumlah);
                break;
            default:
                cout << "Tidak melakukan perubahan apapun\n";
            }

            break;
        }
        case 7:
        {
            system("cls");
            tampilkanSemuaDaftarBelanja();
            cout << "Masukkan nama barang yang ingin dicari: ";
            getline(cin, barang);

            NodeList* foundItem = NULL;
            string kategoriDitemukan = "Tidak ditemukan";

            foundItem = cariBarang(makanan, barang);
            if (foundItem != NULL) {
                kategoriDitemukan = "Makanan";
            }
            else {
                foundItem = cariBarang(minuman, barang);
                if (foundItem != NULL) {
                    kategoriDitemukan = "Minuman";
                }
                else {
                    foundItem = cariBarang(kebutuhanLain, barang);
                    if (foundItem != NULL) {
                        kategoriDitemukan = "Kebutuhan Lain";
                    }
                }
            }

            if (foundItem != NULL) {
                cout << "\nBarang ditemukan di kategori " << kategoriDitemukan << "!\n";
                cout << "Detail: " << foundItem->namaBarang
                    << " | Harga: Rp" << foundItem->harga
                    << " | Jumlah: " << foundItem->jumlah
                    << " | Status: " << (foundItem->selesaiDibeli ? "Selesai" : "Belum")
                    << " | Subtotal: Rp" << foundItem->harga * foundItem->jumlah << endl;
            }
            else {
                cout << "Barang '" << barang << "' tidak ditemukan di daftar belanja manapun.\n";
            }
            break;
        }
        case 8:
            cout << "Keluar dari program.\n";
            break;
        default:
            cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 8);

    return 0;
}
