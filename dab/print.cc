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
    board.print();
    return 0;
}
