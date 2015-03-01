from dodona import core

def AlphaOnly(word):
    return ''.join(c for c in word.lower() if c.isalpha())

def ReadBook(book, wordlist=None, cleaner=AlphaOnly):
    if wordlist == None:
        wordlist = core.WordList()
    with open(book, "r", errors="replace") as f:
        for line in f:
            for word in line.split():
                word = cleaner(word)
                if len(word) > 0:
                    wordlist.AddWord(word)
    return wordlist

def MostCommon(wordlist, N):
    w2 = core.WordList()
    for i in range(min(N,wordlist.Words())):
        w2.AddWord(wordlist.Word(i), wordlist.Occurances(i))
    return w2
