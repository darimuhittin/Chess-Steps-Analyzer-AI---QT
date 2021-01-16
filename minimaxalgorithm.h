#ifndef MINIMAXALGORITHM_H
#define MINIMAXALGORITHM_H

#include "analyzernode.h"
#include "boardnode.h"
#include "chessalgorithm.h"
#include <QObject>

class MinimaxAlgorithm : public ChessAlgorithm
{
    Q_OBJECT
public:
    // COKLU DEGER DONDURMEK ICIN OLUSTURDUM BASKA BIR AMACI YOK
    struct MinimaxReturnType{
        Move move;
        int eval_value;
    };

    // TEK SATIR ICIN BURADA TANIMLADIM.
    // TEMIZLIK ICIN SOURCE A ALINABILIR.
    MinimaxAlgorithm(QObject *parent = nullptr):ChessAlgorithm(parent){};

    // ChessAlgorithm interface
public slots:
    void newGame();
    bool move(int colFrom, int rankFrom, int colTo, int rankTo);
signals:
    void valueChanged(int);

    // ANALIZ PENCERESINE DATA YOLLAMAK AMACI ILE OLUSTURULDU
    void analyzerValueChanged(QList<AnalyzerNode>);

    void calculatingMinimax(bool);
protected:
    void setupBoard();
    int evaluateBoardValue(ChessBoard board,Color maximizingColor);

    // ASAGIDAKI IKI FONKSIYON IPTAL EDILDI.
    // ISTEGE GORE SILINEBILIR :
    QList<QPoint> getAttackPointsPlayer(ChessBoard* board,char piece,int rank,int col);
    QList<QPoint> getAttackPointsComputer(ChessBoard* board,char piece,int rank,int col);
    // IPTAL EDILEN FONKSIYONLARIN SONU.


    MinimaxReturnType minimax(ChessBoard board,int depth,int alpha,int beta,Player maximizingPlayer,Color maximizingColor);
    QList<Move> getBoardMoves(const ChessBoard &board,Player player);
private:
    QList<AnalyzerNode> nodes;
    int lastMinimaxCaller = 0;
};

#endif // MINIMAXALGORITHM_H
