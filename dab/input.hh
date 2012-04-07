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
	int player_num;

	Board()
	: player_num(0)
	{}

	bool is_init()
	{
		return player_num != 0;
	}

	void read_input(void)
	{
		std::cin >> player_num;

		for (int r = 0; r < bh; r++) {
			for (int c = 0; c < bw; c++) {
				std::cin >> raw[r][c];
			}
		}
	}

	void print(void)
	{
		std::cout << player_num << std::endl;

		for (int r = 0; r < bh; r++) {
			for (int c = 0; c < bw; c++) {
				std::cout << raw[r][c] << ' ';
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

#endif
