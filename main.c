#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// initialize
char board[10][10];
int ROWS = 6, COLS = 6, nurse_x = 0, nurse_y = 0, patient_x = 5,
    patient_y = 5, help_count = 0, level = 1;

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

// Cetak papan permainan
void printBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Periksa apakah langkah valid
bool isValidMove(int x, int y)
{
    return x >= 0 && x < ROWS && y >= 0 && y < COLS;
}

// Perbarui posisi pemain di papan
void updatePosition(char player, int old_x, int old_y, int new_x, int new_y)
{
    board[old_x][old_y] = '-';    // Kosongkan posisi lama
    board[new_x][new_y] = player; // Tempatkan pemain di posisi baru
}

// Dapatkan pergeseran langkah berdasarkan input menggunakan switch-case
void getMove(char input, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    switch (input)
    {
    case 'w':
        *dx = -1;
        break;
    case 's':
        *dx = 1;
        break;
    case 'a':
        *dy = -1;
        break;
    case 'd':
        *dy = 1;
        break;
    default:
        printf("Input tidak valid. Gunakan w/a/s/d untuk bergerak.\n");
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
    printf("Pasien baru muncul di posisi: (%d, %d)\n", patient_x, patient_y);
}

// Simpan status permainan ke file
void saveGame(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Gagal menyimpan permainan.\n");
        return;
    }

    fprintf(file, "%d %d\n", nurse_x, nurse_y);
    fprintf(file, "%d %d\n", patient_x, patient_y);
    fprintf(file, "%d\n", help_count);
    fprintf(file, "%d\n", level);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Permainan berhasil disimpan ke file %s.\n", filename);
}

// Muat status permainan dari file
void loadGame(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Gagal memuat permainan.\n");
        return;
    }

    fscanf(file, "%d %d\n", &nurse_x, &nurse_y);
    fscanf(file, "%d %d\n", &patient_x, &patient_y);
    fscanf(file, "%d\n", &help_count);
    fscanf(file, "%d\n", &level);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fscanf(file, " %c", &board[i][j]);
        }
    }

    fclose(file);
    printf("Permainan berhasil dimuat dari file %s.\n", filename);
}

// Tingkatkan level permainan dan perbesar papan
void increaseLevel()
{
    level++;
    printf("Level meningkat! Level saat ini: %d\n", level);

    // Jika level meningkat, perbesar ukuran papan
    if (ROWS + level <= 10 && COLS + level <= 10)
    {
        ROWS++;
        COLS++;
        printf("Ukuran papan bertambah! Papan sekarang berukuran %d x %d.\n", ROWS, COLS);
        initializeBoard();
    }
}

// Main program
int main()
{
    srand(time(NULL));

    printf("----------------------------------------\n");
    printf("            MEDICAL MINI GAME           \n");
    printf("----------------------------------------\n");
    printf("Selamat datang Di Medical Mini Game!\n");
    printf("Apakah kamu ingin memuat Game ? (y/n): ");
    char load_input;
    scanf(" %c", &load_input);
    printf("----------------------------------------\n");

    if (load_input == 'y')
    {
        loadGame("game_save.csv");
        printf("Selamat bermain kembali!\n");
    }
    else
    {
        initializeBoard();
        printf("Permainan baru dimulai.\n");
    }
    printf("----------------------------------------\n");

    char input;
    int move = 0; // 0 untuk perawat, 1 untuk pasien

    while (1)
    {
        printBoard();
        printf("----------------------------------------\n");
        printf("Level: %d | Total bantuan: %d\n", level, help_count);
        printf("Posisi Perawat (*): (%d, %d)\n", nurse_x, nurse_y);
        printf("Posisi Pasien (#): (%d, %d)\n", patient_x, patient_y);
        printf("----------------------------------------\n");

        printf("%s's move! (gunakan w/a/s/d untuk bergerak, q untuk menyimpan & keluar)\n", move == 0 ? "Perawat (*)" : "Pasien (#)");
        printf("Masukkan langkah: ");
        scanf(" %c", &input);
        printf("----------------------------------------\n");

        if (input == 'q')
        {
            printf("----------------------------------------\n");
            saveGame("game_save.csv");
            printf("Permainan disimpan. Keluar dari permainan.\n");
            printf("----------------------------------------\n");

            break;
        }

        int dx = 0, dy = 0;
        getMove(input, &dx, &dy);

        int current_x = (move == 0) ? nurse_x : patient_x;
        int current_y = (move == 0) ? nurse_y : patient_y;

        int new_x = current_x + dx;
        int new_y = current_y + dy;

        if (isValidMove(new_x, new_y))
        {
            updatePosition(move == 0 ? '*' : '#', current_x, current_y, new_x, new_y);

            if (move == 0)
            {
                nurse_x = new_x;
                nurse_y = new_y;
            }
            else
            {
                patient_x = new_x;
                patient_y = new_y;
            }

            if (nurse_x == patient_x && nurse_y == patient_y)
            {
                help_count++;
                printf("Perawat menolong pasien! Total bantuan: %d\n", help_count);
                spawnNewPatient();
                if (help_count % 3 == 0)
                {
                    increaseLevel();
                }
            }

            move = 1 - move;
        }
        else
        {
            printf("Langkah tidak valid! Coba lagi.\n");
        }
    }

    return 0;
}
