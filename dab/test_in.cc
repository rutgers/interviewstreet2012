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
    forReals.print();

    Board &board = forReals.get_board();
    Move move = play(board, 1, 3);
    move.apply();
    board.print();
    std::cout << move.r() << ',' << move.c() << std::endl;
    return 0;
}
