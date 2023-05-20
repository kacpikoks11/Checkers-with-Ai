#include "Minimax.h"


int static_evaluation(int board[8][8]) {
    int sum = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            sum += board[i][j];
        }
    return sum;
}

Eval minimax(int board[8][8], int depth, bool maximizing_player, bool was_a_capture, sf::Vector2i coords) {
    if (depth == 0)
        return static_evaluation(board);
    if (!look_for_moves(board, maximizing_player > 0 ? 1 : -1))
        return -100 * maximizing_player > 0 ? 1 : -1;
    if (maximizing_player) {
        Eval maxEval(-1000000);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j] > 0)
                    for (sf::Vector2i move : legal_moves(board, 1, { i,j }, board[i][j], was_a_capture || look_for_capture(board, 1))) {
                        if (coords.x != i && coords.y != -1 && was_a_capture)
                            continue;
                        int board_cp[8][8];
                        for (int z = 0; z < 8; ++z)
                            std::copy(std::begin(board[z]), std::end(board[z]), std::begin(board_cp[z]));
                        Eval eval;
                        if (make_a_move(board_cp, 1, { i,j }, move, board_cp[i][j]).size() != 0) {
                            eval = minimax(board_cp, depth - 1, true, true, move);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = true;
                        }
                        else {
                            eval = minimax(board_cp, depth - 1, false, false, move);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = false;
                        }
                        maxEval = maxEval.eval > eval.eval ? maxEval : eval;
                    }
            }
        return maxEval;
    }
    else {
        Eval minEval(1000000);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j] < 0)
                    for (sf::Vector2i move : legal_moves(board,-1, { i,j }, board[i][j], was_a_capture || look_for_capture(board, -1))) {
                        if (coords.x != i && coords.y != -1 && was_a_capture)
                            continue;
                        int board_cp[8][8];
                        for (int z = 0; z < 8; ++z)
                            std::copy(std::begin(board[z]), std::end(board[z]), std::begin(board_cp[z]));
                        Eval eval;
                        if (make_a_move(board_cp,-1, { i,j }, move, board_cp[i][j]).size() != 0) {
                            eval = minimax(board_cp, depth - 1, false, true, move);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = true;
                        }
                        else {
                            eval = minimax(board_cp, depth - 1, true, false, move);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = false;
                        }
                        minEval = minEval.eval < eval.eval ? minEval : eval;
                    }
            }
        return minEval;
    }
}


Eval minimax_ab(int board[8][8], int depth, bool maximizing_player, bool was_a_capture, sf::Vector2i coords, int alpha, int beta) {
    if (depth == 0)
        return static_evaluation(board);
    if (!look_for_moves(board, maximizing_player > 0 ? 1 : -1))
        return -100 * maximizing_player > 0 ? 1 : -1;
    if (maximizing_player) {
        Eval maxEval(-1000000);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j] > 0)
                    for (sf::Vector2i move : legal_moves(board, 1, { i,j }, board[i][j], was_a_capture || look_for_capture(board, 1))) {
                        if (coords.x != i && coords.y != -1 && was_a_capture)
                            continue;
                        int board_cp[8][8];
                        for (int z = 0; z < 8; ++z)
                            std::copy(std::begin(board[z]), std::end(board[z]), std::begin(board_cp[z]));
                        Eval eval;
                        if (make_a_move(board_cp, 1, { i,j }, move, board_cp[i][j]).size() != 0) {
                            eval = minimax_ab(board_cp, depth - 1, true, true, move, alpha, beta);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = true;
                        }
                        else {
                            eval = minimax_ab(board_cp, depth - 1, false, false, move, alpha, beta);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = false;
                        }
                        maxEval = maxEval.eval > eval.eval ? maxEval : eval;
                        alpha = alpha > maxEval.eval ? alpha : maxEval.eval;
                        if (beta <= alpha)
                            goto exit_max;
                    }
            }
        exit_max:
        return maxEval;
    }
    else {
        Eval minEval(1000000);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j] < 0)
                    for (sf::Vector2i move : legal_moves(board, -1, { i,j }, board[i][j], was_a_capture || look_for_capture(board, -1))) {
                        if (coords.x != i && coords.y != -1 && was_a_capture)
                            continue;
                        int board_cp[8][8];
                        for (int z = 0; z < 8; ++z)
                            std::copy(std::begin(board[z]), std::end(board[z]), std::begin(board_cp[z]));
                        Eval eval;
                        if (make_a_move(board_cp, -1, { i,j }, move, board_cp[i][j]).size() != 0) {
                            eval = minimax_ab(board_cp, depth - 1, false, true, move, alpha, beta);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = true;
                        }
                        else {
                            eval = minimax_ab(board_cp, depth - 1, true, false, move, alpha, beta);
                            eval.from = { i,j };
                            eval.to = move;
                            eval.capture = false;
                        }
                        minEval = minEval.eval < eval.eval ? minEval : eval;
                        beta = beta < minEval.eval ? beta : minEval.eval;
                        if (beta <= alpha)
                            goto exit_min;
                    }
            }
        exit_min:
        return minEval;
    }
}