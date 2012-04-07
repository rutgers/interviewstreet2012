#include <algorithm>
#include <cstdlib>
#include <vector>
#include "input.hh"

/*
 * Move
 */
Move::Move(void)
    : board_(NULL),
      player_(0)
{
}

Move::Move(Board *board, int player, int r, int c)
    : board_(board),
      player_(player),
      r_(r),
      c_(c)
{}

int Move::r(void) const
{
    return r_;
}

int Move::c(void) const
{
    return c_;
}

bool Move::is_invalid(void) const
{
    return board_ == NULL;
}

int Move::apply(void)
{
    return board_->make_move(player_, r_, c_);
}

void Move::unapply(void)
{
    board_->make_move(r_, c_, 0);
}

Move Move::invalid(void)
{
    return Move();
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

int Board::make_move(int player, int r, int c) {
    raw_[r][c] = player;
    // FIXME: Return the number of points that this move scored.
    return 0;
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
static int eval(Board move, int player)
{
    // FIXME: Use a real evaluation function...
    return 0;
}

static std::pair<Move, int> search(Board &board, int player, int depth)
{
    std::vector<Move> moves;
    std::insert_iterator<std::vector<Move> > moves_it = std::inserter(moves, moves.end());
    board.get_valid_moves(player, moves_it);

    // Nothing to see here...
    if (moves.empty()) {
        return std::make_pair(Move::invalid(), 0);
    }
    // We've bottomed out in the search. Use the evaluation functions instead
    // of a recursive search.
    else if (depth <= 0) {
        int const value = eval(board, player);
        return std::make_pair(Move::invalid(), value);
    }
    // Recursively deepen the search. 
    else {
        std::vector<Move> optimal_moves;
        int optimal_score = 0;

        for (size_t i = 0; i < moves.size(); i++) {
            Move &move = moves[i];

            int score_move = move.apply();
            int score_subtree = search(board, player, depth - 1).second;
            int score = score_move + score_subtree;
            move.unapply();

            if (score == optimal_score) {
                optimal_moves.push_back(move);
            } else if (score > optimal_score) {
                optimal_score = score;
                optimal_moves.clear();
                optimal_moves.push_back(move);
            }
        }

        // Randomly select one of the "equally optimal" moves.
        int const i = rand() % optimal_moves.size();
        return std::make_pair(optimal_moves[i], optimal_score);
    }
}

Move play(Board &board, int player, int depth)
{
    return search(board, player, depth).first;
}
