#include <algorithm>
#include <limits>
#include <cstdlib>
#include <vector>
#include "input.hh"
#include <time.h>

class DeltaTime {
    private:

    public:
        DeltaTime()
        {
        }

        /* milliseconds */
        clock_t get_elapsed() {
            clock_t c = clock() / (CLOCKS_PER_SEC / 1000);
            return c;
        }
};

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
    if (player != 0 && player != 1 && player != 2) {
        std::cerr << "Bad player = " << player << std::endl;
    }
    raw_[r][c] = player;
    return old;
}

void Board::print(void)
{
    std::ostream &o = std::cerr;


    o << "-\t";
    for (int c = 0; c < bw_; c++)
        o << c;
    o << std::endl;

    for (int r = 0; r < bh_; r++) {
        o << r << '\t';

        for (int c = 0; c < bw_; c++) {
            bool col_odd = c%2;
            bool row_odd = r%2;

            if (!col_odd && !row_odd) {
                o << '.';
            } else if (col_odd && !row_odd) {
                if (raw_[r][c] == 1)
                    o << '-';
                else
                    o << ' ';
            } else if (col_odd && row_odd) {
                if (raw_[r][c] != 0) {
                    if (raw_[r][c] == 1)
                        o << 'A';
                    else if (raw_[r][c] == 2)
                        o << 'B';
                    else
                        o << '?';
                }
                else
                    o << ' ';
            } else if (!col_odd && row_odd) {
                if (raw_[r][c] == 1)
                    o << '|';
                else
                    o <<' ';
            } else {
                o << '?';
            }
        }
        o << std::endl;
    }
}

void Board::print_raw(void)
{
    std::ostream &o = std::cerr;
    for (int r = 0; r < bh_; r++) {
        for (int c = 0; c < bw_; c++) {
            o << raw_[r][c] << ' ';
        }
        o << std::endl;
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
        std::cerr << "invalid edge c " << r << ", " << c << std::endl;
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

void Box::set_owner(int player)
{
    b->set_owner(player, r, c);
}

bool Box::is_closed()
{
    std::vector<Edge> edge;
    edge.reserve(4);
    std::insert_iterator<std::vector<Edge> > edge_it = std::inserter(edge, edge.end());
    b->get_edges_in_box(*this, edge_it);

    int ct = 0;
    for (int i = 0; i < 4; i++) {
        ct += b->has_edge(edge[i]);
    }

    return ct == 4;
}

int Board::pull_up_edge(int r, int c)
{
    if (!rc_is_edge(r,c)) {
        std::cerr << "invalid edge b " << r << ", " << c << std::endl;
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
        std::cerr << "invalid edge a " << r << ", " << c << std::endl;
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
    std::cerr << player_ << std::endl;
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

bool Board::closes_box(Move move)
{
    int r = move.r();
    int c = move.c();
    int rOdd = r & 1;
    int cOdd = c & 1;
    /* Vertical Slice */
    if (rOdd && !cOdd)
    {
        if (c == 0)
        {
            if (raw_[r][c+2] && raw_[r-1][c+1] && raw_[r+1][c+1])
                return true;
        }
        else if (c == bw_)
        {
            if (raw_[r][c-2] && raw_[r-1][c-1] && raw_[r+1][c-1])
                return true;
        }
        else
        {
            if (raw_[r][c+2] && raw_[r-1][c+1] && raw_[r+1][c+1])
                return true;
            if (raw_[r][c-2] && raw_[r-1][c-1] && raw_[r+1][c-1])
                return true;
        }
    }
    /* Horizontal Slice */
    else if (!rOdd && cOdd)
    {
        if (r == 0)
        {
            if (raw_[r+2][c] && raw_[r+1][c-1] && raw_[r+1][c+1])
                return true;
        }
        else if (r == bh_)
        {
            if (raw_[r-2][c] && raw_[r-1][c-1] && raw_[r-1][c+1])
                return true;
        }
        else
        {
            if (raw_[r+2][c] && raw_[r+1][c-1] && raw_[r+1][c+1])
                return true;
            if (raw_[r-2][c] && raw_[r-1][c-1] && raw_[r-1][c+1])
                return true;
        }
    }
    return false;
}

#if 0
static int eval(Board currBoard, int player)
{
    /* Find largest chain */
    std::vector<Move> moves;
    std::insert_iterator<std::vector<Move> > currmoves = std::inserter(moves, moves.end());
    /* Find moves */
    currBoard.get_valid_moves(player, currmoves);
    
    std::vector<Move> movesUsable;
    movesUsable.reserve(moves.size());
    
    /* Remove if this move doesn't close a box */
    for (int i = 0, len = moves.size(); i < len; ++i)
    {
        if (currBoard.closes_box(moves[i])) {
            movesUsable.push_back(moves[i]);
        }
    }
    
    if (movesUsable.size() == 0)
        return 0;
    /* only moves that close boxes are left */
    int max = 0, tmp;
    for (int i = 0, len = movesUsable.size(); i < len; ++i) {
        currBoard.make_move(player, movesUsable[i].r(), movesUsable[i].c());
        tmp = eval(currBoard, player);
        if (tmp > max)
            max = tmp;
        currBoard.pull_up_edge(movesUsable[i].r(), movesUsable[i].c());
    }
    
    return max + 1;  /* At least one move closes a box */
}
#endif

static int eval(Board &move, int player)
{
    std::vector<Edge> edge;
    edge.reserve(4);

    int b3 = 0;

    for (int c = 1; c < move.bw_; c+= 2) {
        for (int r = 1; r < move.bh_; r+= 2) {
            std::insert_iterator<std::vector<Edge> > edge_it = std::inserter(edge, edge.end());
            move.get_edges_in_box(r, c, edge_it);

            int ct = 0;
            for (int i = 0; i < edge.size(); i++) {
                if (move.has_edge(edge[i]))
                    ct ++;
            }

            if (ct == 3)
                b3++;

            edge.clear();
        }
    }
    return -b3;
}

static std::pair<Move, int> search(Board &board, int us, int player, int depth, int alpha, int beta)
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
        int optimal_score = std::numeric_limits<int>::min();

        for (size_t i = 0; i < moves.size(); i++) {
            Move &move = moves[i];

            int score_move = move.apply();
            int score;

#ifdef DEBUG
            std::cerr << std::endl << "player " << player << " at depth " << depth
                      << " [ move = " << move.r() << ", " << move.c() << " ] "
                      << " [ move_score = " << score_move << " ]" << std::endl;
            board.print();
            std::cerr << std::endl << std::endl;
#endif

            // If we didn't score a point, then proceed as usual (i.e. as a a
            // minimax min node).
#ifdef DEBUG
            std::cerr << ">>>RECURSIVE START" << std::endl;
#endif
            if (score_move == 0) {
                int next_player = player ^ 3;
                score = score_move - search(board, us, next_player, depth - 1, alpha, beta).second;
            }
            // If we scored a point, it's still our turn...so keep going.
            else {
                score = score_move + search(board, us, player, depth - 1, alpha, beta).second;
            }
#ifdef DEBUG
            std::cerr << "<<<RECURSIVE END [ move_score = " << score << " ]" << std::endl;
#endif

            move.unapply();

            // Keep track of a set of equally optimal values.
            if (score == optimal_score) {
                optimal_moves.push_back(move);
            } else if (score > optimal_score) {
                optimal_score = score;
                optimal_moves.clear();
                optimal_moves.push_back(move);
            }

#if 0
            // Alpha-beta pruning
            if (player == us) {
                alpha = std::max(alpha, optimal_score);
                if (beta <= alpha) {
                    return std::make_pair(move, alpha);
                }
            } else {
                beta = std::min(beta, optimal_score);
                if (beta <= alpha) {
                    return std::make_pair(move, beta);
                }
            }
#endif
        }

        // Randomly select one of the equally optimal moves.
        if (optimal_moves.empty()) {
            if (moves.empty()) {
                return std::make_pair(Move::invalid(), optimal_score);
            }
            // This should never happen. Just in case...
            else {
                std::cerr << "WTF?" << std::endl;
                return std::make_pair(moves[0], 0);
            }
        } else {
            int const i = rand() % optimal_moves.size();
            return std::make_pair(optimal_moves[i], optimal_score);
        }
    }
}

Move play(Board &board, int player, long timeout_ms)
{
    DeltaTime timer;

    int const alpha = std::numeric_limits<int>::min();
    int const beta  = std::numeric_limits<int>::max();
    int const branching_factor = 36;

#ifdef DEBUG
    std::cerr << "STARTING BOARD" << std::endl;
    board.print();
    std::cerr << std::endl << std::endl;
#endif

    for (int depth = 0; ; depth++) {
#ifdef DEBUG
        std::cerr << "!!! IDS depth = " << depth << std::endl;
#endif

        // Time how long a depth of search d takes.
        clock_t const before_ms = timer.get_elapsed();
        Move move = search(board, player, player, depth, alpha, beta).first;
        clock_t const after_ms = timer.get_elapsed();

        // Estimate how long a search of depth (d + 1) will take using the
        // branching factor.
        clock_t const curr_ms = after_ms - before_ms;
        clock_t const next_ms = curr_ms * (branching_factor - depth);
        if (timer.get_elapsed() + next_ms >= timeout_ms) {
            return move;
        }
    }
}
