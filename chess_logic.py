import sys
import serial

# Initialize serial connection
ser = serial.Serial('/dev/ttyACM0', 9600)

# Continuously listen for voice input!
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
    #if not isLegal(row1, col1, row2, col2):
     #   print "Illegal move!"
      #  continue
    # perform move software-side (including changing players)
    #performMove(row1, col1, row2, col2)
    # perform move hardware-side
    #kill_piece = 0 if (board[row2][col2] == None) else 1
    ser.write(str(row1) + " " + str(col1) + " " + str(row2) + " " + str(col2) + " " + str(0))
    # check for check(mate)?
