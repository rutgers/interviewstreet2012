#ifndef INPUT_HH_
#define INPUT_HH_

#include <iostream>


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
};

#endif
