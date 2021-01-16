#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QHBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    m_view = new ChessView;
    m_algorithm = new MinimaxAlgorithm(this);
    wndAnalyze = new AnalyzeWindow;

    connect(m_algorithm,SIGNAL(calculatingMinimax(bool)),wndAnalyze,SLOT(showWaitText(bool)));
    connect(m_view,SIGNAL(clicked(QPoint)),
            this,SLOT(viewClicked(QPoint)));
    connect(m_algorithm,SIGNAL(valueChanged(int)),
            m_view,SLOT(setValue(int)));
    connect(m_algorithm,SIGNAL(gameOver()),m_view,SLOT(drawGameOver()));

    connect(m_algorithm,SIGNAL(analyzerValueChanged(QList<AnalyzerNode>)),
            wndAnalyze,SLOT(updateNodes(QList<AnalyzerNode>)));

    m_view->setPiece('P',
                     QIcon(":/pieces/Chess_plt45.svg"));
    //pawn
    m_view->setPiece('K',
                     QIcon(":/pieces/Chess_klt45.svg"));
    //king
    m_view->setPiece('R',
                     QIcon(":/pieces/Chess_rlt45.svg"));
    //rook


    m_view->setPiece('p',
                     QIcon(":/pieces/Chess_pdt45.svg"));
    //pawn
    m_view->setPiece('k',
                     QIcon(":/pieces/Chess_kdt45.svg"));
    //king

    m_view->setPiece('r',
                     QIcon(":/pieces/Chess_rdt45.svg"));
    //rook


    m_algorithm->newGame();
    m_view->setBoard(m_algorithm->board());


    //setCentralWidget(m_view);
//    m_view->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
//    m_analyze->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

//    QGraphicsScene *s1 = new QGraphicsScene;
//    QGraphicsScene *s2 = new QGraphicsScene;
//    s1->addWidget(m_view);
//    s2->addWidget(m_analyze);

//    ui->view1->setScene(s1);
//    ui->view2->setScene(s2);
    //hl->addWidget(m_view);
    //hl->addWidget(m_analyze);

    setCentralWidget(m_view);
    m_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_view->setFieldSize(QSize(50,50));
//    ui->view1->fitInView(m_view->rect());
    m_selectedField = nullptr;
    wndAnalyze->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::viewClicked(const QPoint &field)
{
    if(m_clickPoint.isNull()) {
        if(m_view->board()->data(field.x(),field.y()) != ' '){
            m_clickPoint = field;
            m_selectedField = new ChessView::FieldHighlight(
                        field.x(), field.y(), QColor(255, 0, 0, 50)
                        );
            m_view->addHighlight(m_selectedField);
        }
    } else {
        if(field != m_clickPoint) {
            m_algorithm->move(
                        m_clickPoint.x(), m_clickPoint.y(),
                        field.x(), field.y());
        }
        m_clickPoint = QPoint();
        m_view->removeHighlight(m_selectedField);
        delete m_selectedField;
        m_selectedField = 0;
    }

}

