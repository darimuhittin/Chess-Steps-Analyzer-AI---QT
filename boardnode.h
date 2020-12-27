#ifndef BOARDNODE_H
#define BOARDNODE_H

#include "chessboard.h"

#include <QPointer>



class BoardNode
{
public:
    BoardNode(QPointer<BoardNode> parent,QPointer<ChessBoard> board,int value,QString moveToMakeThis,char turn);
    void addChild(QPointer<BoardNode> child);
    void setMoveToMakeThis(QString move);
    QPointer<ChessBoard> board(){return m_board;}
private:
    QPointer<BoardNode> m_parent;
    QList<QPointer<BoardNode>> m_children;
    QPointer<ChessBoard> m_board;
    int m_value;
    QString m_moveToMakeThis;
    char m_turn;
};

#endif // BOARDNODE_H
