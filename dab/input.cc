#include <input.hh>

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

void Board::read_input(void)
{
    bool input_funky = false;

    for (int r = 0; r < bh; r++) {
        for (int c = 0; c < bw; c++) {
            std::cin >> raw[r][c];
            if (std::cin.fail()) {
                std::cin.clear();
                if (!input_funky) {
                    std::cerr << "Input file invalid (probably missing last row). Assuming '0'."
                              << "Your only warning" << std::endl;
                    input_funky = true;
                }
                raw[r][c] = 0;
            }
        }
    }
}

void Board::print(void)
{
    for (int r = 0; r < bh; r++) {
        for (int c = 0; c < bw; c++) {
            bool col_odd = c%2;
            bool row_odd = r%2;

            if (!col_odd && !row_odd) {
                std::cout << '.';
            } else if (col_odd && !row_odd) {
                if (raw[r][c] == 1)
                    std::cout << '-';
                else
                    std::cout << ' ';
            } else if (col_odd && row_odd) {
                if (raw[r][c] != 0)
                    std::cout << raw[r][c];
                else
                    std::cout << ' ';
            } else if (!col_odd && row_odd) {
                if (raw[r][c] == 1)
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

void Board::make_move(int player, int r, int c) {
    raw[r][c] = player;
}

/*
 * Game
 */
Game::Game()
    : player_num(0)
{}

bool Game::is_init()
{
    return player_num != 0;
}

void Game::read_input(void)
{
    std::cin >> player_num;
    board.read_input();
}

void Game::print(void)
{
    std::cout << player_num << std::endl;
    board.print();
}
