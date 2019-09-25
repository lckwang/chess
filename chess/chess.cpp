// chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include "chess.h"

bool Piece::isInBoard(file_t f, rank_t r) const {
    return (f>='a' && f<='h' && r>=1 && r<=8 ) ? true : false;
}

Positions King::allValidPieceMove(Board& b) const {
    Positions ps{};
    for (short i=-1; i<=1; ++i)
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            file_t f = mFile + i;
            rank_t r = mRank + j;
            if (isInBoard(f, r)) {
                auto opt = b.getPieceAt(f, r);
                if (opt.has_value()) {
                    if ((*opt).mIsWhite == mIsWhite)
                        continue;
                }
                ps.push_back(Position{ f, r });
            }
        }
    return ps;
}

// The following macro is used only by Queen::allValidPieceMove() to check if a new position is valid.
// If the position is valid add it valid position list. Note break statement in the macro.
#define check_n_add { \
        if (isInBoard(f, r)) { \
            auto opt = b.getPieceAt(f, r); \
            if (opt.has_value()) { \
                if ((*opt).mIsWhite != mIsWhite) { \
                    ps.push_back(Position{ f, r }); \
                } \
                break; \
            } \
            ps.push_back(Position{ f, r }); \
        } \
}
Positions Queen::allValidPieceMove(Board& b) const {
    Positions ps{};
    short i;
    // Find positions on file up and down direction
    i = 1;
    while ((mFile + i) <= 'h') {
        file_t f = mFile + i;
        rank_t r = mRank;
        i++;
        check_n_add;
    }
    i = 1;
    while ((mFile - i) >= 'a') {
        file_t f = mFile - i;
        rank_t r = mRank;
        i++;
        check_n_add;
    }
    // Find positions on rank up and down direction
    i = 1;
    while ((mRank + i) <= 8) {
        file_t f = mFile;
        rank_t r = mRank+i;
        i++;
        check_n_add;
    }
    i = 1;
    while ((mRank - i) >= 1) {
        file_t f = mFile;
        rank_t r = mRank - i;
        i++;
        check_n_add;
    }
    // Find positions on upper-left and down-right direction
    i = 1;
    while ((mRank + i) <= 8 && (mFile -i) >= 'a') {
        file_t f = mFile - i;
        rank_t r = mRank + i;
        i++;
        check_n_add;
    }
    i = 1;
    while ((mRank - i) >= 1 && (mFile + i) <= 'h') {
        file_t f = mFile + i;
        rank_t r = mRank - i;
        i++;
        check_n_add;
    }
    // Find positions on lower-left and upper-right direction
    i = 1;
    while ((mRank - i) >= 1 && (mFile - i) >= 'a') {
        file_t f = mFile - i;
        rank_t r = mRank - i;
        i++;
        check_n_add;
    }
    i = 1;
    while ((mRank + i) <= 8 && (mFile + i) <= 'h') {
        file_t f = mFile + i;
        rank_t r = mRank + i;
        i++;
        check_n_add;
    }
    return ps;
}

std::optional<Piece> Board::getPieceAt(file_t f, rank_t r) const {
    for (auto& p : mPieces) {
        if ((*p).mFile == f && (*p).mRank == r)
            return std::make_optional(*p);
    }
    return std::optional<Piece>{};
}

bool Board::addAPiece(Piece& p) {
    if (!p.isInBoard(p.mFile, p.mRank)) return false;
    auto opt = getPieceAt(p.mFile, p.mRank);    // opt is an optional value
    if (opt.has_value()) return false;
    mPieces.push_back(std::make_unique<Piece>(p));
    return true;
}
void Board::moveAPiece(const Position& old, const Position& newP) {
    for (auto& p : mPieces) {
        if (((*p).mFile == old.f) && ((*p).mRank == old.r))
            (*p).setPos(newP);
    }
}

int main()
{
    using namespace std;

    King bk("bd4");
    Queen wq("wf7");
    King wk{ "wh3" };

    cout << bk << endl;
    cout << wq << endl;
    cout << wk << endl;


    Board board{};
    bool ret;
    ret = board.addAPiece(bk);    assert(ret);
    ret = board.addAPiece(wq);    assert(ret);
    ret = board.addAPiece(wk);    assert(ret);
    ret = board.addAPiece(wq);    assert(!ret);
    board.showPieces();

#if 1
    auto moves = wq.allValidPieceMove(board);
    cout << "Check moves by white queen " << wq << " are:\n";
    for_each(moves.begin(), moves.end(), [&wq, &board](Position& p) {
        //cout << p.f << p.r << " ";
        Position old{ wq.mFile, wq.mRank };
        board.moveAPiece(old, p);
        wq.setPos(p);
        auto fm = wq.allValidPieceMove(board);
        for (auto& np : fm) {
            auto opt = board.getPieceAt(np.f, np.r);
            if (opt.has_value()) {
                if ((*opt).mPiece == king)
                    cout << wq << " \n";
            }
        }
        });
    cout << endl;

    cout << "Done checking " << wq << endl;
    board.showPieces();
#endif
}
