import chess.pgn
import io
import re
import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

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
        chess.KING: 1,
    }
    
    # Initialize feature variables
    turn = 1 if board.turn == chess.WHITE else -1
    pieces_available = 0
    material = 0
    data = []

    # Count pieces on the board
    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece:
            piece_value = piece_values.get(piece.piece_type, 0)  # Get piece value
            if piece.color == chess.WHITE:
                data.append(piece_value)  # Positive value for white
                material += piece_value
            else:
                data.append(-piece_value)  # Negative value for black
                material -= piece_value

            # Count pieces available
            pieces_available += 1  # Increment count for the piece type
        else:
            data.append(0)  # No piece on this square

    # Add additional features
    data.append(turn)  # Whose turn it is
    data.append(pieces_available)  # Total pieces available on the board
    data.append(material)

    return data

def training_data(pgn_string):
    pgn_io = io.StringIO(pgn_string)
    first_game = chess.pgn.read_game(pgn_io)
    data = []

    for node in first_game.mainline():
        if is_quiet_position(node.board()):
            # Extract feature vector
            input_data = board_to_features(node.board())

            comment = node.comment

            eval_match = re.search(r'\[%eval (-?\d+\.\d+)\]', comment)

            target = 0
            
            # If an evaluation is found, print it
            if eval_match:
                target = float(eval_match.group(1))
            
            data.append((input_data, target))

    return data

# Load PGN data
pgn_df = pd.read_csv('data/games.csv')

data = []
# Extract training data from the first 5 games for demonstration purposes
for i in range(1000):
    game_moves = pgn_df['Moves'].iloc[i]
    data_ = training_data(game_moves)
    data.extend(data_)

# Prepare features (X) and target values (y) for regression
X = [item[0] for item in data]  # Feature vectors
y = [item[1] for item in data]  # Target evaluation scores

# Convert lists to numpy arrays
X = np.array(X)
y = np.array(y)

# Split the data into training and testing sets (80% training, 20% testing)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Create a linear regression model
model = LinearRegression()

# Train the model
model.fit(X_train, y_train)

# Make predictions
y_pred = model.predict(X_test)

# Evaluate the model
mse = mean_squared_error(y_test, y_pred)
print(f'Mean Squared Error: {mse}')

# Display model coefficients
print(f'Coefficients: {model.coef_}')
print(f'Intercept: {model.intercept_}')

modelDataFile = "models/modelData.txt"
# Write coefficients and intercept to a text file
with open(modelDataFile, 'w') as f:
    f.write(' '.join(map(str, model.coef_)))
    f.write(' ')
    f.write(str(model.intercept_))