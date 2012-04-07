#ifndef INPUT_HH_
#define INPUT_HH_

#include <istream>

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
}

#endif
