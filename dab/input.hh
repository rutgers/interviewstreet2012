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
    bool is_invalid(void) const;

    int apply(void);
    void unapply(void);

    static Move invalid(void);

private:
    Board *board_;
    int player_;
    int r_, c_;
};

class Box {
public:
    Board &b_;
    int r_, c_;

    Box(Board &b, int r, int c)
    : b_(b)
    , r_(r)
    , c_(c)
    {}
};

class Board {
public:
    Board(void);
    int make_move(int player, int r, int c);
    void print(void);
    void print_raw(void);
    void read_input(void);

    bool rc_is_valid(int r, int c);
    bool rc_is_edge(int r, int c);
    bool rc_is_box(int r, int c);

    bool has_edge(int y, int x);

    template <typename T>
    void get_adjacent_boxes_to_edge(int r, int c, std::insert_iterator<T> &boxes)
    {
        bool r_odd = r % 2;
        //bool edge_is_lr = r_odd;  /* left to right */
        bool edge_is_hl = !r_odd; /* high to low */

        if (edge_is_hl) {
            int r1 = r - 1;
            int r2 = r + 1;

            if (rc_is_box(r1, c))
                *boxes = Box(*this, r1, c);

            if (rc_is_box(r2, c))
                *boxes = Box(*this, r2, c);
        } else {
            int c1 = c - 1;
            int c2 = c + 1;

            if (rc_is_box(r, c1))
                *boxes = Box(*this, r, c1);
            if (rc_is_box(r, c2))
                *boxes = Box(*this, r, c2);
        }
    }

    template <typename T>
    void get_valid_moves(int player, std::insert_iterator<T> &moves)
    {
        // if the row is odd, look at even columns and vice versa
        for (int r = 0; r < bh_; r += 2)
        for (int c = ((r&1)==0); c < bw_; c += 2) {
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

    Board &get_board(void);

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        board_.get_valid_moves(player_, moves);
    }

private:
    Board board_;
	int player_;
};

Move play(Board &board, int player, int depth);

#endif

/* vim: set sts=4 et sw=4 ts=4 : */
