#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "analyzewindow.h"
#include "chessview.h"
#include "minimaxalgorithm.h"

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ChessView *m_view;
    MinimaxAlgorithm *m_algorithm;
    QPoint m_clickPoint;
    ChessView::FieldHighlight *m_selectedField;
    AnalyzeWindow *wndAnalyze;
private slots:
    void viewClicked(const QPoint &field);

};
#endif // MAINWINDOW_H
