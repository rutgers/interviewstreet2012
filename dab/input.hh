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

class Edge {
public:
    Board *b;
    int r, c;

    Edge(Board *b_, int r_, int c_)
    : b(b_)
    , r(r_)
    , c(c_)
    {}
};

class Box {
public:
    Board *b;
    int r, c;

    Box();
    Box(Board *b_, int r_, int c_)
    : b(b_)
    , r(r_)
    , c(c_)
    {}

    bool is_closed();
    void set_owner(int player);
};

class Board {
public:
    Board(void);
    Board(const Board &xboard)
    {
        for (int r = 0; r < bh_; ++r)
        for (int c = 0; c < bw_; ++c)
            raw_[r][c] = xboard.raw_[r][c];
    }

    /* returns positive points GAINED or negative on error */
    int make_move(int player, int r, int c);
    /* returns positive points LOST   or negative on error */
    int pull_up_edge(int r, int c);
    /* returns previous owner */
    int set_owner(int player, int r, int c);

    void print(void);
    void print_raw(void);
    void read_input(void);

    bool rc_is_valid(int r, int c);
    bool rc_is_edge(int r, int c);
    bool rc_is_box(int r, int c);

    bool has_edge(int y, int x);
    bool has_edge(Edge &e);

    template <typename T>
    void get_edges_in_box(int r, int c, std::insert_iterator<T> &edges)
    {
        *edges = Edge(this, r + 1, c);
        *edges = Edge(this, r - 1, c);
        *edges = Edge(this, r,     c + 1);
        *edges = Edge(this, r,     c - 1);
    }

    template <typename T>
    void get_edges_in_box(Box &b, std::insert_iterator<T> &edges)
    {
        get_edges_in_box(b.r, b.c, edges);
    }

    template <typename T>
    void get_adjacent_boxes_to_edge(int r, int c, std::insert_iterator<T> &boxes);

    template <typename T>
    void get_valid_moves(int player, std::insert_iterator<T> &moves)
    {
        // if the row is odd, look at even columns and vice versa
        for (int r = 0; r < bh_; r ++)
        for (int c = 0; c < bw_; c ++) {
            bool c_odd = c % 2;
            bool r_odd = r % 2;

            if (r_odd ^ c_odd) {
                if (raw_[r][c] == 0) {
                    *moves = Move(this, player, r, c);
                }
            }
        }
    }
    
    bool closes_box(int, int);

    static int const bw_ = 11;
    static int const bh_ = 11;
private:
    int raw_[bh_][bw_];
};

class Game {
public:
	Game(void);
	bool is_init(void);
    void read_input(void);
    void print(void);

    Board &get_board(void);
    int    get_player(void) {
        return player_;
    }

    template <typename T>
    void get_valid_moves(std::insert_iterator<T> &moves)
    {
        board_.get_valid_moves(player_, moves);
    }

private:
    Board board_;
	int player_;
};

Move play(Board &board, int player, long timeout_ms);

template <typename T>
void Board::get_adjacent_boxes_to_edge(int r, int c, std::insert_iterator<T> &boxes)
{
    bool r_odd = r % 2;
    //bool edge_is_lr = r_odd;  /* left to right */
    bool edge_is_hl = !r_odd; /* high to low */

    if (edge_is_hl) {
        int r1 = r - 1;
        int r2 = r + 1;

        if (rc_is_box(r1, c))
            *boxes = Box(this, r1, c);

        if (rc_is_box(r2, c))
            *boxes = Box(this, r2, c);
    } else {
        int c1 = c - 1;
        int c2 = c + 1;

        if (rc_is_box(r, c1))
            *boxes = Box(this, r, c1);
        if (rc_is_box(r, c2))
            *boxes = Box(this, r, c2);
    }
}

#endif

/* vim: set sts=4 et sw=4 ts=4 : */
