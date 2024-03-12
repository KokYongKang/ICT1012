#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIPHER_KEY 3

int main() {
    char fileName[20], ch;
    FILE *fpTxtOriginal, *fpTxtEncrypt, *fpBinDecrypt, *fpBinData;
    char strTemp[100] = "", strOriginal[100] = "";
    char strEncrypted[100] = "", strDecrypted[100] = "";
    char strBin[100] = "";
    float floatBin = 0.0;
    unsigned char dataBin[300] = {0x00};
    int i, length, ret;

    printf("\nEnter the name of file to encrypt: ");
    scanf("%s", fileName);

    printf("Opening original file %s in text mode\n", fileName);
    fpTxtOriginal = fopen(fileName, "r");
    if(fpTxtOriginal == NULL) {
        printf("File does not exist or error in opening the file %s\n", fileName);
        exit(1);
    }

    printf("Opening file %s in text mode to write encrypted content\n", "Lab_EncryptedText.txt");
    fpTxtEncrypt = fopen("Lab_EncryptedText.txt", "w");
    if(fpTxtEncrypt == NULL) {
        printf("Error in creating %s\n", "Lab_EncryptedText.txt");
        fclose(fpTxtOriginal);
        exit(2);
    }

    while((ch = fgetc(fpTxtOriginal)) != EOF) {
        // Perform Caesar Cipher substitution encryption
        if (ch != ' ') {
            ch = ((ch - 'A') + CIPHER_KEY) % 26 + 'A';
        }
        fputc(ch, fpTxtEncrypt);
    }

    fclose(fpTxtOriginal);
    fclose(fpTxtEncrypt);

    fpTxtEncrypt = fopen("Lab_EncryptedText.txt", "r");
    if(fpTxtEncrypt == NULL) {
        printf(" File does not exist or error in opening %s\n", "Lab_EncryptedText.txt");
        exit(4);
    }

    printf("\nEncrypted content: ");
    if(fgets(strEncrypted, 100, fpTxtEncrypt) != NULL ) {
        puts(strEncrypted); // write content to stdout
    }

    fclose(fpTxtEncrypt);

    // Decrypt
    length = strlen(strEncrypted);
    for (i = 0; i < length; i++) {
        ch = strEncrypted[i];
        if (ch != ' ') {
            ch = ((ch - 'A') - CIPHER_KEY + 26) % 26 + 'A';
        }
        strncat(strDecrypted, &ch, 1);
    }
    printf("\nDecrypted content: %s\n", strDecrypted);

    fpBinDecrypt = fopen("Lab_BinData.bin", "wb");
    if(fpBinDecrypt == NULL) {
        printf("Error in creating file %s\n", "Lab_BinData.txt");
        fclose(fpBinDecrypt);
        exit(5);
    }

    ret = fputs(strDecrypted, fpBinDecrypt);

    float f = 1.234;
    fwrite(&f, sizeof(float), 1, fpBinDecrypt);

    for (i = 0x00; i <= 0xFF; i++) {
        ch = i;
        ret = fputc(ch, fpBinDecrypt);
    }
    fclose(fpBinDecrypt);

    printf("\nOpening bin mode data file, %s\n", "Lab_BinData.bin");
    fpBinData = fopen("Lab_BinData.bin", "rb"); // binary mode
    if(fpBinData == NULL) {
        printf("Error in creating file, %s\n", "Lab_BinData.bin");
        exit(6);
    }

    printf("\nRead string from bin mode file, %s: ", "Lab_BinData.bin");
    if(fgets(strBin, 44, fpBinData) != NULL ) {
        printf("<%s>", strBin);
    }
    printf("\n"); // newline

    printf("\nRead float from bin mode file, %s: ", "Lab_BinData.bin");
    fread(&floatBin, sizeof(float), 1, fpBinData);
    printf("<%f>", floatBin);

    printf("\n"); // newline
    printf("\nRead binary data from bin mode file, %s: ", "Lab_BinData.bin");
    if(fread(dataBin, sizeof(dataBin), 1, fpBinData) < 300 ) {
        for (i = 0; i < 256; i++) {
            if((i % 16) == 0)
                printf("\n"); // newline
            printf("[%02X]", dataBin[i]);
        }
    }

    printf("\nClosing bin mode data file, %s\n\n", "Lab_BinData.bin");
    fclose(fpBinData);

    return 0;
}
