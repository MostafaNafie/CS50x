import cs50
import sys

# checks that the user entered 1 argument only (the key)
if len(sys.argv) != 2:
    print("Usage ./caesar key")
    exit(1)
else:
    # checks that the key is digits only
    if not sys.argv[1].isdigit():
        print("Usage ./caesar key")
        exit(1)

key = int(sys.argv[1])

# Gets the plain text from the user and save it as a list
# Because string are immutable in python and I want to change each letter with its encoded one
plainText = list(cs50.get_string("plaintext: "))

# Encoding the plain text
for i in range(len(plainText)):
    if plainText[i].isalpha():
        if  plainText[i].isupper():
            cipheredLetter = ((ord(plainText[i]) - ord('A')) + key) % 26 + ord('A')
            plainText[i] = chr(cipheredLetter)
        else:
            cipheredLetter = ((ord(plainText[i]) - ord('a')) + key) % 26 + ord('a')
            plainText[i] = chr(cipheredLetter)

# print the encrypted text
print("ciphertext: " + "".join(plainText))