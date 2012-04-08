#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "input.hh"

int main(void)
{
    srand(time(NULL));

    Game forReals;
    forReals.read_input();

    Board &board = forReals.get_board();
    Move move = play(board, forReals.get_player(), 2000);
    std::cout << move.r() << ' ' << move.c() << std::endl;
    return 0;
}
