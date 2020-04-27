#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int print_whole_file() {
    FILE *fp;
    int c, i = -1, max;

    fp = fopen("../gpt.iso", "rb");

    if (fp == NULL) {
        fprintf(stderr, "cannot open input file\n");
        return 1;
    }

    //for (i = 0, max = 4900; i < max && (c = getc(fp)) != EOF; i++) {
    while ((c = getc(fp)) != EOF) {
        i++;
        printf("%02x %c \t", c, (char) c);
        //printf("%c", (char) c);
        if (i % 16 == 15) putchar('\n');  // 16 bytes/line
        else if (i % 2 == 1) putchar(' ');
    }

    fclose(fp);

    return 0;
}

FILE *fp;

int file_open(char filename[]) { // open file with filename
    fp = fopen(filename, "rb");

    if (fp == NULL) {
        fprintf(stderr, ANSI_COLOR_RED "cannot open input file\n");
        return 1; // return if error occurs
    }
    return 0;
}

int file_close() {
    return fclose(fp); // close open file
}

void file_seek(long seek) { // jump to byte
    fseek(fp, seek, SEEK_CUR);
}

void file_print(int bytes_to_read, int print_chars) {
    char buffer[bytes_to_read]; // create buffer with length of bytes to read

    fread(buffer, 1, bytes_to_read, fp); // read given bytes

    for (int i = 0; i < bytes_to_read; ++i) {
        printf(ANSI_COLOR_MAGENTA "%02x ", buffer[i]); // print as hex

        if (i % 16 == 15) putchar('\n');  // 16 bytes/line
        else if (i % 2 == 1) putchar(' ');
    }
    printf("\n");

    if (print_chars) {
        for (int i = 0; i < bytes_to_read; ++i) printf(ANSI_COLOR_CYAN "%c ", (char) buffer[i]); // print as char
        printf("\n");
    }
    printf("\n");
}

int main() {
    if (file_open("../gpt.iso")) return 1;

    file_seek(512L);

    printf(ANSI_COLOR_GREEN "Offset 0\t\tLength: 8 bytes\t\tSignatur („EFI PART“, 45h 46h 49h 20h 50h 41h 52h 54h)\n");
    file_print(8, 1);

    printf(ANSI_COLOR_GREEN "Offset 8\t\tLength: 4 bytes\t\tRevision (00h 00h 01h 00h)\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 12\t\tLength: 4 bytes\t\tHeader-Größe – Little Endian (5Ch 00h 00h 00h entspricht 92 bytes)\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 16\t\tLength: 4 bytes\t\tHeader-CRC32-Prüfsumme\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 20\t\tLength: 4 bytes\t\tReservierter Bereich – muss Null (0) sein\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 24\t\tLength: 8 bytes\t\tPosition des eigenen LBA (dieses Headers)\n");
    file_print(8, 0);

    printf(ANSI_COLOR_GREEN "Offset 32\t\tLength: 8 bytes\t\tPosition des Backup-LBA (des anderen Headers)\n");
    file_print(8, 0);

    printf(ANSI_COLOR_GREEN "Offset 40\t\tLength: 8 bytes\t\tErster benutzbarer LBA für Partitionen (Letzter LBA der primären Partitionstabelle + 1, normalerweise 34)\n");
    file_print(8, 0);

    printf(ANSI_COLOR_GREEN "Offset 48\t\tLength: 8 bytes\t\tLetzter benutzbarer LBA (Erster LBA der sekundären Partitionstabelle – 1, normalerweise Datenträgergröße – 34)\n");
    file_print(8, 0);

    printf(ANSI_COLOR_GREEN "Offset 56\t\tLength: 16 bytes\tDatenträger-GUID\n");
    file_print(16, 0);

    printf(ANSI_COLOR_GREEN "Offset 72\t\tLength: 8 bytes\t\tStart-LBA der Partitionstabelle\n");
    file_print(8, 0);

    printf(ANSI_COLOR_GREEN "Offset 80\t\tLength: 4 bytes\t\tAnzahl der Partitionseinträge (Partitionen)\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 84\t\tLength: 4 bytes\t\tGröße eines Partitionseintrags (normalerweise 128)\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offset 88\t\tLength: 4 bytes\t\tPartitionstabellen-CRC32-Prüfsumme\n");
    file_print(4, 0);

    printf(ANSI_COLOR_GREEN "Offsett 92\t\tLength: *\t\t\tReservierter Bereich; muss mit Nullen, für den Rest des Blocks, belegt sein (420 Bytes bei einem 512-byte LBA)\n");
    file_print(420,0);

    file_close();
    return 0;
}