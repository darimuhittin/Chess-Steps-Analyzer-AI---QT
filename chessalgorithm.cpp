#include "chessalgorithm.h"

#include <QPoint>

ChessAlgorithm::ChessAlgorithm(QObject *parent) : QObject(parent)
{
    m_currentPlayer = NoPlayer;
    m_result = NoResult;
}

ChessBoard *ChessAlgorithm::board() const
{
    return m_board;
}

void ChessAlgorithm::newGame()
{
    setupBoard();
    //board()->setFen("r1pkp1/6/6/6/6/2PKPR");
}

bool ChessAlgorithm::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
    Q_UNUSED(colFrom)
    Q_UNUSED(rankFrom)
    Q_UNUSED(colTo)
    Q_UNUSED(rankTo)
    return false;
}

bool ChessAlgorithm::move(const QPoint &from, const QPoint &to)
{
    return move(from.x(), from.y(), to.x(), to.y());
}

void ChessAlgorithm::setupBoard()
{
    setBoard(new ChessBoard(8,8,this));
}

void ChessAlgorithm::setBoard(ChessBoard *board)
{
    if(board == m_board) return;
    if(m_board) delete m_board;
    m_board = board;
    emit boardChanged(m_board);

}

void ChessAlgorithm::setResult(ChessAlgorithm::Result value)
{
    if (result() == value) return;
    if(result() == NoResult){
        m_result = value;
        emit gameOver(m_result);
    }else{
        m_result = value;
    }

}

void ChessAlgorithm::setCurrentPlayer(ChessAlgorithm::Player value)
{
    if(currentPlayer() == value) return;
    m_currentPlayer = value;
    emit currentPlayerChanged(m_currentPlayer);
}
