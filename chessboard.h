#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>



class ChessBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
    Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)
    Q_PROPERTY(QVector<char> boardData READ boardData)

public:

    void operator = (ChessBoard const &other);

    ChessBoard(const ChessBoard &board);
    ChessBoard(int r,int c,QObject *parent = nullptr);
    int ranks() const{ return m_ranks; }
    int columns() const{ return m_columns; }
    QVector<char> boardData() const{ return m_boardData; }
    char data(int column,int rank) const{ return m_boardData.at((rank-1)*columns() + (column-1)); }
    void setData(int column,int rank,char value);
    void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
    void setFen(const QString &fen);
private:
    int m_ranks;
    int m_columns;
    QVector<char> m_boardData;
signals:
    void ranksChanged(int rank);
    void columnsChanged(int col);
    void boardReset();
    void dataChanged(int c,int r);
protected:
    void setRanks(int newRanks);
    void setColumns(int newColumns);
    bool setDataInternal(int column,int rank,char value);
    void initBoard();


};

#endif // CHESSBOARD_H
