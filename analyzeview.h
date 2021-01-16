#ifndef ANALYZEVIEW_H
#define ANALYZEVIEW_H

#include "analyzernode.h"

#include <QObject>
#include <QWidget>

class AnalyzeView : public QWidget
{
    Q_OBJECT
public:
    struct PaintData{
        QRect rect;
        QLine line;
        QPixmap boardPixmap;
        QString moveString;
    };
    explicit AnalyzeView(QWidget *parent = nullptr);

    // YENI DATA BU FONKSIYON ILE HAZIRLANIR
    // AMACIM TEK SEFERDE CIZIM ICIN GEREKLI VERILERI BULUP
    // KAYDEDEREK, PAINT EVENTI ICERISINE YUKLENMEMEK.
    //
    // ICERISI CORBA GIBI LAKIN SADECE CIZIM ICIN GEREKLI
    // KOORDINATLARI VS HESAPLAMAKTA.
    void setupNodes(QList<AnalyzerNode> nodes);
signals:

private:
    QList<AnalyzerNode> m_nodes;
    QList<PaintData> m_node_data;

    int m_board_size = 120;
    int dy = m_board_size*4;
    int dx = m_board_size*1.3;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    virtual QPixmap getBoardPixmap(const ChessBoard &board);

};

#endif // ANALYZEVIEW_H
