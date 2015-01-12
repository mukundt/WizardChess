# Wizard's Chess
# based on Kiran Pandit's 15-112 Term Project

import sys

def isLegal(row1, col1, row2, col2):
    #(row1,col1) where piece is starting
    #(row2,col2) where piece is going
    piece = board[row1][col1]
    drow = row2 - row1
    dcol = col2 - col1
    #there's nothing there!
    if piece==None:
        return False
    if board[row2][col2] != None: #check that it isn't taking own piece
        if (isWTurn == True) and (board[row2][col2][0] == 'w'): return False
        elif (isWTurn == False) and (board[row2][col2][0] == 'b'): return False
    if "Pawn" in piece: return isPawnLegal(row1,col1,row2,col2,drow,dcol)
        elif "Knight" in piece: return isKnightLegal(drow,dcol)
        elif "Bishop" in piece: return isBishopLegal(row1,col1,drow,dcol)
        elif "Rook" in piece: return isRookLegal(row1,col1,drow,dcol)
        elif "Queen" in piece: return isQueenLegal(row1,col1,drow,dcol)
        elif "King" in piece: return isKingLegal(drow,dcol,row2,col2)

def isPawnLegal(row1, col1, row2, col2, drow, dcol):
        #have to en passant in the next turn
        if isWTurn:
            #moves 2 if it is on first move
            if (row1 == 6) and (board[row2][col2] == None) and (drow == -2 and dcol == 0\
                and board[row1 - 1][col1] == None):
                nextEnPassantCol = col2
                return True
            #single move
            elif (board[row2][col2] == None) and (drow == -1) and (dcol == 0):
                nextEnPassantCol = None
                return True
            #captures piece
            elif (board[row2][col2] != None) and (drow == -1) and (abs(dcol) == 1):
                nextEnPassantCol = None
                return True
            #en passant
            elif (nextEnPassantCol != None) and (row1 == 3) and \
                (col2 == nextEnPassantCol) and (abs(col2-col1) == 1) and (row2 == 2):
                enPassant = True
                return True
        else:
            if (row1 == 1) and (board[row2][col2] == None) and (drow == 2) and (dcol == 0)\
                and (board[row1 + 1][col2] == None):
                nextEnPassantCol = col2
                return True
            elif (board[row2][col2] == None) and (drow == 1) and (dcol == 0):
                nextEnPassantCol = None
                return True
            elif (board[row2][col2] != None) and (drow == 1) and (abs(dcol) == 1):
                nextEnPassantCol = None
                return True
            elif (nextEnPassantCol != None) and (row1 == 4) and \
                (col2 == nextEnPassantCol) and (abs(col2-col1) == 1) and (row2 == 5):
                enPassant = True
                return True
        return False

def isKnightLegal(drow, dcol):
        if (abs(drow) == 2 and abs(dcol) == 1) or (abs(drow) == 1 and abs(dcol) == 2):
            return True
        else: return False

def isBishopLegal(row1, col1, drow, dcol):
        if abs(drow) != abs(dcol): return False
        elif abs(drow)>1:
            #need to make sure nothing in the middle
            if drow>0 and dcol>0: #midrow and midcol are equal
                for midrow in xrange(1,drow):
                    if board[row1+midrow][col1+midrow]!=None:
                        return False
            if drow>0 and dcol<0:
                for midrow in xrange(1,drow):
                    for midcol in xrange(dcol+1,0):
                        if midrow==abs(midcol):
                            if board[row1+midrow][col1+midcol]!=None:
                                return False
            if drow<0 and dcol>0:
                for midrow in xrange(drow+1,0):
                    for midcol in xrange(1,dcol):
                        if abs(midrow)==midcol:
                            if board[row1+midrow][col1+midcol]!=None:
                                return False
            if drow<0 and dcol<0:
                for midrow in xrange(drow+1,0): #midrow and midcol are equal
                    if board[row1+midrow][col1+midrow]!=None:
                        return False
        return True

def isRookLegal(row1, col1, drow, dcol):
        if drow==0 or dcol==0:
            if drow==0 and abs(dcol)>1:
                if dcol<0:
                    for midcol in xrange(dcol+1,0):
                        if board[row1][col1+midcol]!=None:
                            return False
                else:
                    for midcol in xrange(1,dcol):
                        if board[row1][col1+midcol]!=None:
                            return False
            elif dcol==0 and abs(drow)>1:
                if drow<0:
                    for midrow in xrange(drow+1,0):
                        if board[row1+midrow][col1]!=None:
                            return False
                else:
                    for midrow in xrange(1,drow):
                        if board[row1+midrow][col1]!=None:
                            return False
            return True
        return False

def isQueenLegal(row1, col1, drow, dcol):
        #queen basically combines rook and bishop
        if isRookLegal(row1,col1,drow,dcol): return True
        elif isBishopLegal(row1,col1,drow,dcol): return True
        else: return False

    def isKingLegal(drow, dcol, row2, col2):
        if abs(drow)>1 or abs(dcol)>1:
            return False
        if check(row2,col2)==True:
            return False
        else: return True

    def check(row2, col2):
        for row in xrange(rows):
            for col in xrange(cols):
                #checks legal moves for all of the pieces
                if board[row][col]!=None:
                    if "King" not in board[row][col]:
                        #checking for black pieces
                        if isWTurn and board[row][col][0]!='w':
                            checkForLegalMovesB(row,col)
                            legalMoves=copy.deepcopy(legalMovesB)
                            if (row2,col2) in legalMoves:
                                return True
                        elif isWTurn==False and board[row][col][0]!='b':
                            checkForLegalMovesB(row,col)
                            legalMoves=copy.deepcopy(legalMovesB)
                            if (row2,col2) in legalMoves:
                                return True
                    #if a pawn is in a position to capture the king
                    if isWTurn:
                        if row2>0:
                            if col2>0 and board[row2-1][col2-1]!=None:
                                if "bPawn" in board[row2-1][col2-1]:
                                    return True
                            if col2<7 and board[row2-1][col2+1]!=None:
                                if "bPawn" in board[row2-1][col2+1]:
                                    return True
                    else:
                        if row2<7:
                            if col2>0 and board[row2+1][col2-1]!=None:
                                if "wPawn" in board[row2+1][col2-1]:
                                    return True
                            if col2<7 and board[row2+1][col2+1]!=None:
                                if "wPawn" in board[row2+1][col2+1]:
                                    return True
        return False

def checkForEndGame():
        #checks if king has been captured
        wKing=bKing=False
        for row in xrange(rows):
            for col in xrange(cols):
                if board[row][col]=='wKing':
                    wKing=True
                if board[row][col]=='bKing':
                    bKing=True
        if wKing==False:
            isGameOver=True
            winner='black'
            return
        elif bKing==False:
            isGameOver=True
            winner='white'
            return

def changePlayers():
        if isWTurn==True:
            isWTurn=False
        else: isWTurn=True

def init(self):
        board=[['bRook1','bKnight1','bBishop1','bQueen1',
                    'bKing','bBishop2','bKnight2','bRook2',],
                    ['bPawn1','bPawn2','bPawn3','bPawn4',
                     'bPawn5','bPawn6','bPawn7','bPawn8'],
                    [None,None,None,None,None,None,None,None],
                    [None,None,None,None,None,None,None,None],
                    [None,None,None,None,None,None,None,None],
                    [None,None,None,None,None,None,None,None],
                    ['wPawn1','wPawn2','wPawn3','wPawn4',
                     'wPawn5','wPawn6','wPawn7','wPawn8'],
                    ['wRook1','wKnight1','wBishop1','wQueen1',
                    'wKing','wBishop2','wKnight2','wRook2',]]
        isGameOver=False
        scene.userzoom=True
        scene.userspin=True
        wQueenCount=1
        bQueenCount=1
        isWTurn=True
        isPieceSelected=False
        highlightedCol=None
        highlightedRow=None
        selectedRow=None
        selectedCol=None
        nextEnPassantCol=None
        enPassant=False
        isBlackChecked=isWhiteChecked=False
        rows=cols=8
        redrawAll()


input = sys.stdin.read()
