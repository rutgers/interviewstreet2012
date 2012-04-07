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
    Board(void);
    void make_move(int player, int r, int c);

    void read_input(void);
    void print(void);

    template <typename T>
    void get_valid_moves(int player, std::insert_iterator<T> &moves)
    {
        for (int r = 0; r < bw_; r += 2)
        for (int c = 0; c < bw_; c += 2) {
            if (raw_[r][c] == 0) {
                *moves = Move(this, player, r, c);
            }
        }
    }

private:
    static int const bw_ = 11;
    static int const bh_ = 11;
    int raw_[bh_][bw_];
};

class Game {
public:
	Game(void);
	bool is_init(void);
    void read_input(void);
    void print(void);

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        board_.get_valid_moves(player_, moves);
    }

private:
    Board board_;
	int player_;
};

#endif

/* vim: set sts=4 et sw=4 ts=4 : */
