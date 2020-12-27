#include "chessboard.h"

void ChessBoard::operator =(const ChessBoard &other)
{
    m_boardData = other.m_boardData;
    m_ranks = other.m_ranks;
    m_columns = other.m_columns;
}

ChessBoard::ChessBoard(const ChessBoard &board)
{
    setRanks(board.ranks());
    setColumns(board.columns());
    m_boardData = board.boardData();
}

ChessBoard::ChessBoard(int r,int c,QObject *parent) : QObject(parent)
{
    setRanks(r);
    setColumns(c);
    initBoard();

}

void ChessBoard::setData(int column, int rank, char value)
{
    if(setDataInternal(column,rank,value))
        emit dataChanged(column,rank);
}

void ChessBoard::movePiece(int fromColumn, int fromRank, int toColumn, int toRank)
{
    setData(toColumn,toRank,data(fromColumn,fromRank));
    setData(fromColumn,fromRank,' ');
}

void ChessBoard::setFen(const QString &fen)
{
    int index = 0;
    int skip = 0;
    const int columnCount = columns();
    QChar ch;
    for(int rank = ranks();rank>0;--rank){
        for (int column = 1;column <= columnCount ;++column ) {
            if (skip > 0) {
                ch = ' ';
                skip --;
            }else {
                ch = fen.at(index++);
                if (ch.isDigit()) {
                    skip = ch.toLatin1()-'0';
                    ch = ' ';
                    skip --;
                }
            }
            setDataInternal(column,rank,ch.toLatin1());
        }
        QChar next = fen.at(index++);
        if(next!= '/' && next != ' '){
            initBoard();
            return;
        }
    }
    emit boardReset();
}


void ChessBoard::setRanks(int newRanks)
{
    if(ranks() == newRanks) return;
    m_ranks = newRanks;
    initBoard();
    emit ranksChanged(m_ranks);
}

void ChessBoard::setColumns(int newColumns)
{
    if(columns() == newColumns) return;
    m_columns = newColumns;
    initBoard();
    emit columnsChanged(m_columns);
}

bool ChessBoard::setDataInternal(int column, int rank, char value)
{
    int index = (rank -1)*columns() + (column-1);
    if(m_boardData.at(index) == value) return false;
    m_boardData[index] = value;
    return true;
}

void ChessBoard::initBoard()
{
    m_boardData.fill(' ',ranks()*columns());
    emit boardReset();
}

