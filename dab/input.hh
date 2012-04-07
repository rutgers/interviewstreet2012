#ifndef INPUT_HH_
#define INPUT_HH_

#include <iostream>
#include <iterator>

class Board;

class Move {
public:
    Move(Board *board, int player, int r, int c)
        : board_(board),
          player_(player),
          r_(r),
          c_(c)
    {}

    int r(void) const {
        return r_;
    }

    int c(void) const {
        return c_;
    }

    void apply(void)
    {
        board_->make_move(player_, r_, c_);
    }

    void undo(void)
    {
        board_->make_move(r_, c_, 0);
    }

private:
    Board *board_;
    int player_;
    int r_, c_;
};

class Board {
public:
	static const int bw = 11;
	static const int bh = 11;
	int raw[bh][bw];

    Board()
    {}

    void read_input(void)
	{
		bool input_funky = false;

		for (int r = 0; r < bh; r++) {
			for (int c = 0; c < bw; c++) {
				std::cin >> raw[r][c];
				if (std::cin.fail()) {
					std::cin.clear();
					if (!input_funky) {
						std::cerr << "Input file invalid (probably missing last row). Assuming '0'."
					              << "Your only warning" << std::endl;
						input_funky = true;
					}
					raw[r][c] = 0;
				}
			}
		}
	}

    void print(void)
	{
		for (int r = 0; r < bh; r++) {
			for (int c = 0; c < bw; c++) {
				bool col_odd = c%2;
				bool row_odd = r%2;

				if (!col_odd && !row_odd) {
					std::cout << '.';
				} else if (col_odd && !row_odd) {
					if (raw[r][c] == 1)
						std::cout << '-';
					else
						std::cout << ' ';
				} else if (col_odd && row_odd) {
					if (raw[r][c] != 0)
						std::cout << raw[r][c];
					else
						std::cout << ' ';
				} else if (!col_odd && row_odd) {
					if (raw[r][c] == 1)
						std::cout << '|';
					else
						std::cout <<' ';
				} else {
					std::cout << '?';
				}
			}
			std::cout << std::endl;
		}
	}

    void make_move(int player, int r, int c) {
        raw[r][c] = player;
    }

    template <typename T>
    void get_valid_moves(int player, std::insert_iterator<T> &moves)
    {
        for (int r = 0; r < bw; r += 2)
        for (int c = 0; c < bw; c += 2) {
            if (raw[r][c] == 0) {
                *moves = Move(this, player, r, c);
            }
        }
    }
};

class Game {
public:
    Board board;
	int player_num;

	Game()
	: player_num(0)
	{}

	bool is_init()
	{
		return player_num != 0;
	}

    void read_input(void)
	{
		std::cin >> player_num;
        board.read_input();
	}


    void print(void)
    {
		std::cout << player_num << std::endl;
        board.print();
    }

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        board.get_valid_moves(moves);
    }

};

#endif

/* vim: set sts=4 et sw=4 ts=4 : */
