#include <cstdlib>
#include <vector>
#include "input.hh"

/*
 * Move
 */
Move::Move(Board *board, int player, int r, int c)
    : board_(board),
      player_(player),
      r_(r),
      c_(c)
{}

int Move::r(void) const {
    return r_;
}

int Move::c(void) const {
    return c_;
}

void Move::apply(void)
{
    board_->make_move(player_, r_, c_);
}

void Move::undo(void)
{
    board_->make_move(r_, c_, 0);
}

/*
 * Board
 */
Board::Board()
{}

void Board::print(void)
{
    for (int r = 0; r < bh_; r++) {
        for (int c = 0; c < bw_; c++) {
            bool col_odd = c%2;
            bool row_odd = r%2;

            if (!col_odd && !row_odd) {
                std::cout << '.';
            } else if (col_odd && !row_odd) {
                if (raw_[r][c] == 1)
                    std::cout << '-';
                else
                    std::cout << ' ';
            } else if (col_odd && row_odd) {
                if (raw_[r][c] != 0) {
                    if (raw_[r][c] == 1)
                        std::cout << 'A';
                    if (raw_[r][c] == 2)
                        std::cout << 'B';
                }
                else
                    std::cout << ' ';
            } else if (!col_odd && row_odd) {
                if (raw_[r][c] == 1)
                    std::cout << '|';
                else
                    std::cout <<' ';
            } else {
                std::cout << '?';
            }
        }
        std::cout << std::endl;
    }
}

void Board::print_raw(void)
{
    for (int r = 0; r < bh_; r++) {
        for (int c = 0; c < bw_; c++) {
            std::cout << raw_[r][c] << ' ';
        }
        std::cout << std::endl;
    }
}

void Board::read_input(void)
{
    for (int r = 0; r < bh_; r++) {
        for (int c = 0; c < bw_; c++) {
            std::cin >> raw_[r][c];
            if (std::cin.fail()) {
                std::cin.clear();
                std::cerr << "input invalid @ (" << r << ',' << c << "). Assuming '0'." << std::endl;
                raw_[r][c] = 0;
            }
        }
    }
}

#if 0
class Edge {
    public:
        int x_, y_;
        int r_, c_;

        Board &board_;

        Edge(board, x, y)
        : board_(board)
        , x_(x)
        , y_(y)
        {}

};
#endif

bool Board::rc_is_valid(int r, int c)
{
    bool r_valid = r > 0 && r < bh_;
    bool c_valid = c > 0 && c < bw_;
    return r_valid && c_valid;
}

bool Board::rc_is_edge(int r, int c)
{
    bool valid = rc_is_valid(r, c);
    bool c_odd = c % 2;
    bool r_odd = r % 2;

    return valid && (c_odd ^ r_odd);
}

bool Board::rc_is_box(int r, int c)
{
    bool valid = rc_is_valid(r, c);
    bool c_odd = c % 2;
    bool r_odd = r % 2;

    return valid && c_odd && r_odd;
}

bool Board::has_edge(int y, int x)
{
    int r = y * 2;
    int c = x * 2;

    if (!rc_is_edge(r, c)) {
        return false;
    }

    if (raw_[r][c] == 1)
        return true;
    return false;
}

#if 0
int Board::add_edge(int y, int x)
{
    int r = y * 2;
    int c = x * 2;

    if (!rc_is_edge(r, c)) {
        return -1;
    }
}
#endif


void Board::make_move(int player, int r, int c) {

    if (!rc_is_edge(r,c)) {
        std::cerr << "invalid edge" << std::endl;
        return;
    }

    /* get list of Boxes closed by adding the edge. */

    /* get the max(2) boxes adjacent to the edge */
    std::vector<Box> moves;
    std::insert_iterator<std::vector<Box> > moves_it = std::inserter(moves, moves.end());

    raw_[r][c] = player;
}

/*
 * Game
 */
Game::Game()
    : player_(0)
{}

bool Game::is_init(void)
{
    return player_ != 0;
}

void Game::read_input(void)
{
    std::cin >> player_;
    board_.read_input();
}

void Game::print(void)
{
    std::cout << player_ << std::endl;
    board_.print();
}

/*
 * Play
 */
Move play(Board &board, int player)
{
    std::vector<Move> moves;
    std::insert_iterator<std::vector<Move> > moves_it = std::inserter(moves, moves.end());
    board.get_valid_moves(player, moves_it);

    int i = rand() % moves.size();
    return moves[i];
}
