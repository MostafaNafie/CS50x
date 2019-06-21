#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    // Checks that th euser enterd 1 argument only (the key)
    if (argc != 2) {
        printf("Usage ./caesar key\n");
        return 1;
    } else {
        // checks that the key is digits only
        for (int i = 0, j = strlen(argv[1]); i < j; i++) {
            if (isdigit(argv[1][i]) == 0) {
                printf("Usage ./caesar key\n");
                return 1;
            }
        }
        int key = atoi(argv[1]);
        // Gets the plain text from the user
        string plainText = get_string("plaintext: ");
        // Encoding the plain text
        for (int i = 0, j = strlen(plainText); i < j; i++){
            if (isalpha(plainText[i]) != 0) {
                if (isupper(plainText[i]) != 0) {
                    plainText[i] = ((plainText[i] - 'A') + key) % 26 + 'A';
                } else {
                    plainText[i] = ((plainText[i] - 'a') + key) % 26 + 'a';
                }
            }
        }
        printf("ciphertext: %s\n", plainText);
        return 0;
    }
}
