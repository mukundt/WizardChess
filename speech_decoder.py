import sys

def wordToCol(word):
    if word == "AZKABAN":
        return 0
    elif word == "BUTTERBEER":
        return 1
    elif word == "CENTAUR":
        return 2
    elif word == "DEMENTOR":
        return 3
    elif word == "EXPELLIARMUS":
        return 4
    elif word == "FIREBOLT":
        return 5
    elif word == "GRYFFINDOR":
        return 6
    elif word == "HOGWARTS":
        return 7
    else:
        return -1

while True:
    input_string = sys.stdin.readline()
    print "Decoder received: " + input_string
    if not input_string.startswith('Wizard Chess:'):
        print "Bad input!"
        continue
    input_string = input_string[14:]
    print "Processed input: " + input_string
    input_words = input_string.split();
    print "Input length: " + str(len(input_words))
    for item in input_words:
        print "item = " + item
    if len(input_words) != 5:
        print "Bad input!"
        continue

    row1, col1, row2, col2 = int(input_words[1]) - 1, wordToCol(input_words[0]), int(input_words[4]) - 1, wordToCol(input_words[3])
    print row1, col1, row2, col2
