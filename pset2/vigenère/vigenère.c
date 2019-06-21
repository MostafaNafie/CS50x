#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    // Checks that th euser enterd 1 argument only (the key)
    if (argc != 2) {
        printf("Usage ./vigenere keyword\n");
        return 1;
    } else {
        // checks that the key is digits only
        for (int i = 0, j = strlen(argv[1]); i < j; i++) {
            if (isalpha(argv[1][i]) == 0) {
                printf("Usage ./vigenere keyword\n");
                return 1;
            }
        }
        int m = 0;
        // Gets the plain text from the user
        string plainText = get_string("plaintext: ");
        // Encoding the plain text
        for (int i = 0, j = strlen(plainText); i < j; i++){
            if (isalpha(plainText[i]) != 0) {               
                int key = shift(argv[1][m]);
                if (isupper(plainText[i]) != 0) {
                    plainText[i] = ((plainText[i] - 'A') + key) % 26 + 'A';
                } else {
                    plainText[i] = ((plainText[i] - 'a') + key) % 26 + 'a';
                }
                if (m < strlen(argv[1]) - 1) {
                    m++;
                } else {
                    m = 0;
                } 
            }
        }
        printf("ciphertext: %s\n", plainText);
        return 0;
    }
}

int shift(char c)
{
    int key;
    if (isupper(c)) {
        key = c - 'A';
    } else {
        key = c - 'a';
    }
    return key;
}
