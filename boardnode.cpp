#include "boardnode.h"


BoardNode::BoardNode(QPointer<BoardNode> parent, QPointer<ChessBoard> board,int value,QString moveToMakeThis, char turn)
{
    m_parent = parent;
    m_board = board;
    m_moveToMakeThis = moveToMakeThis;
    m_turn = turn;
    m_value = value;
}

void BoardNode::addChild(QPointer<BoardNode> child)
{
    m_children.append(child);
}

void BoardNode::setMoveToMakeThis(QString move)
{
    m_moveToMakeThis = move;
}
