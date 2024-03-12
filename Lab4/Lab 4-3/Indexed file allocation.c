#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_BLOCKS 32
#define MIN_FILE_SIZE 1
#define MAX_FILE_SIZE 24
#define BLOCK_SIZE 1
#define INODE_BLOCKS 8

// Structure to represent a file
typedef struct File {
    char name[20];
    int size;
    int* blocks_allocated;
} File;

// Function prototypes
void allocateFiles(File files[], int num_files);
void printFiles(File files[], int num_files);

int main() {
    int num_files;

    printf("File allocation method: Linked list\n");
    printf("Total blocks: %d\n", TOTAL_BLOCKS);
    printf("File allocation start at block : 8\n");
    printf("File allocation end at block : 31\n");
    printf("Enter no of files: ");
    scanf("%d", &num_files);

    if (num_files < 1 || num_files > MAX_FILE_SIZE) {
        printf("Invalid number of files. Exiting...\n");
        return 1;
    }

    File files[num_files];

    // Input file names and sizes
    for (int i = 0; i < num_files; i++) {
        printf("Enter the name of file #%d: ", i + 1);
        scanf("%s", files[i].name);
        printf("Enter the size(kB) of file #%d: ", i + 1);
        scanf("%d", &files[i].size);

        if (files[i].size < MIN_FILE_SIZE || files[i].size > MAX_FILE_SIZE) {
            printf("Invalid file size. Exiting...\n");
            return 1;
        }
    }

    // Seed the random number generator
    srand(time(NULL));

    // Allocate files
    allocateFiles(files, num_files);

    // Print allocated files
    printFiles(files, num_files);

    return 0;
}

// Function to allocate blocks for files using linked list allocation
void allocateFiles(File files[], int num_files) {
    for (int i = 0; i < num_files; i++) {
        files[i].blocks_allocated = (int*)malloc(files[i].size * sizeof(int));
        if (files[i].blocks_allocated == NULL) {
            printf("Memory allocation failed. Exiting...\n");
            exit(1);
        }

        int num_blocks = files[i].size;

        // Allocate blocks for the file
        for (int j = 0; j < num_blocks; j++) {
            int rand_block;
            do {
                rand_block = rand() % (TOTAL_BLOCKS - INODE_BLOCKS) + INODE_BLOCKS;
            } while (files[i].blocks_allocated[j] == rand_block);

            files[i].blocks_allocated[j] = rand_block;
        }
    }
}

// Function to print allocated files
void printFiles(File files[], int num_files) {
    printf("\nFile Allocation Table:\n");
    printf("%-20s %-10s %-20s\n", "File Name", "File Size", "BLOCKS_OCCUPIED");

    for (int i = 0; i < num_files; i++) {
        printf("%-20s %-10d ", files[i].name, files[i].size);

        // Print blocks occupied for the file
        for (int j = 0; j < files[i].size; j++) {
            printf("%d", files[i].blocks_allocated[j]);
            if (j < files[i].size - 1) {
                printf("-");
            }
        }
        printf("\n");
    }
}
