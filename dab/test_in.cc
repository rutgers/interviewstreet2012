#include <iostream>
#include <vector>
#include "input.hh"


int main(void)
{
	Board b;
	b.read_input();
	b.print_raw();

    std::vector<Move> moves;
    std::insert_iterator<std::vector<Move> > moves_it = std::inserter(moves, moves.end());
    b.get_valid_moves(1, moves_it);

#if 0
    for (int i = 0; i < moves.size(); i++) {
        std::cout << '(' << moves[i].r << ',' << moves[i].c << ')' << std::endl;
    }
#endif
    return 0;
}
