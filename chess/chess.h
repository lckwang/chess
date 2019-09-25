#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <memory>

typedef char file_t;
typedef short rank_t;

enum piece_e {
    empty = 0,
    king,
    queen,
    rook,
    knight,
    bishop,
    pawn
};
struct Position {
    file_t f;
    rank_t r;
};
using Positions=std::vector<Position>;
class Board;

class Piece {
public: // Constructor and APIs
    Piece(file_t f, rank_t r, char c = 'w') : mFile{ f }, mRank{ r }, mIsWhite{ (c == 'w' || c == 'W') ? true : false } {};
    //virtual bool isValidPieceMove(file_t f, rank_t r) const;  // Is a valid move for the piece without considering other pieces on the board?
    virtual Positions allValidPieceMove(Board& b) const { return Positions{}; };  // Return all valid moves for the piece.
    void setPos(Position p) { mFile = p.f; mRank = p.r; };// Set a piece to a new postion without validity check.
    friend std::ostream& operator<<(std::ostream& ost, const Piece& p) {
        ost << ((p.mIsWhite==true)?"w":"b")+p.toString() + p.mFile + char(p.mRank + '0');
        return ost;
    }
    inline bool isInBoard(file_t f, rank_t r) const;  // Return True if the position is within the board.7
public: // Properties
    file_t mFile;
    rank_t mRank;
    bool mIsWhite;       // true if the piece is white; false otherwise.
    piece_e mPiece;
protected: 
    virtual std::string toString() const { return mPieceName.at(mPiece); };
private:
    const static std::unordered_map<piece_e, std::string> mPieceName;
};

const std::unordered_map<piece_e, std::string> Piece::mPieceName = { {
    { empty, "e"},
    { king, "k" },
    { queen, "q" },
    { rook, "r" },
    { knight, "n" },
    { bishop, "b" },
    { pawn, "p" }
} };
class King : public Piece {
public:
    // Constructor '*p' has to be in the form "{b|w}{file}{rank}" where file or rank must be one charactor.
    King(const char* p) : Piece(p[1], p[2] - '0', p[0]) { mPiece = king; };
    //bool isValidPieceMove(file_t f, rank_t r) const;
    Positions allValidPieceMove(Board& b) const override;  // Return all valid moves for the piece without considering other pieces on the board.
protected:
    std::string toString() const { return "K"; };
};

class Queen : public Piece {
public:
    // Constructor '*p' has to be in the form "{b|w}{file}{rank}" where file or rank must be one charactor.
    Queen(const char* p) : Piece(p[1], p[2] - '0', p[0]) { mPiece = queen; };
    //bool isValidPieceMove(file_t f, rank_t r) const;
    Positions allValidPieceMove(Board& b) const override;  // Return all valid moves for the piece without considering other pieces on the board.
protected:
    std::string toString() const { return "Q"; };
};

// Controller class of chess game
class Board {
public:
    bool isValidMove(Piece p, file_t f, rank_t r, char c) const;  // Is the move valid at game level?
    template <typename T>
    bool addAPiece(T&);  // Add a piece position to the game. Return true if basic check passes.
    std::optional<Piece> getPieceAt(file_t f, rank_t r) const;
    void showPieces() const {
        std::for_each(mPieces.begin(), mPieces.end(), [](const std::unique_ptr<Piece>& p) {
            std::cout << *p << " "; });
        std::cout << std::endl;
    }
    // Move a piece from old position to new position. No valididty checks.
    void moveAPiece(const Position& old, const Position& newP);
private:
    std::vector<std::unique_ptr<Piece>> mPieces;
};