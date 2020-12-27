#include "boardnode.h"
#include "dersalgorithm.h"
#include <QtMath>
#include <QList>
#include <QPoint>
#include <QDebug>
#include <QString>

#define DEBUG 0
#define ANALYZE 0
#define MINIMAX_DEPTH 7

DersAlgorithm::DersAlgorithm(QObject *parent):ChessAlgorithm(parent)
{

}

void DersAlgorithm::newGame()
{
    setupBoard();
    board()->setFen("r1pkp1/6/6/6/6/2PKPR w");

    setResult(NoResult);
    setCurrentPlayer(Player1);

}

bool DersAlgorithm::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
    bool isValid = false;
    if(currentPlayer() == NoPlayer || currentPlayer() == Player2) return false;

    if(currentPlayer() == Player1){
        switch (board()->data(colFrom,rankFrom)) {
        case 'P':
            if(rankTo-rankFrom == 1 && colFrom == colTo && board()->data(colTo,rankTo) == ' '){
                isValid = true;
            }
            if(rankTo-rankFrom == 1 && colFrom == colTo-1 &&  QChar(board()->data(colTo,rankTo)).isLower()){
                isValid = true;
            }
            if(rankTo-rankFrom == 1 && colFrom == colTo+1 &&  QChar(board()->data(colTo,rankTo)).isLower()){
                isValid = true;
            }
            break;
        case 'R':
            if(colFrom == colTo || rankFrom == rankTo) isValid = true;
            break;
        case 'K':
            if(abs(colFrom-colTo) <= 1 && abs(rankFrom-rankTo) <= 1){
                isValid = true;
            }
            break;
        }
    }

    //MOVE IF VALID
    if(isValid){
        board()->movePiece(colFrom,rankFrom,colTo,rankTo);
        if(board()->data(colTo,rankTo) == 'k'){
            setResult(Result::Player1Wins);
            return true;
        }
    }else{
        return false;
    }

    // A - B NEG - POS FOR PLAYER
    // A - B  POS - NEG FOR ENEMY
#if ANALYZE == 1
    //ANALIZ BASLANGIC VERI
    nodes.append(AnalyzerNode(*board(),MINIMAX_DEPTH,0,lastMinimaxCaller,{0,0,0,0},0,evaluateBoardValue(*board(),White)));
    //ANALIZ BASLANGIC SONU
#endif

    emit calculatingMinimax(true);
    Ret bestRet = minimax(*board(),MINIMAX_DEPTH,INT32_MIN,INT32_MAX,Player2,White);
    emit calculatingMinimax(false);

    Move best = bestRet.move;

    board()->movePiece(best.fromCol,best.fromRank,best.toCol,best.toRank);
    if(board()->data(colTo,rankTo) == 'k'){
        setResult(Result::Player1Wins);
        return true;
    }

    qDebug() << "BM" << best.fromCol << best.fromRank
             << best.toCol << best.toRank;
    //const ChessBoard *b = board();

    emit valueChanged(evaluateBoardValue(*board(),White));
#if ANALYZE == 1
    emit analyzerValueChanged(nodes);
    nodes.clear();
#endif
    //evaluateBoardValue(board());
    //CHANGE PLAYER
    //setCurrentPlayer(currentPlayer() == Player1 ? Player2 : Player1);
}

DersAlgorithm::Ret DersAlgorithm::minimax(ChessBoard board, int depth,int alpha,int beta,
                                          ChessAlgorithm::Player maximizingPlayer,
                                          ChessAlgorithm::Color maximizingColor)
{
    if (depth == 0){
        return {{0,0,0,0},evaluateBoardValue(board,maximizingColor)};
    }

    QList<Move> moves = getBoardMoves(board,maximizingPlayer);
    if(moves.isEmpty())
        return {{0,0,0,0},evaluateBoardValue(board,maximizingColor)};

    Move bestMove = moves.at(rand()%moves.length());

    // Player 1 player - player 2 comp
    if(maximizingPlayer == Player1){
        int max_eval = INT32_MIN;
        foreach(Move move,moves){
            //ILERI HAMLE
            board.movePiece(move.fromCol,move.fromRank,move.toCol,move.toRank);
#if DEBUG == 1
            qDebug() << "Depth :" << depth-1 << " Index : " << moves.indexOf(move);
            qDebug() << "\tMove :" << QString("(%1,%2)-(%3,%4)").arg(
                            move.fromCol).
                        arg(move.fromRank).
                        arg(move.toCol).
                        arg(move.toRank);
            qDebug() << "\tValue" << evaluateBoardValue(board,maximizingColor);
#endif
            int current_eval = minimax(board,depth-1,alpha,beta,Player2,maximizingColor).eval_value;
#if ANALYZE == 1
            int moveIndex = moves.indexOf(move);
            nodes.append(AnalyzerNode(board,depth,moveIndex,lastMinimaxCaller,move,current_eval,evaluateBoardValue(board,maximizingColor)));
            lastMinimaxCaller = moveIndex;
#endif
            //GERI HAMLE
            board.movePiece(move.toCol,move.toRank,move.fromCol,move.fromRank);
            if(current_eval > max_eval){
                max_eval = current_eval;
                bestMove = move;
            }
            alpha = alpha > current_eval ? alpha : current_eval;
            if (beta <= alpha)
                break;
        }
        return {bestMove,max_eval};
    }else if(maximizingPlayer == Player2){
        int min_eval = INT32_MAX;
        foreach(Move move,moves){

            //tam geri alma icin yedigin hamleyi geri koymak icin sakla
            char temp = board.data(move.toCol,move.toRank);
            board.movePiece(move.fromCol,move.fromRank,move.toCol,move.toRank);
#if DEBUG == 1
            qDebug() << "Depth :" << depth-1 << " Index : " << moves.indexOf(move);
            qDebug() << "\tMove :" << QString("(%1,%2)-(%3,%4)").arg(
                            move.fromCol).
                        arg(move.fromRank).
                        arg(move.toCol).
                        arg(move.toRank);
            qDebug() << "\tValue" << evaluateBoardValue(board,maximizingColor);
#endif
            int current_eval = minimax(board,depth-1,alpha,beta,Player1,maximizingColor).eval_value;
#if ANALYZE == 1
            int moveIndex = moves.indexOf(move);
            nodes.append(AnalyzerNode(board,depth,moveIndex,lastMinimaxCaller,move,current_eval,evaluateBoardValue(board,maximizingColor)));
            lastMinimaxCaller = moveIndex;
#endif
            board.movePiece(move.toCol,move.toRank,move.fromCol,move.fromRank);
            //hamle geri alindiktan sonra bos kalan yere tekrar tasi koy
            board.setData(move.toCol,move.toRank,temp);
            if(current_eval < min_eval){
                min_eval = current_eval;
                bestMove = move;
            }
            beta = beta < current_eval ? beta : current_eval;
            if (beta <= alpha)
                break;
        }
        return {bestMove,min_eval};
    }
}

int DersAlgorithm::evaluateBoardValue(ChessBoard board,Color maximizingColor)
{
    int value = 0;
    for (int i=1;i<=6;i++) {
        for (int j=1;j<=6;j++) {
            char c = board.data(i,j);
            switch (c) {
            case 'r':
                value -= 50;
                break;
            case 'k':
                value -= 99999;
                break;
            case 'p':
                value -= 10;
                break;
            case 'R':
                value += 50;
                break;
            case 'K':
                value += 99999;
                break;
            case 'P':
                value += 10;
                break;
            }
        }
    }
    if(maximizingColor == Black)
        value *= -1;
    return value;
}

// IPTAL
QList<QPoint> DersAlgorithm::getAttackPointsPlayer(ChessBoard* board,char piece,int rank,int col)
{
    QList<QPoint> attackPoints;
    switch (piece) {
    case 'P':
        if(rank>1 || col+1 <= 6 || col-1 >= 1){
            attackPoints.append(QPoint(col+1,rank+1));
            attackPoints.append(QPoint(col-1,rank+1));
        }
        break;
    case 'R':
        //cols
        for(int x=col;x>=1;x--){
            if(col != x){
                char destPiece = board->data(x,rank);
                if (destPiece == 'R' ||destPiece == 'K' ||destPiece == 'P')
                    break;
                attackPoints.append(QPoint(x,rank));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'r' || destPiece == 'k' || destPiece == 'p')
                    break;
            }
        }
        for(int x=col;x<=6;x++){
            if(col != x){
                char destPiece = board->data(x,rank);
                if (destPiece == 'R' ||destPiece == 'K' ||destPiece == 'P')
                    break;
                attackPoints.append(QPoint(x,rank));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'r' || destPiece == 'k' || destPiece == 'p')
                    break;
            }
        }
        //ranks
        for(int y=rank;y>=1;y--){
            if(rank != y){
                char destPiece = board->data(col,y);
                if (destPiece == 'R' ||destPiece == 'K' ||destPiece == 'P')
                    break;
                attackPoints.append(QPoint(col,y));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'r' || destPiece == 'k' || destPiece == 'p')
                    break;
            }
        }
        for(int y=rank;y<=6;y++){
            if(rank != y){
                char destPiece = board->data(col,y);
                if (destPiece == 'R' ||destPiece == 'K' ||destPiece == 'P')
                    break;
                attackPoints.append(QPoint(col,y));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'r' || destPiece == 'k' || destPiece == 'p')
                    break;
            }
        }
        break;
    case 'K':
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                if(i==0 && j==0) continue;
                if(rank+i <=6 && rank+i >= 1 && col+j <=6 && col+j >= 1 ){
                    attackPoints.append(QPoint(col+j,rank+i));
                }
            }
        }
        break;
    }
    return attackPoints;
}
// IPTAL
QList<QPoint> DersAlgorithm::getAttackPointsComputer(ChessBoard* board,char piece, int rank, int col)
{
    QList<QPoint> attackPoints;
    switch (piece) {
    case 'p':
        if(rank>1 || col+1 <= 6 || col-1 >= 1){
            attackPoints.append(QPoint(col+1,rank-1));
            attackPoints.append(QPoint(col-1,rank-1));
        }
        break;
    case 'r':
        //cols0
        for(int x=col;x>=1;x--){
            if(col != x){
                char destPiece = board->data(x,rank);
                if (destPiece == 'r' ||destPiece == 'k' ||destPiece == 'p')
                    break;
                attackPoints.append(QPoint(x,rank));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'R' || destPiece == 'K' || destPiece == 'P')
                    break;
            }
        }
        for(int x=col;x<=6;x++){
            if(col != x){
                char destPiece = board->data(x,rank);
                if (destPiece == 'r' ||destPiece == 'k' ||destPiece == 'p')
                    break;
                attackPoints.append(QPoint(x,rank));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'R' || destPiece == 'K' || destPiece == 'P')
                    break;
            }
        }
        //ranks
        for(int y=rank;y>=1;y--){
            if(rank != y){
                char destPiece = board->data(col,y);
                if (destPiece == 'r' ||destPiece == 'k' ||destPiece == 'p')
                    break;
                attackPoints.append(QPoint(col,y));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'R' || destPiece == 'K' || destPiece == 'P')
                    break;
            }
        }
        for(int y=rank;y<=6;y++){
            if(rank != y){
                char destPiece = board->data(col,y);
                if (destPiece == 'r' ||destPiece == 'k' ||destPiece == 'p')
                    break;
                attackPoints.append(QPoint(col,y));
                //OYUNCU TASI ISE EKLEYIP DUR
                if(destPiece == 'R' || destPiece == 'K' || destPiece == 'P')
                    break;
            }
        }
        break;
    case 'k':
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                if(i==0 && j==0) continue;
                if(rank+i <=6 && rank+i >= 1 && col+j <=6 && col+j >= 1 ){
                    attackPoints.append(QPoint(col+j,rank+i));
                }
            }
        }
        break;
    }
    return attackPoints;
}

QList<Move> DersAlgorithm::getBoardMoves(const ChessBoard &board, Player player)
{
    QList<Move> moves;
    for(int col=1;col<=6;col++){
        for(int row=1;row<=6;row++){
            char pieceChar = board.data(col,row);
            if(player == Player1){
                switch (pieceChar) {
                case 'P':
                    //ATTACK
                    if((col+1) <= 6 && (row+1) <= 6){
                        char attackedPiece = board.data(col+1,row+1);
                        if(attackedPiece == 'p' || attackedPiece == 'r' || attackedPiece == 'k')
                            moves.append({col,row,col+1,row+1});
                    }
                    //ATTACK
                    if((col-1) >= 1 && (row+1) <= 6){
                        char attackedPiece = board.data(col-1,row+1);
                        if(attackedPiece == 'p' || attackedPiece == 'r' || attackedPiece == 'k')
                            moves.append({col,row,col-1,row+1});
                    }
                    //MOVE
                    if((row+1) <= 6){
                        if(board.data(col,row+1) == ' ')
                            moves.append({col,row,col,row+1});
                    }
                    break;
                case 'R':
                    //yukari
                    if(row != 6){
                        for(int r=row+1;r<=6;r++){
                            char p = board.data(col,r);
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                            moves.append({col,row,col,r});
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                        }
                    }
                    //asagi
                    if(row != 1){
                        for(int r=row-1;r>=1;r--){
                            char p = board.data(col,r);
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                            moves.append({col,row,col,r});
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                        }
                    }
                    //saga
                    if(col!=6){
                        for(int c=col+1;c<=6;c++){
                            char p = board.data(c,row);
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                            moves.append({col,row,c,row});
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                        }
                    }
                    //sola
                    if(col!=1){
                        for(int c=col-1;c>=1;c--){
                            char p = board.data(c,row);
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                            moves.append({col,row,c,row});
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                        }
                    }
                    break;
                case 'K':
                    for(int i=-1;i<=1;i++){
                        for (int j=-1;j<=1;j++) {
                            if(i==0 && j==0)
                                continue;

                            if(col+i <= 6 && col+i >= 1 && row+j <= 6 && row+j >= 1){
                                char destPiece = board.data(col+i,row+j);
                                //IS FRIEND PIECE ?
                                if(destPiece != 'P' && destPiece != 'R' && destPiece != 'K')
                                    moves.append({col,row,col+i,row+j});
                            }
                        }
                    }
                    break;
                }
            }else if(player == Player2){
                switch (pieceChar) {
                case 'p':
                    //ATTACK RIGT
                    if((col+1) <= 6 && (row-1) >= 1){
                        char attackedPiece = board.data(col+1,row-1);
                        if(attackedPiece == 'P' || attackedPiece == 'R' || attackedPiece == 'K')
                            moves.append({col,row,col+1,row-1});
                    }
                    //ATTACK LEFT
                    if((col-1) >= 1 && (row-1) >= 1){
                        char attackedPiece = board.data(col-1,row-1);
                        if(attackedPiece == 'P' || attackedPiece == 'R' || attackedPiece == 'K')
                            moves.append({col,row,col-1,row-1});
                    }
                    //MOVE
                    if((row-1) >= 1){
                        if(board.data(col,row-1) == ' ')
                            moves.append({col,row,col,row-1});
                    }
                    break;
                case 'r':
                    //yukari
                    if(row != 6){
                        for(int r=row+1;r<=6;r++){
                            char p = board.data(col,r);
                            //FRIEND
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                            //ENEMY - ADD AND STOP
                            moves.append({col,row,col,r});
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                        }
                    }
                    //asagi
                    if(row != 1){
                        for(int r=row-1;r>=1;r--){
                            char p = board.data(col,r);
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                            moves.append({col,row,col,r});
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                        }
                    }
                    //saga
                    if(col!=6){
                        for(int c=col+1;c<=6;c++){
                            char p = board.data(c,row);
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                            moves.append({col,row,c,row});
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                        }
                    }
                    //sola
                    if(col!=1){
                        for(int c=col-1;c>=1;c--){
                            char p = board.data(c,row);
                            if(p == 'p' || p == 'r' || p == 'k'){
                                break;
                            }
                            moves.append({col,row,c,row});
                            if(p == 'P' || p == 'R' || p == 'K'){
                                break;
                            }
                        }
                    }
                    break;
                case 'k':
                    for(int i=-1;i<=1;i++){
                        for (int j=-1;j<=1;j++) {
                            if(i==0 && j==0)
                                continue;

                            if(col+i <= 6 && col+i >= 1 && row+j <= 6 && row+j >= 1){
                                char destPiece = board.data(col+i,row+j);
                                // king can't move towards friend
                                if(destPiece != 'p' && destPiece != 'r' && destPiece != 'k')
                                    moves.append({col,row,col+i,row+j});
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    return moves;
}

void DersAlgorithm::setupBoard()
{
    setBoard(new ChessBoard(6,6,this));
}
//int DersAlgorithm::evaluateBoardValue(ChessBoard* board)
//{
//    int value = 0;
//    // FOR PLAYER
//    QList<QPoint> allAttackPointsPlayer;
//    for(int i = 0;i<board->boardData().length();i++){
//        const char piece = board->boardData().at(i);

//        if(piece == 'P' || piece == 'K' || piece == 'R'){
//            qDebug() << piece;
//            qDebug() << i;
//            int col = (i%6)+1;
//            int rank = (i/6)+1;
//            allAttackPointsPlayer.append(getAttackPointsPlayer(board,piece,rank,col));
//        }else {
//            continue;
//        }
//    }
//    foreach(QPoint attackPoint,allAttackPointsPlayer){
//        qDebug() << QString::number(attackPoint.x()) + "-" + QString::number(attackPoint.y());
//        switch (board->data(attackPoint.x(),attackPoint.y())) {
//        case 'p':
//            value += 1;
//            break;
//        case 'r':
//            value += 5;
//            break;
//        case 'k':
//            value += 1000;
//            break;
//        }
//    }

//    //FOR COMPUTER
//    QList<QPoint> allAttackPointsComputer;
//    for(int i = 0;i<board->boardData().length();i++){
//        const char piece = board->boardData().at(i);

//        if(piece == 'p' || piece == 'k' || piece == 'r'){
//            qDebug() << piece;
//            qDebug() << i;
//            int col = (i%6)+1;
//            int rank = (i/6)+1;
//            allAttackPointsComputer.append(getAttackPointsComputer(board,piece,rank,col));
//        }else {
//            continue;
//        }
//    }
//    foreach(QPoint attackPoint,allAttackPointsComputer){
//        qDebug() << QString::number(attackPoint.x()) + "-" + QString::number(attackPoint.y());
//        switch (board->data(attackPoint.x(),attackPoint.y())) {
//        case 'P':
//            value -= 1;
//            break;
//        case 'R':
//            value -= 5;
//            break;
//        case 'K':
//            value -= 1000;
//            break;
//        }
//    }
//    emit valueChanged(value);
//    return value;
//}
