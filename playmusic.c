#include <windows.h>
#include <stdio.h>
#include <MMsystem.h>
#pragma comment(lib, "winmm.lib") // Pastikan pustaka winmm.lib digunakan

int main()
{

    PlaySound(TEXT("D:\\Pemrograman\\Minigame-C\\src\\music.wav"), NULL, SND_SYNC | SND_FILENAME);
    printf("Musik diputar. Tekan Enter untuk keluar.\n");
    // getchar();
    return 0;
}
