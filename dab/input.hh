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
	int raw[11][11];
	int player_num;

	void read_input(void)
	{
		std::cin >> player_num;

		for (int r = 0; r < 11; r++) {
			for (int c = 0; c < 11; c++) {
				cin >> raw[r][c];
			}
		}
	}

	void print(void)
	{
		
	}

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        for (int r = 0; r < bh; r += 2)
        for (int c = 0; c < bw; c += 2) {
            if (raw[r][c] == 0) {
                *moves = Move(r, c);
            }
        }
    }
};

#endif
