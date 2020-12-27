#ifndef ANALYZERNODE_H
#define ANALYZERNODE_H

#include "chessboard.h"


struct Move{
    int fromCol;
    int fromRank;
    int toCol;
    int toRank;

    bool operator ==(Move const &other){
        if(fromCol == other.fromCol && fromRank == other.fromRank
                && toCol == other.toCol && toRank == other.toRank)
            return true;
        else
            return false;
    }
};

class AnalyzerNode
{
public:
    AnalyzerNode(ChessBoard board,int depth,int index,int parentIndex,Move moveToMake,int current_eval,int board_value);
    void operator =(AnalyzerNode const &other);

    ChessBoard board() const{return m_board;}
    int depth() const{return m_depth;}
    int index() const{return m_index;}
    int parentIndex() const{return m_parentIndex;}
    Move moveToMake() const{return m_moveToMake;}
    int current_eval() const{return m_current_eval;}
    int board_value() const{return m_board_value;}
private:
    ChessBoard m_board;
    int m_depth;
    int m_index;
    int m_parentIndex;
    Move m_moveToMake;
    int m_current_eval;
    int m_board_value;
};

#endif // ANALYZERNODE_H
