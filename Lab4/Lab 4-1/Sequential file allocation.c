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
    int blocks_occupied;
} File;

// Function prototypes
void allocateFiles(File files[], int num_files);
void printFiles(File files[], int num_files);

int main() {
    int num_files;

    printf("File allocation method: Sequential\n");
    printf("Total blocks: %d\n", TOTAL_BLOCKS);
    printf("Size of each block: %d\n", BLOCK_SIZE);
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

void allocateFiles(File files[], int num_files) {
    int allocated_blocks[TOTAL_BLOCKS] = {0}; // Array to track allocated blocks

    for (int i = 0; i < num_files; i++) {
        int num_blocks = files[i].size / BLOCK_SIZE;
        if (files[i].size % BLOCK_SIZE != 0) {
            num_blocks++; // Round up if not evenly divisible
        }

        int start_block = -1;
        while (start_block == -1) {
            int rand_block = rand() % (TOTAL_BLOCKS - INODE_BLOCKS) + INODE_BLOCKS;

            if (allocated_blocks[rand_block] == 0) {
                int j;
                for (j = 0; j < num_blocks; j++) {
                    if (allocated_blocks[rand_block + j] == 1) {
                        break;
                    }
                }
                if (j == num_blocks) {
                    start_block = rand_block; // Start block found
                    for (j = 0; j < num_blocks; j++) {
                        allocated_blocks[rand_block + j] = 1; // Mark blocks as allocated
                    }
                }
            }
        }

        // Assign blocks occupied in the correct format
        files[i].blocks_occupied = start_block + INODE_BLOCKS;
        for (int j = 1; j < num_blocks; j++) {
           printf("-%d", start_block + INODE_BLOCKS + j);
        }
    }
}



void printFiles(File files[], int num_files) {
    printf("\nFile Allocation Table:\n");
    printf("%-20s %-10s %-20s\n", "File Name", "File Size", "Blocks Occupied");

    for (int i = 0; i < num_files; i++) {
        printf("%-20s %-10d ", files[i].name, files[i].size);

        // Print blocks occupied
        printf("%d", files[i].blocks_occupied);
        for (int j = 1; j < files[i].size; j++) {
            printf("-%d", files[i].blocks_occupied + j);
        }
        printf("\n");
    }
}

