#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "input.hh"

int main(void)
{
    time_t s = time(NULL);
    std::cout << s << std::endl;
    srand(s);

    Game forReals;
    forReals.read_input();

    Board &board = forReals.get_board();
    Move move = play(board, forReals.get_player(), 3000);

    board.print();
    std::cout << move.r() << ' ' << move.c() << std::endl;
    move.apply();

    board.print();
    return 0;
}
