#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_BLOCKS 32
#define MIN_FILE_SIZE 1
#define MAX_FILE_SIZE 24
#define BLOCK_SIZE 1
#define INODE_BLOCKS 8

// Structure to represent a block in the disk
typedef struct Block {
    int block_number;
    struct Block* next;
} Block;

// Structure to represent a file
typedef struct File {
    char name[20];
    int size;
    Block* start_block;
    Block* end_block;
} File;

// Function prototypes
void allocateFiles(File files[], int num_files);
void printFiles(File files[], int num_files);

int main() {
    int num_files;

    printf("File allocation method: Linked List\n");
    printf("Total blocks: %d\n", TOTAL_BLOCKS);
    printf("File allocation start at block: %d\n", INODE_BLOCKS);
    printf("File allocation end at block: %d\n", TOTAL_BLOCKS - 1);

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
        files[i].start_block = NULL;
        files[i].end_block = NULL;

        int num_blocks = files[i].size / BLOCK_SIZE;
        if (files[i].size % BLOCK_SIZE != 0) {
            num_blocks++; // Round up if not evenly divisible
        }

        for (int j = 0; j < num_blocks; j++) {
            int rand_block = rand() % (TOTAL_BLOCKS - INODE_BLOCKS) + INODE_BLOCKS;

            // Create a new block
            Block* new_block = (Block*)malloc(sizeof(Block));
            if (new_block == NULL) {
                printf("Memory allocation failed. Exiting...\n");
                exit(1);
            }
            new_block->block_number = rand_block;
            new_block->next = NULL;

            // Update the linked list of blocks for the file
            if (files[i].start_block == NULL) {
                files[i].start_block = new_block;
                files[i].end_block = new_block;
            } else {
                files[i].end_block->next = new_block;
                files[i].end_block = new_block;
            }
        }
    }
}

// Function to print allocated files
void printFiles(File files[], int num_files) {
    printf("\nFile Allocation Table:\n");
    printf("%-20s %-10s %-20s\n", "File Name", "File Size", "Blocks Allocated");

    for (int i = 0; i < num_files; i++) {
        printf("%-20s %-10d ", files[i].name, files[i].size);

        // Print blocks allocated for the file
        Block* current_block = files[i].start_block;
        printf("%d", current_block->block_number);
        current_block = current_block->next;
        while (current_block != NULL) {
            printf("-%d", current_block->block_number);
            current_block = current_block->next;
        }
        printf("\n");
    }
}
