/*#include "llkwin.h"
#include "ui_llkwin.h"

LLKWin::LLKWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LLKWin)
{
    ui->setupUi(this);
}

LLKWin::~LLKWin()
{
    delete ui;
}
*/

#include <stdlib.h>
#include <time.h>
#include <QtGui>
#include "llkwin.h"

//控制连线保持一秒
void sleep1(unsigned int msec)
{
    QTime time = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < time)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

LLKWin::LLKWin(QMainWindow *parent)
    :QMainWindow(parent)
{
    setWindowTitle(tr("Link Link"));
    setGeometry(100, 100, 600, 600);
    setMinimumSize(600, 600);
    setMaximumSize(600, 600);

    //设置restart选项
    startAct = new QAction(tr("&Restart"), this);
    startAct->setToolTip(tr("Restart game."));
    connect(startAct, SIGNAL(triggered()), this, SLOT(Restart()));
    /*exitAct = new QAction(tr("&Exit"), this);
    exitAct->setToolTip(tr("Exit application."));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));*/

    //加入选栏
    menuBar = new QMenuBar(this);
    appMenu = menuBar->addMenu(tr("Option"));
    appMenu->addAction(startAct);
    //appMenu->addAction(exitAct);
    //menuBar->show();

    srand( (unsigned)time( NULL ) );      //种子
    Init();							      //设置虚拟背景格子
}

//画背景
void LLKWin::paintEvent(QPaintEvent *pe)
{
    /*QPainter bg;
    QPixmap bgPix;
    bgPix.load(":/images/images/bggreen.png");
    bg.drawPixmap(0, 0, 600, 600, bgPix);*/
    DrawBackground();
}

//判断鼠标左键是否按下
void LLKWin::mousePressEvent(QMouseEvent *me)
{
    QPoint  pt;
    pt.setX(me->pos().x());
    pt.setY(me->pos().y());
    switch(me->button())
    {
    case Qt::LeftButton:
    default:
        OnLeftButtonDown(pt);
        break;
    }
}

//产生随机数用于随机挑选图片
int LLKWin::RandNumber()
{
    int num;
    num = (int)(((double)rand()/(double)RAND_MAX)*7 + 1);
    return num;
}

//设置虚拟网格背景
void LLKWin::Init()
{
    int i, j;
    for(i = 1; i < ROW-1; i++)
    {
        for(j = 1; j < COLUMN-1; j++)
        {
            bg[i][j].value = 1;				//value表示图片状态，0不显示，1正常显示，2被选中
            bg[i][j].left = i*SIZE;			//格子左边框x坐标
            bg[i][j].right = (i+1)*SIZE - 1;//格子右边框x坐标
            bg[i][j].top = j*SIZE;			//格子上边框y坐标
            bg[i][j].bottom = (j+1)*SIZE - 1;//格子下边框y坐标
            bg[i][j].centerX = bg[i][j].left + SIZE/2;
            bg[i][j].centerY = bg[i][j].top + SIZE/2;
            bg[i][j].index = RandNumber();   //格子对应图片的标号
           // if(i == 5 && j == 5) bg[i][j].index = 2;
        }
    }
    for(i=0, j=0; j < COLUMN; j++)
    {
        bg[0][j].value = 0;	//周围四列格子value置为0表示不显示
        bg[0][j].left = 0;
        bg[0][j].right = SIZE - 1;
        bg[0][j].top = j*SIZE;
        bg[0][j].bottom = (j+1)*SIZE - 1;
        bg[0][j].centerX = bg[0][j].left + SIZE/2;
        bg[0][j].centerY = bg[0][j].top + SIZE/2;
        bg[0][j].index = 0;
        bg[ROW-1][j].value = 0;
        bg[ROW-1][j].left = (ROW-1)*SIZE;
        bg[ROW-1][j].right = ROW*SIZE - 1;
        bg[ROW-1][j].top = j*SIZE;
        bg[ROW-1][j].bottom = (j+1)*SIZE - 1;
        bg[ROW-1][j].centerX = bg[ROW-1][j].left + SIZE/2;
        bg[ROW-1][j].centerY = bg[ROW-1][j].top + SIZE/2;
        bg[ROW-1][j].index = 0;
        bg[j][0].value = 0;
        bg[j][0].left = j*SIZE;
        bg[j][0].right = (j+1)*SIZE - 1;
        bg[j][0].top = 0;
        bg[j][0].bottom = SIZE - 1;
        bg[j][0].centerX = bg[j][0].left + SIZE/2;
        bg[j][0].centerY = bg[j][0].top + SIZE/2;
        bg[j][0].index = 0;
        bg[j][COLUMN-1].value = 0;
        bg[j][COLUMN-1].left = j*SIZE;
        bg[j][COLUMN-1].right = (j+1)*SIZE - 1;
        bg[j][COLUMN-1].top = (COLUMN-1)*SIZE;
        bg[j][COLUMN-1].bottom = COLUMN*SIZE - 1;
        bg[j][COLUMN-1].centerX = bg[j][COLUMN-1].left + SIZE/2;
        bg[j][COLUMN-1].centerY = bg[j][COLUMN-1].top + SIZE/2;
        bg[j][COLUMN-1].index = 0;
    }
    selectNum = 0;
    m_path.clear();
}

//重新开始
void LLKWin::Restart()
{
    Init();
    repaint();
}

//贴图
void LLKWin::DrawBackground()
{

    /*QPixmap bgPic;
    bgPic.load(":/images/images/bggreen.png");
    bgPic.drawPixmap(0, 0, 600, 600, bgPic);*/

    int i, j;

    //贴背景图
    QPainter dc(this);
    QPixmap bgPic;
    bgPic.load(":/images/images/bgblue.png");
    dc.drawPixmap(0, 0, 600, 600, bgPic);


    for(i = 1; i < ROW-1; i++)
    {
        for(j = 1; j < COLUMN-1; j++)
        {
            if( bg[i][j].value )
            {
                /*QPainter bg;
                QPixmap bgPix;
                bgPix.load(":/images/images/bgbluebig_.png");
                bg.drawPixmap(0, 0, 600, 600, bgPix);*/

                //贴每一个格子的小图
                QPixmap pmp;
                QString str;
                str = QString(tr(":/images/images/%1.png")).arg(bg[i][j].index);
                //str = ":/images/1.jpg";
                pmp.load(str);
                dc.drawPixmap(bg[i][j].left, bg[i][j].top, SIZE-1, SIZE-1, pmp);

            }
            if(bg[i][j].value == 2)
            {
                QPixmap pmp;
                QString str;
                //str = QString(tr(":/images/images/%1pix.png")).arg(bg[i][j].index);
                pmp.load(":/images/images/press2.png");
                //选中后贴上被选中后的图
                dc.drawPixmap(bg[i][j].left, bg[i][j].top, SIZE-1, SIZE-1, pmp);
                /*dc.setPen(Qt::blue);
                dc.drawLine(bg[i][j].left, bg[i][j].top, bg[i][j].right, bg[i][j].top);
                dc.drawLine(bg[i][j].right, bg[i][j].top, bg[i][j].right, bg[i][j].bottom);
                dc.drawLine(bg[i][j].right, bg[i][j].bottom, bg[i][j].left, bg[i][j].bottom);
                dc.drawLine(bg[i][j].left, bg[i][j].bottom, bg[i][j].left, bg[i][j].top);*/
            }

            //画两个图片消除时的路径
            if(m_path.count() > 1)
            {
                QPoint ptStart, ptEnd;
                for(int k=0; k<m_path.size()-1; k++)
                {
                    ptStart = m_path.at(k);
                    ptEnd = m_path.at(k+1);
                    dc.setPen(Qt::darkYellow);
                    dc.drawLine(ptStart, ptEnd);
                }
            }
        }
    }
}

//鼠标左键按下时执行的处理函数
void LLKWin::OnLeftButtonDown(QPoint pt)
{
    int x = 0;
    int y = 0;
    x = pt.x()/SIZE;
    y = pt.y()/SIZE;
    if(x >= 0 && x <ROW && y >= 0 && y < COLUMN )
    {
        if((selectNum < 2) && (bg[x][y].value == 1))
        {
            bg[x][y].value = 2;
            ++selectNum;
            repaint();
            switch(selectNum)
            {
            case 1:
                selectPt1.setX(x);
                selectPt1.setY(y);
                break;
            case 2:
                selectPt2.setX(x);
                selectPt2.setY(y);
                break;
            }
        }
    }
    if(selectNum == 2)
    {
        if(ProcessSelectedTwoPoint(selectPt1.x(), selectPt1.y(), selectPt2.x(), selectPt2.y()))
        {
            bg[selectPt1.x()][selectPt1.y()].value = 0;
            bg[selectPt2.x()][selectPt2.y()].value = 0;
            update();
        }
        else
        {
            bg[selectPt1.x()][selectPt1.y()].value = 1;
            bg[selectPt2.x()][selectPt2.y()].value = 1;
        }
        selectNum = 0;
        sleep1(1000);
    }
    m_path.clear();
    update();
}

//处理两个已选图片关系
int LLKWin::ProcessSelectedTwoPoint(int x1, int y1, int x2, int y2)
{
    int ret = 0;
    int temp;
    if(bg[x1][y1].index != bg[x2][y2].index)
    {
        return ret;
    }
    if( x1 > x2 )
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if( y1 == y2 )   //same one row
    {
        ret = ProcTwoPointSameLine(x1, y1, x2, y2);
    }
    else if( x1 == x2 )  //same one column
    {
        ret = ProcTwoPointSameColumn(x1, y1, x2, y2);
    }
    else
    {
        if(((x1+1) == x2 && (y1+1) == y2) || ((x1+1) == x2 && (y1-1) == y2) )
        {
            ret = ProcTwoPointNear(x1, y1, x2, y2);
        }
        else
        {
            ret = ProcTwoPointApart(x1, y1, x2, y2);
        }
    }
    return ret;
}
int LLKWin::ProcTwoPtSameLineBetween(int x1, int y1, int x2, int y2)
{
    int i;
    int temp;
    if( x1 > x2)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    for(i=x1+1; i<x2; i++)
    {
        if(bg[i][y1].value != 0)
        {
            return 0;
        }
    }
    return 1;
}

//两图片在同一行
int LLKWin::ProcTwoPointSameLine(int x1, int y1, int x2, int y2)
{
    int ret = 1;
    int i,j,m;
    if( (x1+1) == x2 )
    {
        QPoint pt;
        pt.setX( bg[x1][y1].centerX );
        pt.setY( bg[x1][y1].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x2][y2].centerX );
        pt.setY( bg[x2][y2].centerY );
        m_path.push_back(pt);
        return 1;
    }
    ret = ProcTwoPtSameLineBetween(x1, y1, x2, y2);
    if( ret == 1)
    {
        QPoint pt;
        for(i=x1; i<=x2; i++)
        {
            pt.setX( bg[i][y1].centerX );
            pt.setY( bg[i][y1].centerY );
            m_path.push_back(pt);
        }
    }
    if(ret == 0)
    {
        for(i=x1, j=y1-1, m=x2; j>=0; j--)
        {
            if(bg[i][j].value == 0 && bg[m][j].value == 0)
            {
                ret = ProcTwoPtSameLineBetween(i, j, m, j);
                if(ret == 1)
                {
                    int k,l;
                    QPoint pt;
                    for(k=x1, l=y1; l>j; l--)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=x1, l=j; k<x2; k++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=x2, l=j; l<=y2; l++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    break;
                }
            }
            else
            {
                ret = 0;
                break;
            }
        }
    }
    if(ret == 0)
    {
        for(i=x1, j=y1+1, m=x2; j<COLUMN; j++)
        {
            if(bg[i][j].value == 0 && bg[m][j].value == 0)
            {
                ret = ProcTwoPtSameLineBetween(i, j, m, j);
                if(ret == 1)
                {
                    int k,l;
                    QPoint pt;
                    for(k=x1, l=y1; l<j; l++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=x1, l=j; k<x2; k++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=x2, l=j; l>=y2; l--)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    break;
                }
            }
            else
            {
                ret = 0;
                break;
            }
        }
    }
    return ret;
}
int LLKWin::ProcTwoPtSameColumnBetween(int x1, int y1, int x2, int y2)
{
    int i;
    int temp;
    if( y1 > y2)
    {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    for(i=y1+1; i<y2; i++)
    {
        if(bg[x1][i].value != 0)
        {
            return 0;
        }
    }
    return 1;
}
int LLKWin::ProcTwoPointSameColumn(int x1, int y1, int x2, int y2)
{
    int ret = 1;
    int i,j,n;
    int temp;
    if( y1 > y2)
    {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if( (y1+1) == y2 )
    {
        QPoint pt;
        pt.setX( bg[x1][y1].centerX );
        pt.setY( bg[x1][y1].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x2][y2].centerX );
        pt.setY( bg[x2][y2].centerY );
        m_path.push_back(pt);
        return 1;
    }
    ret = ProcTwoPtSameColumnBetween(x1, y1, x2, y2);
    if(ret == 1)
    {
        QPoint pt;
        for(j=y1; j<=y2; j++)
        {
            pt.setX( bg[x1][j].centerX );
            pt.setY( bg[x1][j].centerY );
            m_path.push_back(pt);
        }
    }
    if(ret == 0)
    {
        for(i=x1-1, j=y1, n=y2; i>=0; i--)
        {
            if(bg[i][j].value == 0 && bg[i][n].value == 0)
            {
                ret = ProcTwoPtSameColumnBetween(i, j, i, n);
                if(ret == 1)
                {
                    int k,l;
                    QPoint pt;
                    for(k=x1, l=y1; k>i; k--)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=i, l=y1; l<y2; l++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=i, l=y2; k<=x1; k++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    break;
                }
            }
            else
            {
                ret = 0;
                break;
            }
        }
    }
    if(ret == 0)
    {
        for(i=x1+1, j=y1, n=y2; i<ROW; i++)
        {
            if(bg[i][j].value == 0 && bg[i][n].value == 0)
            {
                ret = ProcTwoPtSameColumnBetween(i, j, i, n);
                if(ret == 1)
                {
                    int k,l;
                    QPoint pt;
                    for(k=x1, l=y1; k<i; k++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=i, l=y1; l<y2; l++)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    for(k=i, l=y2; k>=x1; k--)
                    {
                        pt.setX( bg[k][l].centerX );
                        pt.setY( bg[k][l].centerY );
                        m_path.push_back(pt);
                    }
                    break;
                }
            }
            else
            {
                ret = 0;
                break;
            }
        }
    }
    return ret;
}
int LLKWin::ProcTwoPointNear(int x1, int y1, int x2, int y2)
{
    if( bg[x1][y2].value == 0)
    {
        QPoint pt;
        pt.setX( bg[x1][y1].centerX );
        pt.setY( bg[x1][y1].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x1][y2].centerX );
        pt.setY( bg[x1][y2].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x2][y2].centerX );
        pt.setY( bg[x2][y2].centerY );
        m_path.push_back(pt);
        return 1;
    }
    if(bg[x2][y1].value == 0)
    {
        QPoint pt;
        pt.setX( bg[x1][y1].centerX );
        pt.setY( bg[x1][y1].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x2][y1].centerX );
        pt.setY( bg[x2][y1].centerY );
        m_path.push_back(pt);
        pt.setX( bg[x2][y2].centerX );
        pt.setY( bg[x2][y2].centerY );
        m_path.push_back(pt);
        return 1;
    }
    return 0;
}
int LLKWin::ProcTwoPointApart(int x1, int y1, int x2, int y2)
{
    int i, j;
    int ret = 0;
    for(j=0; j<COLUMN; j++)
    {
        if( (bg[x1][j].value != 1) && (bg[x2][j].value != 1) )
        {
            if( j != y1 )
            {
                if( ProcTwoPtSameColumnBetween(x1, y1, x1, j) == 0)
                    continue;
            }
            if( j != y2 )
            {
                if( ProcTwoPtSameColumnBetween(x2, y2, x2, j) == 0)
                    continue;
            }
            if( ProcTwoPtSameLineBetween(x1, j, x2, j) == 0)
                continue;
            int k,l;
            QPoint pt;
            if(y1 > j)
            {
                for(k=x1, l=y1; l>j; l--)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            else
            {
                for(k=x1, l=y1; l<j; l++)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            for(k=x1, l=j; k<x2; k++)
            {
                pt.setX( bg[k][l].centerX );
                pt.setY( bg[k][l].centerY );
                m_path.push_back(pt);
            }
            if(y2 > j)
            {
                for(k=x2, l=j; l<=y2; l++)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            else
            {
                for(k=x2, l=j; l>=y2; l--)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            return 1;
        }
        else
        {
            continue;
        }
    }
    for(i=0; i<ROW; i++)
    {
        if( (bg[i][y1].value != 1) && (bg[i][y2].value != 1) )
        {
            if( i != x1 )
            {
                if( ProcTwoPtSameLineBetween(x1, y1, i, y1) == 0)
                    continue;
            }
            if( i != x2 )
            {
                if( ProcTwoPtSameLineBetween(x2, y2, i, y2) == 0)
                    continue;
            }
            if( ProcTwoPtSameColumnBetween(i, y1, i, y2) == 0)
                continue;
            int k,l;
            QPoint pt;
            if(x1 > i)
            {
                for(k=x1, l=y1; k>i; k--)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            else
            {
                for(k=x1, l=y1; k<i; k++)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            if(y1 < y2)
            {
                for(k=i, l=y1; l<y2; l++)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            else
            {
                for(k=i, l=y1; l>y2; l--)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            if(x2 > i)
            {
                for(k=i, l=y2; k<=x2; k++)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            else
            {
                for(k=i, l=y2; k>=x2; k--)
                {
                    pt.setX( bg[k][l].centerX );
                    pt.setY( bg[k][l].centerY );
                    m_path.push_back(pt);
                }
            }
            return 1;
        }
        else
        {
            continue;
        }
    }
    return ret;
}
