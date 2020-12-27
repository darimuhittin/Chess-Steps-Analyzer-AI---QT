#ifndef DERSALGORITHM_H
#define DERSALGORITHM_H

#include "analyzernode.h"
#include "boardnode.h"
#include "chessalgorithm.h"
#include <QObject>

class DersAlgorithm : public ChessAlgorithm
{
    Q_OBJECT
public:
    struct Ret{
        Move move;
        int eval_value;
    };
    DersAlgorithm(QObject *parent);

    // ChessAlgorithm interface
public slots:
    void newGame();
    bool move(int colFrom, int rankFrom, int colTo, int rankTo);
signals:
    void valueChanged(int);
    void analyzerValueChanged(QList<AnalyzerNode>);
    void calculatingMinimax(bool);
protected:
    void setupBoard();
    int evaluateBoardValue(ChessBoard board,Color maximizingColor);
    QList<QPoint> getAttackPointsPlayer(ChessBoard* board,char piece,int rank,int col);
    QList<QPoint> getAttackPointsComputer(ChessBoard* board,char piece,int rank,int col);
    Ret minimax(ChessBoard board,int depth,int alpha,int beta,Player maximizingPlayer,Color maximizingColor);
    QList<Move> getBoardMoves(const ChessBoard &board,Player player);
private:
    QList<AnalyzerNode> nodes;
    int lastMinimaxCaller = 0;
};

#endif // DERSALGORITHM_H
