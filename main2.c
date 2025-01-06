#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 5
#define COLS 5

// Representasi papan permainan
char board[ROWS][COLS];

// Posisi pemain
int nurse_x = 0, nurse_y = 0;                   // Perawat mulai di pojok kiri atas
int patient_x = ROWS - 1, patient_y = COLS - 1; // Pasien mulai di pojok kanan bawah

// Skor bantuan
int help_count = 0;

// Inisialisasi papan permainan
void initializeBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = '.'; // Kosongkan semua posisi dengan "."
        }
    }
    board[nurse_x][nurse_y] = 'N';     // Tempatkan perawat
    board[patient_x][patient_y] = 'P'; // Tempatkan pasien
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

// Cetak koordinat pemain
void printCoordinates()
{
    printf("Posisi Perawat (N): (%d, %d)\n", nurse_x, nurse_y);
    printf("Posisi Pasien (P): (%d, %d)\n", patient_x, patient_y);
    printf("\n");
}

// Simpan status permainan ke file CSV
void saveGameToCSV(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    fprintf(file, "Board\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fprintf(file, "%c%c", board[i][j], j == COLS - 1 ? '\n' : ',');
        }
    }

    fprintf(file, "\nNurse,%d,%d\n", nurse_x, nurse_y);
    fprintf(file, "Patient,%d,%d\n", patient_x, patient_y);
    fprintf(file, "HelpCount,%d\n", help_count);

    fclose(file);
    printf("Status permainan telah disimpan ke file '%s'.\n", filename);
}

// Muat status permainan dari file CSV
void loadGameFromCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Gagal membuka file untuk memuat data.\n");
        return;
    }

    char line[256];
    int row = 0;

    // Baca papan permainan
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == 'N' || line[0] == 'P' || line[0] == 'H')
        {
            break;
        }

        for (int col = 0, i = 0; col < COLS; col++, i += 2)
        {
            board[row][col] = line[i];
        }
        row++;
    }

    // Baca posisi perawat
    fscanf(file, "Nurse,%d,%d\n", &nurse_x, &nurse_y);

    // Baca posisi pasien
    fscanf(file, "Patient,%d,%d\n", &patient_x, &patient_y);

    // Baca jumlah bantuan
    fscanf(file, "HelpCount,%d\n", &help_count);

    fclose(file);
    printf("Status permainan telah dimuat dari file '%s'.\n", filename);
}

// Periksa apakah langkah valid
bool isValidMove(int x, int y)
{
    return x >= 0 && x < ROWS && y >= 0 && y < COLS;
}

// Perbarui posisi pemain di papan
void updatePosition(char player, int old_x, int old_y, int new_x, int new_y)
{
    board[old_x][old_y] = '.';    // Kosongkan posisi lama
    board[new_x][new_y] = player; // Tempatkan pemain di posisi baru
}

// Dapatkan pergeseran langkah berdasarkan input untuk perawat
void getMoveNurse(char input, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    if (input == 'w')
    {
        *dx = -1;
    }
    else if (input == 's')
    {
        *dx = 1;
    }
    else if (input == 'a')
    {
        *dy = -1;
    }
    else if (input == 'd')
    {
        *dy = 1;
    }
}

// Dapatkan pergeseran langkah berdasarkan input untuk pasien tanpa pointer
void getMovePatient(char input, int dx_dy[2])
{
    dx_dy[0] = 0;
    dx_dy[1] = 0;
    if (input == 'w')
    {
        dx_dy[0] = -1;
    }
    else if (input == 's')
    {
        dx_dy[0] = 1;
    }
    else if (input == 'a')
    {
        dx_dy[1] = -1;
    }
    else if (input == 'd')
    {
        dx_dy[1] = 1;
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
    } while (board[new_x][new_y] != '.'); // Pastikan posisi kosong

    patient_x = new_x;
    patient_y = new_y;
    board[patient_x][patient_y] = 'P';
    printf("Pasien baru muncul di posisi: (%d, %d)\n", patient_x, patient_y);
}

// Menu utama
void mainMenu()
{
    char choice;
    while (1)
    {
        printf("================ MENU UTAMA ================\n");
        printf("1. Pasien vs Perawat\n");
        printf("2. Perawat Menolong Pasien\n");
        printf("3. Keluar\n");
        printf("Pilih opsi: ");
        scanf(" %c", &choice);

        if (choice == '1')
        {
            printf("\n=== Pasien vs Perawat ===\n");
            printf("Mulai permainan baru atau muat permainan? (n: baru, l: load): ");
            scanf(" %c", &choice);
            if (choice == 'l')
            {
                loadGameFromCSV("game_state_pvp.csv");
            }
            else
            {
                initializeBoard();
            }

            printf("Game Dimulai!\n\n");
            int turn = 0; // 0 untuk perawat, 1 untuk pasien
            while (1)
            {
                printBoard();
                printCoordinates();
                printf("%s's turn! (gunakan w/a/s/d untuk bergerak)\n", turn == 0 ? "Perawat (N)" : "Pasien (P)");

                char input;
                int dx = 0, dy = 0;
                printf("Masukkan langkah: ");
                scanf(" %c", &input);

                if (turn == 0)
                {
                    getMoveNurse(input, &dx, &dy);
                }
                else
                {
                    int dx_dy[2] = {0, 0};
                    getMovePatient(input, dx_dy);
                    dx = dx_dy[0];
                    dy = dx_dy[1];
                }

                int current_x = (turn == 0) ? nurse_x : patient_x;
                int current_y = (turn == 0) ? nurse_y : patient_y;

                int new_x = current_x + dx;
                int new_y = current_y + dy;

                if (isValidMove(new_x, new_y))
                {
                    updatePosition(turn == 0 ? 'N' : 'P', current_x, current_y, new_x, new_y);

                    if (turn == 0)
                    {
                        nurse_x = new_x;
                        nurse_y = new_y;
                    }
                    else
                    {
                        patient_x = new_x;
                        patient_y = new_y;
                    }

                    // Check if the nurse reaches the patient
                    if (nurse_x == patient_x && nurse_y == patient_y)
                    {
                        printf("Perawat berhasil mencapai pasien!\n");
                        help_count++;
                        printf("Jumlah bantuan: %d\n", help_count);

                        // Spawn a new patient
                        spawnNewPatient();
                    }

                    // Switch turn
                    turn = 1 - turn;
                }
                else
                {
                    printf("Langkah tidak valid. Coba lagi.\n");
                }

                // Save the game after every move
                saveGameToCSV("game_state_pvp.csv");
            }
        }
        else if (choice == '2')
        {
            printf("\n=== Perawat Menolong Pasien ===\n");
            initializeBoard();
            printf("Game Dimulai!\n\n");

            while (1)
            {
                printBoard();
                printCoordinates();

                printf("Giliran Perawat (N)! (gunakan w/a/s/d untuk bergerak, q untuk keluar)\n");
                char input;
                int dx = 0, dy = 0;

                printf("Masukkan langkah: ");
                scanf(" %c", &input);

                if (input == 'q')
                {
                    printf("Game berakhir.\n");
                    break;
                }

                getMoveNurse(input, &dx, &dy);

                int new_x = nurse_x + dx;
                int new_y = nurse_y + dy;

                if (isValidMove(new_x, new_y))
                {
                    updatePosition('N', nurse_x, nurse_y, new_x, new_y);
                    nurse_x = new_x;
                    nurse_y = new_y;

                    if (nurse_x == patient_x && nurse_y == patient_y)
                    {
                        printf("Perawat berhasil menolong pasien!\n");
                        help_count++;
                        printf("Jumlah bantuan: %d\n", help_count);

                        // Spawn a new patient
                        spawnNewPatient();
                    }
                }
                else
                {
                    printf("Langkah tidak valid. Coba lagi.\n");
                }

                // Save the game after every move
                saveGameToCSV("game_state_help.csv");
            }
        }
        else if (choice == '3')
        {
            printf("Terima kasih telah bermain!\n");
            break;
        }
        else
        {
            printf("Pilihan tidak valid. Coba lagi.\n");
        }
    }
}

int main()
{
    srand(time(NULL)); // Seed untuk posisi pasien acak
    mainMenu();
    return 0;
}
