#ifndef INPUT_HH_
#define INPUT_HH_

#include <iostream>
#include <iterator>

class Board;

class Move {
public:
    Move(Board *board, int player, int r, int c);
    int r(void) const;
    int c(void) const;
    void apply(void);
    void undo(void);

private:
    Board *board_;
    int player_;
    int r_, c_;
};

class Board {
public:
	static const int bw = 11;
	static const int bh = 11;
	int raw[bh][bw];

    Board(void);
    void make_move(int player, int r, int c);

    void read_input(void);
    void print(void);

    template <typename T>
    void get_valid_moves(int player, std::insert_iterator<T> &moves)
    {
        for (int r = 0; r < bw; r += 2)
        for (int c = 0; c < bw; c += 2) {
            if (raw[r][c] == 0) {
                *moves = Move(this, player, r, c);
            }
        }
    }
};

class Game {
public:
    Board board;
	int player_num;

	Game(void);
	bool is_init(void);
    void read_input(void);
    void print(void);

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        board.get_valid_moves(player_num, moves);
    }

};

#endif

/* vim: set sts=4 et sw=4 ts=4 : */
