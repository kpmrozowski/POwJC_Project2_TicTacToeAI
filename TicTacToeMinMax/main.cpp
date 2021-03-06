#include <iostream>
#include <vector>
#include <array>
#include <random>

struct move {
	int r, c, score;
};

struct TicTacToe {

	TicTacToe() {
		grid = std::vector<std::vector<int>>(3, std::vector<int>(3, -1));
	}
	~TicTacToe() {}

	int player{}, computer{};
	std::vector<std::vector<int>> grid;

	void makeRandomMove(int who) {
		std::mt19937 mt(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, 2);
		grid[dist(mt)][dist(mt)] = who;
	}

	bool win() {

		std::vector<std::vector<int>> win_states = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

			for(auto i : win_states) {
				int first_r = i[0] / 3;
				int first_c = i[0] % 3;

			bool win = true; // Let's assume that there's win, and if there is a contradiction (dowod nie w prost), change it to flalse

				for(auto j : i) {
				int r = j / 3;
				int c = j % 3;

				//	If the first element in a configuration is empty or if elements in a configuration are not the same, than there's no win:
				if(grid[first_r][first_c] == -1 || grid[first_r][first_c] != grid[r][c]) {
					win = false;
				}
			}
			if(win)
				return true;
		}
		return false;
	}

	bool tie() {
		if(win())
			return false;

			for (auto i : grid) {
				for (auto j :  i) {
					if (j == -1) {
						return false;
					}
				}
			}

		return true;
	}

	void player_move() {
		while(true) {
			std::cout << "Enter an empty cell (1 - 9): ";
			int cell;
			try {
				std::cin >> cell;
				// cin.fail() --> fail if type is not correct
				if(std::cin.fail() || (static_cast<int>(cell) != cell)) {
					std::cout << "\nWrong input. Please enter one integer\n";
					std::cin.clear();
					std::cin.ignore(256, '\n'); // ignore the line change
				}
			} catch(std::exception& e) {
				std::cout << "\nYou entered: " << e.what() << "\nAnd you were ment to enter a number from range 1 to 9\n";
			}
			int r = (cell - 1) / 3;
			int c = (cell - 1) % 3;
			if(cell >= 1 && cell <= 9 && grid[r][c] == -1) {
				grid[r][c] = player;
				break;
			} else {
				player_move();
			}
		}
	}

	void computer_move() {
		move best_move = minimax();
		grid[best_move.r][best_move.c] = computer;
	}

	// First we check if the player is maximizing_player;
	//	If you are the maximizing_player then we will try to maximize your score, but if y're not then we want to harm you as much as posible:
	move minimax(bool maximizing_player = true) {
		move best_move;
		if(win()) {
			// If maximizing_player wins we can return 1
			//		and if maximizing_player loses we
			//		returns -1;
			if(maximizing_player) {
				best_move.score = -1;
			} else {
				best_move.score = 1;
			}
			return best_move;
		} else if(tie()) {
			best_move.score = 0;
			return best_move;
		}

		best_move.score = maximizing_player ? -2 : 2;
		// -2 and 2 acts just like as -inf and inf

		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				if(grid[i][j] == -1) {
					//	Ai tries to maximize it's score and player is tring to minimize AI's score
					grid[i][j] = maximizing_player ? computer : player;
					//print();	uncomment to see AI's analizes
					move board_state = minimax(!maximizing_player);
					if(maximizing_player) {
						//	if the move is better than the move we'd already found or if we haven't found it yet:
						if(board_state.score > best_move.score) {
							// best move is the found one:
							best_move.score = board_state.score;
							best_move.r = i;
							best_move.c = j;
						}
					} else {
						// if we search for the worst move for player:
						if(board_state.score < best_move.score) {
							best_move.score = board_state.score;
							best_move.r = i;
							best_move.c = j;
						}
					}
					grid[i][j] = -1; // reset analised pole
				}
			}
		}
		return best_move;
	}

	void play() {
		char imput{};
		int turnNr{0};

		while(true) {
			std::cout << "Which symbol (X or O, X goes first)? ";
			std::cin >> imput;
			if(imput == 'X' || imput == 'O' || imput == 'x' || imput == 'o') {
				break;
			}
		}

		if(imput == 'x' || imput == 'X')
			player = 8;
		if(imput == 'o' || imput == 'O')
			player = 0;
		computer = player == 8 ? 0 : 8;

		if(player == 0) {
			std::cout << "Computer is making a move...\n";
			makeRandomMove(computer);
			turnNr++;
		}

		print();

		// main game loop:
		while(true) {
			player_move();
			turnNr++;
			print();

			if(win()) {
				std::cout << "Player wins!\n";
				return;
			} else if(tie()) {
				std::cout << "Tie!\n";
				return;
			}

			std::cout << "Computer is making a move...\n";
			if(turnNr > 1) {
				computer_move();
			} else {
				// Find player's first move:
				move playerFirstMove{};
				for(int i = 0; i < 3; i++)
					for(int j = 0; j < 3; j++)
						if(grid[i][j] == player) {
							playerFirstMove.r = i;
							playerFirstMove.c = j;
						}

				//	In the AI's first turn: place "O" on the oposite side of "X"
				std::mt19937 mt(std::random_device{}());
				std::uniform_int_distribution<int> dist(0, 1);
				switch(playerFirstMove.r * 3 + playerFirstMove.c) {
					case 1:
						grid[2][1] = computer;
						break;
					case 3:
						grid[1][2] = computer;
						break;
					case 4:
						grid[2 * dist(mt)][2 * dist(mt)] = computer;
						break;
					case 5:
						grid[1][0] = computer;
						break;
					case 7:
						grid[0][1] = computer;
						break;
					default:
						grid[1][1] = computer;
						break;
				};
			}
			
			print();

			if(win()) {
				std::cout << "Computer wins!\n";
				return;
			} else if(tie()) {
				std::cout << "Tie!\n";
				return;
			}
		}
	}

	void print() {
		std::cout << '\n';

		for(int i = 0; i < 3; i++) {
			if(i) {
				std::cout << "-----------\n";
			}

			for(int j = 0; j < 3; j++) {
				if(j)
					std::cout << "|";
				std::cout << ' ';

				if(grid[i][j] == -1) {
					std::cout << 3 * i + j + 1;
				} else if(grid[i][j] == 0) {
					std::cout << 'O';
				} else {
					std::cout << 'X';				
				}
				std::cout << ' ';
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
};

int main() {
	TicTacToe game;
	game.play();
}