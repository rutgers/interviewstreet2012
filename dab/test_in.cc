#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "input.hh"

int main(void)
{
    srand(time(NULL));

	Board b;
	b.read_input();
	b.print();

    Move move = play(b, 1);
    std::cout << move.r() << ',' << move.c() << std::endl;
    return 0;
}
