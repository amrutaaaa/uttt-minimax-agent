#include "board.cpp"

void showBoards(UTTBoard & board);
uint8_t getUserMove(void);
void makeMove(UTTBoard & board);
uint8_t getBestMove(UTTBoard & board, int depth);

int main(int argc, char *argv[]) {
  UTTBoard board = UTTBoard();

  showBoards(board);
  while (!board.gameOver()) {
    cout << "Player " << board.whoseTurn() + 1 << "'s Turn: " << ((board.whoseTurn() == 0) ? 'X' : 'O') << '\n';
    makeMove(board);
    showBoards(board);
  }

  cout << "Winner: Player " << board.getWinner() + 1 << "!\n";
}

void makeMove(UTTBoard & board) {
  uint8_t position;
  if (board.whoseTurn()==0){
    try{
      position = getBestMove(board, 8);
    } catch (...){
      position = board.getLegalMoves()[0];
    }
    
    cout<<"AI CHOSE THE MOVE "<< board.getMoveString(position)<<endl;
  }else{
    position = getUserMove();
  }

  // Reprompt until valid input
  while((position >= 81) || !board.checkMove(position)) {
    cout << "Invalid input or move, please enter a 0-indexed number with 2 digits to represent the board and cell number, or 4 digits for the row and col of the board and cell\n";
    position = getUserMove();
  }

  board.makeMove(position);
}

// Takes in user input from stdin and returns the corresponding absolute board position
// If invalid, returns 81 (outside legal range)
// Valid formats (0120: Sub-board row 0 col 1, Cell row 2 col 0)
// Valid formats (64: Sub-board 6, Cell 4)
uint8_t getUserMove() {
  uint8_t maxDigit = 0;
  uint8_t digits[4];
  string input;

  // Validate Input
  getline(cin, input);
  for (int i = 0; i < 4 && i < (int)input.length(); i++) {
    if ((input[i] < '0') || (input[i] > '8')) return 81;
    digits[i] = input[i] - '0';
    if (digits[i] > maxDigit) maxDigit = digits[i];
  }

  // Translate string to 
  if ((input.length() == 4) && (maxDigit < 4)) { // Row/Col format
    return 9*(3*digits[0] + digits[1]) + (3*digits[2] + digits[3]);
  } else if ((input.length() == 2) && (maxDigit < 9)) { // Board/Cell Number
    return 9*digits[0] + digits[1];
  } else { // Invalid format
    return 81;
  }
}

void showBoards(UTTBoard & board) {
  cout << "Board:\n";
  board.print();
  cout << "\nWon Sub-boards:\n";
  board.printTop();
  cout << '\n';
}

int minimax(UTTBoard &board, int depth, int alpha, int beta, bool isMaximizing, uint8_t aiPlayer){

  if (depth==0||board.gameOver()){
    return board.getHeuristicScore(aiPlayer);
  }

  vector<uint8_t> moves = board.getLegalMoves();
  if (moves.empty()) return board.getHeuristicScore(aiPlayer);

  if (isMaximizing){
    int maxEval = -9999999;

    for (uint8_t move: moves){
      UTTBoard nextBoard = board;
      nextBoard.makeMove(move);

      int eval = minimax(nextBoard, depth-1, alpha, beta, false, aiPlayer);

      if (eval>maxEval) maxEval=eval;

      alpha = max(alpha, eval);

      if (beta<=alpha) break;
    }
    return maxEval;
  } else{
    int minEval = 9999999;

    for (uint8_t move: moves){
      UTTBoard nextBoard = board;
      nextBoard.makeMove(move);

      int eval = minimax(nextBoard, depth-1, alpha, beta, true, aiPlayer);

      if (eval<minEval)minEval=eval;

      beta = min(beta, eval);

      if (beta<=alpha) break;
    }
    return minEval;
  }
};

uint8_t getBestMove(UTTBoard &board, int depth){
  uint8_t aiPlayer = board.whoseTurn();
  vector<uint8_t> moves = board.getLegalMoves();

  uint8_t bestMove = 81;
  int bestValue = -9999999;

  int alpha = -2000000000;
  int beta = 2000000000;

  for (uint8_t move: moves){
    UTTBoard nextBoard = board;
    nextBoard.makeMove(move);

    int moveValue = minimax(nextBoard, depth-1, alpha, beta, false, aiPlayer);

    // cout << "Move " << (int)move << " (Board " << (int)(move/9) << " Cell " << (int)(move%9) << ") Score: " << moveValue << endl;

    if (moveValue>bestValue){
      bestValue = moveValue;
      bestMove = move;
    }
  }
  return bestMove;
};
