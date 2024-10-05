import chess.pgn
import io
import numpy as np  # Regular expressions for extracting evaluation scores
import pandas as pd

def is_quiet_position(board):
    for move in board.legal_moves:
        if board.is_capture(move):
            return False
    return True

def board_to_features(board):
    # Define piece values
    piece_values = {
        chess.PAWN: 100,
        chess.KNIGHT: 320,
        chess.BISHOP: 330,
        chess.ROOK: 500,
        chess.QUEEN: 900,
        chess.KING: 20000,
    }
    
    # Initialize feature variables
    turn = 1 if board.turn == chess.WHITE else -1
    pieces_available = 0

    data = []

    # Count pieces on the board
    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece:
            piece_value = piece_values.get(piece.piece_type, 0)  # Get piece value
            if piece.color == chess.WHITE:
                data.append(piece_value)  # Positive value for white
            else:
                data.append(-piece_value)   # Negative value for black

            # Count pieces available
            pieces_available += 1  # Increment count for the piece type
        else:
            data.append(0)

    data.append(turn)
    data.append(pieces_available)

    return data

def training_data(pgn_string):
    pgn_io = io.StringIO(pgn_string)

    first_game = chess.pgn.read_game(pgn_io)

    data = []

    for node in first_game.mainline():
        if (is_quiet_position(node.board())): 
            input = board_to_features(node.board())
            target = node.eval()
            data.append([input, target])

    return data

pgn_df = pd.read_csv('data/games.csv')

for i in range(5):
    game_moves = pgn_df['Moves'].iloc[i]

    print(training_data(game_moves))
