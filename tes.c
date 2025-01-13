#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h> // Untuk kbhit() dan getch()
#include <mmsystem.h>
// #pragma comment(lib, "winmm.lib")

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

// Fungsi untuk menampilkan header
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

// Dapatkan pergeseran langkah berdasarkan input
void getMove(char input, int *dx, int *dy)
{
    Beep(500, 200); // Suara beep saat bergerak
    switch (input)
    {
    case 'w':
        (*dx)--; // Gerak ke atas
        break;
    case 's':
        (*dx)++; // Gerak ke bawah
        break;
    case 'a':
        (*dy)--; // Gerak ke kiri
        break;
    case 'd':
        (*dy)++; // Gerak ke kanan
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

// // Fungsi untuk memutar musik
// void playMusic(const char *filename)
// {
//     PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
// }

// Fungsi untuk menangkap input dan proses langkah
void processInput()
{
    if (kbhit()) // Periksa apakah ada tombol yang ditekan
    {
        char input = getch(); // Baca tombol tanpa perlu Enter

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
        }
        else
        {
            printf(YELLOW "Langkah tidak valid. Coba lagi!\n" RESET);
        }
    }
}

// Main program
int main()
{
    srand(time(NULL));

    // playMusic("D:\\Pemrograman\\Minigame-C\\src\\music.wav");

    displayHeader();

    printf(YELLOW "Permainan baru dimulai.\n" RESET);
    initializeBoard();

    while (1)
    {
        if (!hide_board)
            printBoardWithFrame();

        printf(GREEN "========================================\n" RESET);
        printf(MAGENTA "Level: %d | Total bantuan: %d\n" RESET, level, help_count);
        printf(GREEN "Posisi Perawat (*): (%d, %d)\n" RESET, nurse_x, nurse_y);
        printf(GREEN "Posisi Pasien (#): (%d, %d)\n" RESET, patient_x, patient_y);
        printf(GREEN "========================================\n" RESET);

        // Proses input tanpa memblokir program
        processInput();
    }

    return 0;
}
