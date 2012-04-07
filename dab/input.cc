#include <istream>


using namespace std;
read_input(void)
	int player_num;

	cin >> player_num;

	int board[11][11];

	for (int r = 0; r < 11; r++) {
		for (int c = 0; c < 11; c++) {
			cin >> board[r][c];
		}
	}
}
