from nltk.tokenize import sent_tokenize, word_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    # Split each string into lines
    linesInA = a.split("\n")
    linesInB = b.split("\n")
    #print(linesInA)
    #print(linesInB)
    # initialize a list of the similar lines
    similarLines = []
    # iterate through line in a and check if it exits in b
    for line in linesInA:
        if line in linesInB:
            similarLines.append(line)
    # remove duplicates from the list
    similarLines = list(dict.fromkeys(similarLines))

    return similarLines


def sentences(a, b):
    """Return sentences in both a and b"""

    # Split each string into sentences
    sentencesInA = sent_tokenize(a)
    sentencesInB = sent_tokenize(b)
    # initialize a list of the similar sentences
    similarSentences= []
    # iterate through sentences in a and check if it exits in b
    for sentence in sentencesInA:
        if sentence in sentencesInB:
            similarSentences.append(sentence)
    # remove duplicates from the list
    similarSentences = list(dict.fromkeys(similarSentences))
    
    return similarSentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # Extract substrings from each string
    subInA = []
    subInB = []
    for i in range(len(a) - (n-1)):
        subInA.append(a[i:i + n])
    print(subInA)
    for i in range(len(b) - (n-1)):
        subInB.append(b[i:i + n])
    # initialize a list of the similar substrings
    similarSubstrings= []
    # iterate through sentences in a and check if it exits in b
    for substring in subInA:
        if substring in subInB:
            similarSubstrings.append(substring)
    # remove duplicates from the list
    similarSubstrings = list(dict.fromkeys(similarSubstrings))

    return similarSubstrings
