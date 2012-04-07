#ifndef INPUT_HH_
#define INPUT_HH_

#include <iostream>

class Move {
public:
    Move(int mr, int mc)
        : r(mr), c(mc)
    {}

    int const r, c;
};

class Board {
public:
	static const int bw = 11;

	int raw[bw][bw];
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

		for (int r = 0; r < bw; r++) {
			for (int c = 0; c < bw; c++) {
				std::cin >> raw[r][c];
			}
		}
	}

	void print(void)
	{
		std::cout << player_num << std::endl;

		for (int r = 0; r < bw; r++) {
			for (int c = 0; c < bw; c++) {
				std::cout << raw[r][c] << ' ';
			}
			std::cout << std::endl;
		}
	}

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        for (int r = 0; r < bw; r += 2)
        for (int c = 0; c < bw; c += 2) {
            if (raw[r][c] == 0) {
                *moves = Move(r, c);
            }
        }
    }
};

#endif
