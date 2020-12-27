#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include "analyzeview.h"

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class AnalyzeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyzeWindow(QWidget *parent = nullptr);
private:
    QGraphicsView *view = new QGraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(view);
    AnalyzeView *analyze = new AnalyzeView;
public slots:
    void updateNodes(QList<AnalyzerNode> nodes);
signals:


    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // ANALYZEWINDOW_H
