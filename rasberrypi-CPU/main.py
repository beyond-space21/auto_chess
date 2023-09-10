from blocks.pn532_chess_poss import pn532 as piece_poss

piece_poss.process.start()

while True:
    a=input()
    print(piece_poss.buff)