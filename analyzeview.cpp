#include "analyzeview.h"

#include <QPainter>
#include <QIcon>
#include <QtDebug>
#include <QWheelEvent>

AnalyzeView::AnalyzeView(QWidget *parent):QWidget(parent)
{
    resize(m_board_size * m_nodes.length()+2 + 1200,(m_board_size+dy) * 3 + 300);

}

void AnalyzeView::setNodes(QList<AnalyzerNode> nodes)
{
    m_nodes = nodes;
    resize(m_board_size * m_nodes.length()+2  + 1200,(m_board_size+dy) * 3 + 300);
    update();

    m_node_data.clear();

    int birIndex = 0;
    int ikiIndex = 0;
    int ucIndex = 0;

    int sifir = 0;
    int bir = 0;
    int iki = 0;
    int parentRectX=0;
    int parentRectY=0;
    for (int i = 1;i<m_nodes.length() ;i++ ){
        switch (m_nodes[i].depth()) {
        case 1:
            sifir++;
            break;
        case 2:
            bir++;
            break;
        case 3:
            iki++;
            break;
        }
    }
    int genislik = sifir * (m_board_size+dx);
    int off0=dx ,off1 =dx, off2 = dx;

    if(iki > 0){
        off2 = (genislik/iki) / 2;
    }
    if(bir > 0)
    {
        off1 = (genislik/bir) / 2;
    }
    if(sifir > 0){
        off0 = (genislik/sifir) / 2;
    }

    int baslangicX = genislik/2 - m_board_size/2;
    int baslangicY = m_board_size/2-15;


    m_node_data.append({
                           QRect(baslangicX,baslangicY,m_board_size,m_board_size),
                           QLine(0,0,0,0),
                           getBoardPixmap(m_nodes[0].board()),
                           "Move : (" + QString::number(m_nodes[0].moveToMake().fromCol) + "," +
                           QString::number(m_nodes[0].moveToMake().fromRank) + ") - ("+
                           QString::number(m_nodes[0].moveToMake().toCol) + ","+
                           QString::number(m_nodes[0].moveToMake().toRank) + ") "
                       });

    for (int i = 1;i<m_nodes.length() ;i++ ) {
        int depth = m_nodes[i].depth();
        int rectX = 0;
        switch (depth) {
        case 3:
            rectX = ((ucIndex*2)+1)*off2 - m_board_size/2  ;
            break;
        case 2:
            rectX = ((ikiIndex*2)+1)*off1 - m_board_size/2 ;
            break;
        case 1:
            rectX = ((birIndex*2)+1)*off0 - m_board_size/2 ;
            break;
        }
        int rectY = (5-(depth+1))* dy;
        int bs = m_board_size;

        QLine line;
        switch (depth) {
        case 3:
            line = QLine(QPoint(rectX+m_board_size/2,rectY),QPoint(genislik/2,m_board_size*1.5));
            ucIndex++;
            break;
        case 2:
            parentRectX = (((ucIndex)*2)+1)*off2 - m_board_size/2;
            parentRectY = (5-(4))*dy;
            line = QLine(QPoint(parentRectX+m_board_size/2,parentRectY+m_board_size),QPoint(rectX+m_board_size/2,rectY));
            ikiIndex++;
            break;
        case 1:
            parentRectX = (((ikiIndex)*2)+1)*off1 - m_board_size/2;
            parentRectY = (5-(3))*dy;
            line = QLine(QPoint(parentRectX+m_board_size/2,parentRectY+m_board_size),QPoint(rectX+m_board_size/2,rectY));
            birIndex++;
            break;
        }
        m_node_data.append({
                               QRect(rectX,rectY,bs,bs),
                               line,
                               getBoardPixmap(m_nodes[i].board()),
                               "Move : (" + QString::number(m_nodes[i].moveToMake().fromCol) + "," +
                               QString::number(m_nodes[i].moveToMake().fromRank) + ") - ("+
                               QString::number(m_nodes[i].moveToMake().toCol) + ","+
                               QString::number(m_nodes[i].moveToMake().toRank) + ") "
                           });

    }

}

void AnalyzeView::paintEvent(QPaintEvent *event)
{
    QPainter ptr(this);

    if(m_nodes.empty()){
        QWidget::paintEvent(event);
        return;
    }

    for (int i = 0;i<m_nodes.length() ;i++ ) {
        QRect rect = m_node_data[i].rect;
        //drawBoard(m_nodes[i].board(),rect,ptr);
        ptr.drawPixmap(rect,m_node_data[i].boardPixmap);

        ptr.drawText(rect.x()+m_board_size+20,rect.y()+10,"Index : " + QString::number(m_nodes[i].index()));
        ptr.drawText(rect.x()+m_board_size+20,rect.y()+20,"Depth : " + QString::number(4-m_nodes[i].depth()));

        ptr.save();
        ptr.setFont(QFont("Arial",15,120));
        ptr.setPen(Qt::magenta);

        if(i!=0)
            ptr.drawText(rect.x(),rect.y()-40,m_node_data[i].moveString);

        ptr.setFont(QFont("Arial",15,15));
        ptr.setPen(Qt::green);
        //DRAW CURRENT EVAL
        ptr.drawText(rect.x()+m_board_size+20,rect.y()+40,"Minimax : " + QString::number(m_nodes[i].current_eval()));
        //DRAW BOARD VAL
        ptr.setPen(Qt::red);
        ptr.drawText(rect.x()+m_board_size+20,rect.y()+60,"Value : " + QString::number(m_nodes[i].board_value()));
        ptr.restore();

        ptr.setPen(QPen(Qt::black,2));//line width
        ptr.drawLine(m_node_data[i].line);


    }
    ptr.save();
    ptr.setFont(QFont("Arial",30));
    ptr.drawText(50,100,QString("Toplam analiz edilen tahta sayisi %1").arg(m_nodes.length()-1));

    ptr.restore();
}

QPixmap AnalyzeView::getBoardPixmap(const ChessBoard &board)
{
    QPixmap pm(m_board_size-3,m_board_size-3);
    QPainter painter(&pm);

    QRect rect(0,0,m_board_size,m_board_size);
    //DRAW FIELD
    int fieldWidth = rect.width()/6;
    for(int col=1; col<=6; col++){
        for(int rank = 1; rank <= 6; rank++){
            QRect rectField(((col-1)*fieldWidth),((rank-1)*fieldWidth),fieldWidth,fieldWidth);
            QColor fillColor = (col+rank) % 2 ? palette().
                                                color(QPalette::Light) : palette().color(QPalette::Mid);
            painter.setPen(fillColor);
            painter.setBrush(fillColor);
            painter.drawRect(rectField);
            switch (board.data(col,7-rank)) {
            case 'p':
                QIcon(":/pieces/Chess_pdt45.svg").paint(&painter,rectField);
                break;
            case 'r':
                QIcon(":/pieces/Chess_rdt45.svg").paint(&painter,rectField);
                break;
            case 'k':
                QIcon(":/pieces/Chess_kdt45.svg").paint(&painter,rectField);
                break;
            case 'P':
                QIcon(":/pieces/Chess_plt45.svg").paint(&painter,rectField);
                break;
            case 'R':
                QIcon(":/pieces/Chess_rlt45.svg").paint(&painter,rectField);
                break;
            case 'K':
                QIcon(":/pieces/Chess_klt45.svg").paint(&painter,rectField);
                break;
            }
        }
    }

    return pm;
}


