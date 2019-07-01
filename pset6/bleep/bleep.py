from cs50 import get_string
from sys import argv


def main():
    # checks that the user entered 1 argument only (the key)
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    # load the banned words in the file into memory
    file = open(argv[1], "r")
    bannedWords = set()
    for line in file:
        bannedWords.add(line.rstrip("\n"))
    file.close()

    # get the message
    message = get_string("What message would you like to censor?\n").strip()

    # get the words of the message into a list
    messageWords = message.split()

    # loop through the message words and check banned words
    for i in range(len(messageWords)):
        if messageWords[i].lower() in bannedWords:
            messageWords[i] = "*" * len(messageWords[i])
    
    # print the message after modification
    print(*messageWords)


if __name__ == "__main__":
    main()
