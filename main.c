#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
        printf(ANSI_COLOR_MAGENTA "0x%02x\t", buffer[i]); // print as hex
        if (i % 16 == 15 && i != bytes_to_read-1) putchar('\n');  // 16 bytes/line
    }
    printf("\n");

    for (int i = 0; i < bytes_to_read; ++i) {
        printf(ANSI_COLOR_CYAN "%d\t\t", buffer[i]); // print as hex
        if (i % 16 == 15 && i != bytes_to_read-1) putchar('\n');  // 16 bytes/line
    }
    printf("\n");

    if (print_chars) {
        for (int i = 0; i < bytes_to_read; ++i) printf(ANSI_COLOR_BLUE "%c\t\t", (char) buffer[i]); // print as char
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf(ANSI_COLOR_RED "missing attribute");
        return 1;
    }

    if (file_open(*(argv+1))) return 1;

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

    fseek(fp, 512L, SEEK_SET);

    for (int i = 2; i <= 33; ++i) {
        printf("\n\n\n\n\n");
        printf(ANSI_COLOR_RED "LBA-%d\n", i);

        printf(ANSI_COLOR_GREEN "Partitionstyp-GUID\n");
        file_print(16, 0);

        printf(ANSI_COLOR_GREEN "Eindeutige Partitions-GUID\n");
        file_print(16, 0);

        printf(ANSI_COLOR_GREEN "Beginn der Partition (erster LBA – Little-Endian)\n");
        file_print(8, 0);

        printf(ANSI_COLOR_GREEN "Ende der Partition (letzter LBA – inklusive)\n");
        file_print(8, 0);

        printf(ANSI_COLOR_GREEN "Attribute (siehe folgende Tabelle)\n");
        file_print(8, 0);

        printf(ANSI_COLOR_GREEN "Partitionsname (36 UTF-16LE-Zeichen)\n");
        file_print(72, 0);
    }

    file_close();
    return 0;
}