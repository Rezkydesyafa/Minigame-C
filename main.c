#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// initialize
char board[10][10];
int ROWS = 6, COLS = 6, nurse_x = 0, nurse_y = 0, patient_x = 5,
    patient_y = 5, help_count = 0, level = 1, challenges_remaining = 3;
bool hide_board = false;

// Warna teks
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET "\033[0m"

// Fungsi untuk menampilkan animasi header
void displayHeader()
{
    printf(GREEN "========================================\n" RESET);
    printf(GREEN "            MEDICAL MINI GAME           \n" RESET);
    printf(GREEN "========================================\n" RESET);
    printf(GREEN "Selamat datang di Medical Mini Game!\n" RESET);
    printf(YELLOW "Bantu perawat (*) mencapai pasien (#) sambil menghindari rintangan (X).\n" RESET);
    printf(GREEN "========================================\n" RESET);
}

// Fungsi untuk menampilkan bingkai papan
void printBoardWithFrame()
{
    if (hide_board)
    {
        printf(YELLOW "Papan tidak terlihat karena tantangan!\n" RESET);
        return;
    }

    printf(CYAN "+");
    for (int j = 0; j < COLS; j++)
        printf(CYAN "--" RESET);
    printf("+\n" RESET);

    for (int i = 0; i < ROWS; i++)
    {
        printf(CYAN "| " RESET);
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == '*')
                printf(GREEN "%c " RESET, board[i][j]);
            else if (board[i][j] == '#')
                printf(RED "%c " RESET, board[i][j]);
            else if (board[i][j] == 'X')
                printf(YELLOW "%c " RESET, board[i][j]);
            else
                printf("%c ", board[i][j]);
        }
        printf(CYAN "|\n" RESET);
    }

    printf(CYAN "+");
    for (int j = 0; j < COLS; j++)
        printf(CYAN "--" RESET);
    printf(CYAN "+\n\n" RESET);
}

// Inisialisasi papan permainan
void initializeBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = '-';
        }
    }
    board[nurse_x][nurse_y] = '*';     // perawat
    board[patient_x][patient_y] = '#'; // pasien
}

// Periksa apakah langkah valid
bool isValidMove(int x, int y)
{
    return x >= 0 && x < ROWS && y >= 0 && y < COLS;
}

// Perbarui posisi perawat di papan
void updatePosition(int old_x, int old_y, int new_x, int new_y)
{
    board[old_x][old_y] = '-'; // Kosongkan posisi lama
    board[new_x][new_y] = '*'; // Tempatkan perawat di posisi baru
}

// Dapatkan pergeseran langkah berdasarkan input dengan increment
void getMove(char input, int *dx, int *dy)
{
    switch (input)
    {
    case 'w':
        (*dx)--;
        break;
    case 's':
        (*dx)++;
        break;
    case 'a':
        (*dy)--;
        break;
    case 'd':
        (*dy)++;
        break;
    default:
        printf(YELLOW "Input tidak valid. Gunakan w/a/s/d untuk bergerak.\n" RESET);
        break;
    }
}

// Tempatkan pasien baru di lokasi acak
void spawnNewPatient()
{
    int new_x, new_y;
    do
    {
        new_x = rand() % ROWS;
        new_y = rand() % COLS;
    } while (board[new_x][new_y] != '-'); // Pastikan posisi kosong

    patient_x = new_x;
    patient_y = new_y;
    board[patient_x][patient_y] = '#';
    printf(YELLOW "Pasien baru muncul di posisi: (%d, %d)\n" RESET, patient_x, patient_y);
}

// Tambahkan tantangan acak
void generateChallenge()
{
    int challenge = rand() % 3;
    printf(RED "\nTANTANGAN! " RESET);

    switch (challenge)
    {
    case 0:
        printf("Papan dipenuhi rintangan! Perawat harus hati-hati.\n");
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (rand() % 10 < 2 && board[i][j] == '-')
                    board[i][j] = 'X'; // Tambahkan rintangan
            }
        }
        break;
    case 1:
        printf("Pasien memerlukan bantuan ekstra cepat! Selesaikan dalam 5 langkah!\n");
        challenges_remaining = 5;
        break;
    case 2:
        printf("Perawat kehilangan peta! Papan tidak akan ditampilkan sementara.\n");
        hide_board = true;
        challenges_remaining = 5; // Perawat harus bergerak 5 langkah untuk mengembalikan peta
        break;
    }
}

// Reset tantangan
void resetChallenge()
{
    hide_board = false;
    challenges_remaining = 3;
    printf(GREEN "Tantangan selesai! Kembali ke kondisi normal.\n" RESET);
}

// Tingkatkan level permainan
void increaseLevel()
{
    level++;
    printf(GREEN "Level meningkat! Level saat ini: %d\n" RESET, level);

    if (ROWS + 1 <= 10 && COLS + 1 <= 10)
    {
        ROWS++;
        COLS++;
        printf(YELLOW "Ukuran papan bertambah! Papan sekarang berukuran %d x %d.\n" RESET, ROWS, COLS);
    }

    initializeBoard();
    board[nurse_x][nurse_y] = '*';
    board[patient_x][patient_y] = '#';
    generateChallenge();
}
// Simpan status permainan ke file
void saveGame(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf(RED "Gagal menyimpan permainan.\n" RESET);
        return;
    }

    fprintf(file, "%d %d\n", ROWS, COLS);           // Simpan ukuran papan
    fprintf(file, "%d %d\n", nurse_x, nurse_y);     // Simpan posisi perawat
    fprintf(file, "%d %d\n", patient_x, patient_y); // Simpan posisi pasien
    fprintf(file, "%d\n", help_count);              // Simpan jumlah bantuan
    fprintf(file, "%d\n", level);                   // Simpan level

    for (int i = 0; i < ROWS; i++) // Simpan papan permainan
    {
        for (int j = 0; j < COLS; j++)
        {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf(GREEN "Permainan berhasil disimpan ke file %s.\n" RESET, filename);
}

// Muat status permainan dari file
void loadGame(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf(RED "Gagal memuat permainan.\n" RESET);
        return;
    }

    fscanf(file, "%d %d\n", &ROWS, &COLS);           // Muat ukuran papan
    fscanf(file, "%d %d\n", &nurse_x, &nurse_y);     // Muat posisi perawat
    fscanf(file, "%d %d\n", &patient_x, &patient_y); // Muat posisi pasien
    fscanf(file, "%d\n", &help_count);               // Muat jumlah bantuan
    fscanf(file, "%d\n", &level);                    // Muat level

    for (int i = 0; i < ROWS; i++) // Muat papan permainan
    {
        for (int j = 0; j < COLS; j++)
        {
            fscanf(file, " %c", &board[i][j]);
        }
    }

    fclose(file);
    printf(GREEN "Permainan berhasil dimuat dari file %s.\n" RESET, filename);
}

// Main program
int main()
{
    srand(time(NULL));

    displayHeader();
    printf(MAGENTA "Apakah kamu ingin memuat Game? (y/n): " RESET);
    char load_input;
    scanf(" %c", &load_input);
    printf(GREEN "========================================\n" RESET);

    if (load_input == 'y')
    {
        loadGame("game_save.csv");
        printf(YELLOW "Selamat bermain kembali!\n" RESET);
        // printf(YELLOW "Fitur memuat permainan belum diimplementasikan.\n" RESET);
    }
    else
    {
        initializeBoard();
        printf(YELLOW "Permainan baru dimulai.\n" RESET);
    }

    char input;

    while (1)
    {
        if (!hide_board)
            printBoardWithFrame();

        printf(GREEN "========================================\n" RESET);
        printf(MAGENTA "Level: %d | Total bantuan: %d\n" RESET, level, help_count);
        printf(GREEN "Posisi Perawat (*): (%d, %d)\n" RESET, nurse_x, nurse_y);
        printf(GREEN "Posisi Pasien (#): (%d, %d)\n" RESET, patient_x, patient_y);
        printf(GREEN "Tantangan tersisa: %d\n" RESET, challenges_remaining);
        printf(GREEN "========================================\n" RESET);

        printf(GREEN "Perawat's move! (gunakan w/a/s/d untuk bergerak) dan x untuk save dan keluar : " RESET);
        scanf(" %c", &input);
        if (input == 'x')
        {
            printf(GREEN "----------------------------------------\n" RESET);
            saveGame("game_save.csv");
            printf("Permainan disimpan. Keluar dari permainan.\n");
            printf(GREEN "----------------------------------------\n" RESET);
            break;
        }

        int dx = 0, dy = 0;
        getMove(input, &dx, &dy);

        int new_x = nurse_x + dx;
        int new_y = nurse_y + dy;

        if (isValidMove(new_x, new_y))
        {
            if (board[new_x][new_y] == '#')
            {
                help_count++;
                printf(GREEN "Perawat berhasil membantu pasien!\n" RESET);
                spawnNewPatient();
            }
            else if (board[new_x][new_y] == 'X')
            {
                printf(YELLOW "Perawat terjebak dalam rintangan!\n" RESET);
            }
            else
            {
                updatePosition(nurse_x, nurse_y, new_x, new_y);
                nurse_x = new_x;
                nurse_y = new_y;
            }

            if (hide_board)
                challenges_remaining--;

            if (challenges_remaining == 0)
            {
                resetChallenge();
            }

            if (help_count >= level * 2)
            {
                increaseLevel();
            }

            if (level > 5)
            {
                printf(GREEN "Selamat, kamu telah menyelesaikan permainan!\n" RESET);
                break;
            }
        }
        else
        {
            printf(YELLOW "Langkah tidak valid. Coba lagi!\n" RESET);
        }
    }

    return 0;
}
