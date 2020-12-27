#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QIcon>
#include <QMap>
#include <QPointer>
#include <QWidget>
#include "chessboard.h"

class ChessView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize fieldSize
               READ fieldSize WRITE setFieldSize
               NOTIFY fieldSizeChanged)

public:

    class Highlight{
    public:
        Highlight(){}
        virtual ~Highlight() {}
        virtual int type() const { return 0;}
    };

    class FieldHighlight : public Highlight
    {
    public:
        enum { Type = 1 };
        FieldHighlight(int column,int rank, QColor color): m_field(column,rank), m_color(color){}
        inline int column() const {return m_field.x();}
        inline int rank() const {return m_field.y();}
        inline QColor color() const {return m_color;}
        int type() const { return Type;}
    private:
        QPoint m_field;
        QColor m_color;
    };

    explicit ChessView(QWidget *parent = nullptr);
    void setBoard(ChessBoard *board);
    QSize fieldSize() const { return m_fieldSize; }
    QSize sizeHint() const;
    QRect fieldRect(int column,int rank) const;
    QPoint fieldAt(const QPoint &pt) const;
    void setPiece(char type,const QIcon &icon);
    QIcon piece(char type) const;
    ChessBoard *board() const { return m_board;}

    void addHighlight(Highlight *h1);
    void removeHighlight(Highlight *h1);
    inline Highlight *highlight(int index) const { return m_highlights.at(index);}
    inline int highlightCount() const { return m_highlights.size();}

public slots:
    void setFieldSize(QSize fieldSize)
    {
        if (m_fieldSize == fieldSize)
            return;

        m_fieldSize = fieldSize;
        emit fieldSizeChanged(m_fieldSize);
        updateGeometry();
    }
    void setValue(int);
    virtual void drawGameOver();
private:
    QPointer<ChessBoard> m_board;
    QSize m_fieldSize;
    QMap<char,QIcon> m_pieces;
    QList<Highlight*> m_highlights;
    int m_value = 0;
signals:

    void fieldSizeChanged(QSize fieldSize);
    void clicked(const QPoint &);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    virtual void drawRank(QPainter *painter,int rank);
    virtual void drawColumn(QPainter *painter,int column);
    virtual void drawField(QPainter *painter,int column,int rank);
    virtual void drawPiece(QPainter *painter,int column,int rank);
    virtual void drawHighlights(QPainter *painter);
    virtual void drawValue(QPainter *painter);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CHESSVIEW_H
