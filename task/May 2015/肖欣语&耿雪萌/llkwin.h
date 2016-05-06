/*#ifndef LLKWIN_H
#define LLKWIN_H

#include <QWidget>

namespace Ui {
class LLKWin;
}

class LLKWin : public QWidget
{
    Q_OBJECT

public:
    explicit LLKWin(QWidget *parent = 0);
    ~LLKWin();

private:
    Ui::LLKWin *ui;
};

#endif // LLKWIN_H
*/
#ifndef LLK_H
#define LLK_H
#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVector>
#include <QMainWindow>
#define ROW 12
#define COLUMN 12
#define SIZE 50
typedef struct {
    int centerX;
    int centerY;
    int value;  /*0:clear, 1:normal, 2:selected*/
    int left;
    int right;
    int top;
    int bottom;
    int index;  /*bitmap index*/
}stBackgroundCell;
void sleep1(unsigned int msec);

class LLKWin: public QMainWindow
{
    Q_OBJECT

public:
    LLKWin(QMainWindow *parent=0);
protected:
    void paintEvent(QPaintEvent *pe);
    void mousePressEvent(QMouseEvent *me);
private:
    void Init();
    void DrawBackground();
    int RandNumber();

    void OnLeftButtonDown(QPoint pt);
    int ProcTwoPointApart(int x1, int y1, int x2, int y2);
    int ProcTwoPtSameColumnBetween(int x1, int y1, int x2, int y2);
    int ProcTwoPtSameLineBetween(int x1, int y1, int x2, int y2);
    int ProcTwoPointSameColumn(int x1, int y1, int x2, int y2);
    int ProcTwoPointSameLine(int x1, int y1, int x2, int y2);
    int ProcTwoPointNear(int x1, int y1, int x2, int y2);
    int ProcessSelectedTwoPoint(int x1, int y1, int x2, int y2);
    stBackgroundCell bg[ROW][COLUMN];
    int selectNum;
    QPoint selectPt1, selectPt2;
    QVector<QPoint> m_path;
    QMenuBar *menuBar;
    QMenu *appMenu;
    QAction *startAct;
    QAction *exitAct;
private slots:
    void Restart();
};
#endif // LLK_H
