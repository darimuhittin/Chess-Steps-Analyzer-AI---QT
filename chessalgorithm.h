#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "chessboard.h"

#include <QObject>
#include <QPointer>

class ChessAlgorithm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Result result READ result)
    Q_PROPERTY(Player currentPlayer
               READ currentPlayer
               NOTIFY currentPlayerChanged)
public:
    enum Result { NoResult, Player1Wins, Draw, Player2Wins };
    enum Player { NoPlayer, Player1, Player2 };
    Q_ENUM(Result)
    Q_ENUM(Player)
    enum Color { White, Black};

    explicit ChessAlgorithm(QObject *parent = nullptr);
    ChessBoard* board() const;

    inline Result result() const {return m_result;}
    inline Player currentPlayer() const { return m_currentPlayer; }

signals:
    void boardChanged(ChessBoard*);
    void gameOver(Result);
    void currentPlayerChanged(Player);

public slots:
    virtual void newGame();
    virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);
    bool move(const QPoint &from,const QPoint &to);

protected:
    virtual void setupBoard();
    void setBoard(ChessBoard *board);
    void setResult(Result);
    void setCurrentPlayer(Player);
private:
    QPointer<ChessBoard> m_board;
    Result m_result;
    Player m_currentPlayer;
};

#endif // CHESSALGORITHM_H
