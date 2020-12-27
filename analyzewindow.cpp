#include "analyzewindow.h"

#include <QPainter>
#include <QWheelEvent>

AnalyzeWindow::AnalyzeWindow(QWidget *parent) : QWidget(parent)
{
    view->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    view->setScene(scene);
    scene->addWidget(analyze);
    view->scale(0.6,0.6);
    view->resize(1200,600);
    view->setRenderHints(QPainter::Antialiasing);
    resize(1200,600);
}

void AnalyzeWindow::updateNodes(QList<AnalyzerNode> nodes)
{
    analyze->setNodes(nodes);
    //view->fitInView(analyze->rect(),Qt::KeepAspectRatio);
}


void AnalyzeWindow::wheelEvent(QWheelEvent *event)
{
    //QWidget::wheelEvent(event);
    if(event->isAccepted()){
        return;
    }
    const qreal factor = 1.1;
    if(event->angleDelta().y()>0){
        view->scale(factor,factor);
    }else{
        view->scale(1/factor,1/factor);
    }
    event->accept();
}

