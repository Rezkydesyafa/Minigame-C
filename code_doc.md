Berikut adalah penjelasan dari setiap fungsi utama dalam program ** Nulungan Game**:

# 1. Fungsi Display dan Inisialisasi

---

## 1.1 `displayHeader()`

- Fungsi ini menampilkan header permainan di layar menggunakan warna yang sudah diatur dengan makro warna (seperti `GREEN`, `YELLOW`).
- Penjelasan isi:
  - **Header utama permainan**: Memberikan informasi nama permainan.
  - **Instruksi permainan**: Memberikan penjelasan singkat tentang tujuan permainan dan elemen-elemen yang akan ditemukan di papan, seperti perawat (`*`), pasien (`#`), dan rintangan (`X`).

## 1.2 `printBoardWithFrame()`

- Fungsi untuk menampilkan papan permainan ke layar dengan bingkai.
- Penjelasan isi:
  - **Cek status `hide_board`**: Jika tantangan menyembunyikan papan aktif (`hide_board = true`), hanya pesan pemberitahuan yang ditampilkan.
  - **Bingkai horizontal**: Menambahkan garis "+" dan "--" untuk setiap kolom.
  - **Isi papan**:
    - Menampilkan simbol dengan warna:
      - Perawat (`*`): **Hijau**.
      - Pasien (`#`): **Merah**.
      - Rintangan (`X`): **Kuning**.
      - Ruang kosong (`-`): **Default**.
    - Setiap baris dibingkai dengan tanda "|".

## 1.3 `initializeBoard()`

- Fungsi untuk menginisialisasi papan permainan dengan kondisi awal.
- Penjelasan isi:
  - **Mengisi papan dengan simbol `-`**: Menandakan posisi kosong.
  - **Menempatkan perawat (`*`) di posisi awal**: `(0, 0)`.
  - **Menempatkan pasien (`#`) di posisi awal**: `(5, 5)`.

## 2. Fungsi Logika Permainan

---

#### 2.1 `isValidMove(x, y)`

- Fungsi untuk memeriksa apakah posisi `(x, y)` berada dalam batas papan permainan.
- **Parameter**:
  - `x`: Koordinat baris tujuan.
  - `y`: Koordinat kolom tujuan.
- **Logika**:
  - Memastikan nilai `x` dan `y` berada dalam rentang valid: `0 <= x < ROWS` dan `0 <= y < COLS`.

### 2.2 `updatePosition(old_x, old_y, new_x, new_y)`

- Fungsi untuk memperbarui posisi perawat di papan.
- **Parameter**:
  - `old_x`, `old_y`: Posisi lama perawat.
  - `new_x`, `new_y`: Posisi baru perawat.
- **Logika**:
  - Mengosongkan posisi lama (`board[old_x][old_y] = '-'`).
  - Menandai posisi baru dengan simbol perawat (`board[new_x][new_y] = '*'`).

## 2.3 `getMove(input, dx, dy)`

- Fungsi untuk menginterpretasikan input pemain menjadi pergeseran posisi.
  1. Parameter:
  - `input`: Input arah dari pemain (`w`, `a`, `s`, `d`).
  - `dx`, `dy`: Variabel pointer untuk menyimpan pergeseran koordinat.
  2. Logika:
  - Berdasarkan input:
    - `w`: Mengurangi baris (`dx--` → bergerak ke atas).
    - `s`: Menambah baris (`dx++` → bergerak ke bawah).
    - `a`: Mengurangi kolom (`dy--` → bergerak ke kiri).
    - `d`: Menambah kolom (`dy++` → bergerak ke kanan).
  - Input yang tidak valid akan menampilkan pesan peringatan.

## 2.4 `spawnNewPatient()`

- Fungsi untuk menempatkan pasien baru secara acak di papan.
- **Logika**:
  - Menggunakan angka acak untuk memilih koordinat baru (`new_x`, `new_y`).
  - Memastikan posisi baru kosong (`board[new_x][new_y] == '-'`).
  - Menempatkan pasien (`board[new_x][new_y] = '#'`) dan memperbarui posisi pasien global (`patient_x`, `patient_y`).

## 2.5 `generateChallenge()`

- Fungsi untuk membuat tantangan acak dalam permainan.
  1. Jenis Tantangan:
  - **Tantangan Rintangan**: Menambahkan simbol `X` (rintangan) di posisi kosong pada papan.
  - **Tantangan Cepat**: Memaksa pemain menyelesaikan dalam jumlah langkah terbatas (misalnya 5 langkah).
  - **Tantangan Papan Tersembunyi**: Menyembunyikan tampilan papan selama beberapa langkah.
  2. Pemilihan Tantangan:
  - Menggunakan `rand() % 3` untuk memilih salah satu dari tiga jenis tantangan secara acak.

## 2.6 `resetChallenge()`

- Fungsi untuk mengakhiri tantangan dan mengembalikan kondisi permainan ke normal.
- **Logika**:
  - Mengatur `hide_board = false` (jika papan disembunyikan).
  - Mengatur ulang `challenges_remaining` ke nilai awal (3).

## 2.7 `increaseLevel()`

- Fungsi untuk meningkatkan level permainan.
- **Logika**:
  - Menambah variabel `level`.
  - Jika ukuran papan belum mencapai maksimum (10x10), meningkatkan ukuran papan (`ROWS++`, `COLS++`).
  - Memulai ulang papan dengan ukuran baru menggunakan `initializeBoard()`.
  - Membuat tantangan baru untuk level berikutnya.

## 3. Fungsi File I/O

---

## 3.1 `saveGame(filename)`

- Fungsi untuk menyimpan status permainan ke dalam file.
- **Data yang Disimpan**:
  - Ukuran papan (`ROWS`, `COLS`).
  - Posisi perawat dan pasien.
  - Jumlah pasien yang berhasil ditolong (`help_count`).
  - Level permainan.
  - Kondisi papan.
- **Proses**:
  - Membuka file dalam mode tulis (`"w"`).
  - Menyimpan semua data dalam format yang bisa dibaca ulang.

## 3.2 `loadGame(filename)`

- Fungsi untuk memuat status permainan dari file.
- **Proses**:
  - Membuka file dalam mode baca (`"r"`).
  - Membaca data dari file (ukuran papan, posisi, dll.).
  - Mengembalikan permainan ke kondisi yang tersimpan.

## 4. Fungsi Musik

---

## 4.1 `playMusic(filename)`

- Fungsi untuk memutar musik latar.
- **Logika**:
- Menggunakan fungsi `PlaySound` untuk memainkan file musik (`.wav`) secara asinkron.

## 5. Main Program

## Alur Program Utama:

1. **Inisialisasi Awal**:

   - Memutar musik latar.
   - Menampilkan header permainan.
   - Menawarkan opsi untuk memuat permainan dari file.
   - Jika tidak, memulai permainan baru.

2. **Gameplay Loop**:

   - Menampilkan papan dan informasi permainan (level, posisi, tantangan).
   - Meminta input arah dari pemain.
   - Mengevaluasi langkah (periksa validitas, tangani rintangan, pasien, atau tantangan).
   - Meningkatkan level jika syarat terpenuhi.
   - Mengakhiri permainan jika level > 5.

3. **Simpan dan Keluar**:
   - Menyimpan status permainan jika pemain memilih keluar.
