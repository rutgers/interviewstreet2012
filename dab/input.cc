#include <algorithm>
#include <limits>
#include <cstdlib>
#include <vector>
#include "input.hh"
#include <sys/time.h>

/* Subtract the `struct timeval' values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0.  */

int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

class DeltaTime {
    private:
        struct timeval tv_;

    public:
        DeltaTime() {
            gettimeofday(&tv_, NULL);
        }

        long get_elapsed() {
            struct timeval tv, diff;
            gettimeofday(&tv, NULL);

            timeval_subtract(&diff, &tv, &tv_);

            if (diff.tv_usec)
                return diff.tv_sec * 1000 + diff.tv_usec / 1000;
            else
                return diff.tv_sec * 1000;
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

    for (int r = 0; r < bh_; r++) {
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
    int nb(0), sb(0), eb(0), wb(0);

    if (c > 1 && r > 0) /* move can close left box */ 
        wb = raw_[r][c-2] + raw_[r+1][c+1] + raw_[r+1][c-1];
    if (c < bw_ && r > 0) /* move can close right box */
        eb = raw_[r][c+2] + raw_[r+1][c+1] + raw_[r+1][c-1];
    if (r > 1 && c < bw_) /* move can close upper box */
        nb = raw_[r+2][c] + raw_[r+1][c+1] + raw_[r+1][c-1];
    if (r < (bh_-1) && c < bw_) /* move can close lower box */
        sb = raw_[r-2][c] + raw_[r-1][c+1] + raw_[r-1][c-1];

    return ((nb > 3 || sb > 3 || wb > 3 || eb > 3)? true : false);
}

#if 0
static int eval(Board currBoard, int player)
{
    // FIXME: Use a real evaluation function...
    /* Find largest chain */
    std::vector<Move> moves;
    std::insert_iterator<std::vector<Move> > currmoves = std::inserter(moves, moves.end());
    /* Find moves */
    currBoard.get_valid_moves(player, currmoves);
    //std::vector<Move>::iterator it;
    std::vector<Move> movesUsable;
    movesUsable.reserve(moves.size());
    /* Remove if this move doesn't close a box */
    for (int i = 0, len = moves.size(); i < len; ++i)
    {
        if (currBoard.closes_box(moves[i])) {
            movesUsable.push_back(moves[i]);
        }
    }
    
    /*for (it = moves.begin(); it < moves.end(); it++)
    {
        if (!currBoard.closes_box(*it)) {
            moves.erase(it);
        }
    }*/
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
    /*for (it = moves.begin(); it < moves.end(); it++)
    {
        currBoard.make_move(player, it->r(), it->c());
        tmp = eval(currBoard, player);
        if (tmp > max)
            max = tmp;
        currBoard.pull_up_edge(it->r(), it->c());
    }*/
    
    return max + 1;  /* At least one move closes a box */
}
#endif

static int eval(Board &move, int player)
{
    std::vector<Edge> edge;
    edge.reserve(4);

    int b3 = 0;

    for (int c = 0; c < move.bw_; c++) {
        for (int r = 0; r < move.bh_; r++) {
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
    return b3;
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
                score = score_move - search(board, us, next_player, depth - 1, alpha, beta).second;
            }
            // If we scored a point, it's still our turn...so keep going.
            else {
                next_player = player;
                score = score_move + search(board, us, next_player, depth - 1, alpha, beta).second;
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

            // Alpha-beta pruning
            if (player == us) {
                alpha = std::max(alpha, optimal_score);
            } else {
                beta = std::min(beta, optimal_score);
            }

            if (beta <= alpha) {
                break;
            }
        }

        // Randomly select one of the equally optimal moves.
        if (optimal_moves.empty()) {
            return std::make_pair(Move::invalid(), optimal_score);
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

    for (int depth = 0; ; depth++) {
        //std::cerr << "depth = " << depth << std::endl;

        // Time how long a depth of search d takes.
        long const before_ms = timer.get_elapsed();
        Move move = search(board, player, player, depth, alpha, beta).first;
        long const after_ms = timer.get_elapsed();

        // Estimate how long a search of depth (d + 1) will take using the
        // branching factor.
        long const curr_ms = after_ms - before_ms;
        long const next_ms = curr_ms * (branching_factor - depth);
        if (timer.get_elapsed() + next_ms >= timeout_ms) {
            return move;
        }
    }
}
