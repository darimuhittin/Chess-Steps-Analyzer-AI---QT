#include "analyzernode.h"

AnalyzerNode::AnalyzerNode(ChessBoard board, int depth, int index,
                           int parentIndex, Move moveToMake, int current_eval, int board_value):
    m_board(board),m_depth(depth),m_index(index),
    m_parentIndex(parentIndex),m_moveToMake(moveToMake),m_current_eval(current_eval),m_board_value(board_value)
{

}

void AnalyzerNode::operator =(const AnalyzerNode &other)
{
    m_board = other.m_board;
    m_depth = other.m_depth;
    m_index = other.m_index;
    m_parentIndex = other.m_parentIndex;
    m_moveToMake = other.m_moveToMake;
    m_current_eval = other.m_current_eval;
    m_board_value = other.m_board_value;
}
