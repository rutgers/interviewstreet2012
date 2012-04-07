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
    /*Board b;
    b.read_input();
    b.print_raw();
    b.print();

    Move move = play(b, 1);
    std::cout << move.r() << ',' << move.c() << std::endl;*/
    return 0;
}
