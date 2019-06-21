#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>

// Function prototype
void checkPossiblePasswords(int maxPassLength, string possPass, string hash, string salt);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: crack <hash>\n");
        return 1;
    }
    // Max length of password to be cracked
    int maxPassLength = 5;
    // Create a string of 5 possible characters + string end (\0)
    char possPass[] = "\0\0\0\0\0\0";
    // Get the given hash
    string hash = argv[1];
    // Get the salt out of the first 2 characters of the hash
    char salt[3];
    memcpy(salt, hash, 2);
    salt[2] = '\0';
    // Call the function to create and compare the passwords
    checkPossiblePasswords(maxPassLength, possPass, hash, salt);
}

// This is a recursive function
void checkPossiblePasswords(int maxPassLength, string possPass, string hash, string salt)
{
    // The possible characters to create the password from
    char possChar[] = "\0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // 1 (\0) + 26 (Capital Letters) + 26 (Small Letters)
    int possCharLength = 53;
    
    // The base case
    if (maxPassLength == 1)
    {
        for (int i = 1; i < possCharLength; i++)
        {
            // Change the first character only and leave the others
            possPass[0] = possChar[i];
            // Compare the created possible password with the given hash
            if (strcmp(crypt(possPass, salt), hash) == 0)
                        {
                            printf("%s\n", possPass);
                            exit(0);
                        }
        }
    } else // The recursive case
    {
        for (int i = 0; i < possCharLength; i++)
        {
            // Change the current character then call the recursive function
            possPass[maxPassLength-1] = possChar[i];
            checkPossiblePasswords(maxPassLength-1, possPass, hash, salt);
        }
    }
}