#include <algorithm>
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
    board_->pull_up_edge(r_, c_);
}

Move Move::invalid(void)
{
    return Move(NULL, 0, -1, -1);
}

/*
 * Board
 */
Board::Board()
{}

int Board::set_owner(int player, int r, int c)
{
    int old = raw_[r][c];
    raw_[r][c] = player;
    return old;
}

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
    bool r_valid = r >= 0 && r < bh_;
    bool c_valid = c >= 0 && c < bw_;
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

bool Board::has_edge(int r, int c)
{
    if (!rc_is_edge(r, c)) {
        std::cerr << "invalid edge" << std::endl;
        return false;
    }

    if (raw_[r][c] == 1)
        return true;
    return false;
}

bool Board::has_edge(Edge &e)
{
    return has_edge(e.r, e.c);
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

void Box::set_owner(int player)
{
    b.set_owner(r, c, player);
}

bool Box::is_closed()
{
    std::vector<Edge> edge;
    edge.reserve(4);
    std::insert_iterator<std::vector<Edge> > edge_it = std::inserter(edge, edge.end());

    int ct = 0;
    for (int i = 0; i < 4; i++) {
        ct += b.has_edge(edge[i]);
    }

    return ct == 4;
}

int Board::pull_up_edge(int r, int c)
{
    if (!rc_is_edge(r,c)) {
        std::cerr << "invalid edge" << std::endl;
        return -1;
    }

    /* get the max(2) boxes adjacent to the edge */
    std::vector<Box> boxes;
    std::insert_iterator<std::vector<Box> > boxes_it = std::inserter(boxes, boxes.end());
    get_adjacent_boxes_to_edge(r, c, boxes_it);

    int pts = 0;
    /* count the number of boxes closed by this added edge. */
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i].is_closed()) {
            pts ++;
            boxes[i].set_owner(0);
        }
    }

    /* update the raw board */
    raw_[r][c] = 0;

    /* FIXME */
    return pts;
}

int Board::make_move(int player, int r, int c)
{

    if (player != 1 && player != 2) {
        std::cerr << "invalid player" << std::endl;
        return -1;
    }

    if (!rc_is_edge(r,c)) {
        std::cerr << "invalid edge " << r << ", " << c << std::endl;
        return -1;
    }

    /* update the raw board */
    raw_[r][c] = 1;

    /* get the max(2) boxes adjacent to the edge */
    std::vector<Box> boxes;
    std::insert_iterator<std::vector<Box> > boxes_it = std::inserter(boxes, boxes.end());
    get_adjacent_boxes_to_edge(r, c, boxes_it);

    int pts = 0;
    /* count the number of boxes closed by this added edge. */
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i].is_closed()) {
            pts ++;
            boxes[i].set_owner(player);
        }
    }

    return pts;
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
    board_.print_raw();
}

Board &Game::get_board(void)
{
    return board_;
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

    if (moves.empty()) {
        return std::make_pair(Move::invalid(), 0);
    }
    // We've bottomed out in the search, so use the evaluation function.
    else if (depth <= 0) {
        int const value = eval(board, player);
        return std::make_pair(Move::invalid(), value);
    }
    // Recursive minimax search.
    else {
        std::vector<Move> optimal_moves;
        int optimal_score = 0;

        for (size_t i = 0; i < moves.size(); i++) {
            Move &move = moves[i];

            int score_move = move.apply();
            int next_player;
            int score;

            // If we didn't score a point, then proceed as usual (i.e. as a a
            // minimax min node).
            if (score_move == 0) {
                next_player = player ^ 3;
                score = score_move - search(board, next_player, depth - 1).second;
            }
            // If we scored a point, it's still our turn...so keep going.
            else {
                next_player = player;
                score = score_move + search(board, next_player, depth - 1).second;
            }
            move.unapply();

            // Keep track of a set of equally optimal values.
            if (score == optimal_score) {
                optimal_moves.push_back(move);
            } else if (score > optimal_score) {
                optimal_score = score;
                optimal_moves.clear();
                optimal_moves.push_back(move);
            }
        }

        // Randomly select one of the equally optimal moves.
        int const i = rand() % optimal_moves.size();
        return std::make_pair(optimal_moves[i], optimal_score);
    }
}

Move play(Board &board, int player, int depth)
{
    return search(board, player, depth).first;
}
