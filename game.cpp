#include "game.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QEvent>
#include <QPainter>
#include <QMovie>
#include <QSize>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <qlabel.h>
#include <QFont>
#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QTextEdit>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <qwidget.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <qvalidator.h>
#include <qlineedit.h>
#include <QVector3D>
#include <QOpenGLFunctions_4_0_Compatibility>
using namespace std;

//
//MakeUp函数进行相关参数的初始化
//包括按钮图片的初始化
//
void GameWindow::MakeUp()
{
    //设置按钮的参数
    list.append("background-image:url(:/image/images/button.jpg)");   //设置背景图片
    list.append("border-style:outset");                               // 边框风格
    list.append("border-width:3px");                                  // 边框宽度
    list.append("border-color:rgb(10,45,110)");                       // 边框颜色
    list.append("border-radius:20px");                                // 边框倒角
    list.append("font:bold 30px");
    stop = new QPushButton(this);
    stop->setStyleSheet(list.join(';'));
    stop->move(width()/2 - 100,height()/2 + 425);
    stop->setText("暂停游戏");
    stop->resize(200,50);
    fileSave = new QPushButton(this);
    fileSave->setStyleSheet(list.join(';'));
    fileSave->setText("保存游戏");
    fileSave->resize(200,50);
    fileSave->move(width()/2 + 100,height()/2 + 425);
    fileSave->hide();
    conTinue = new QPushButton(this);
    conTinue->setStyleSheet(list.join(';'));
    conTinue->setText("继续游戏");
    conTinue->resize(200,50);
    conTinue->move(width()/2 - 300,height()/2 + 425);
    conTinue->hide();
    //加载图片地址 可以是资源文件
    p1.load(":/image/images/pc1.jpg");
    p2.load(":/image/images/pc2.jpg");
    p3.load(":/image/images/pc3.jpg");
    p4.load(":/image/images/pc4.jpg");
    p5.load(":/image/images/pc5.jpg");
    p6.load(":/image/images/pc6.jpg");
    p7.load(":/image/images/pc7.jpg");
    p8.load(":/image/images/pc8.jpg");
    p9.load(":/image/images/pc9.jpg");
    p10.load(":/image/images/pc10.jpg");
    p11.load(":/image/images/pc11.jpg");
    sk1.load(":/image/images/sk1.jpg");
    sk2.load(":/image/images/sk2.jpg");
    sk3.load(":/image/images/sk3.jpg");
    sk4.load(":/image/images/sk4.jpg");
    sk5.load(":/image/images/sk5.png");
    sk6.load(":/image/images/sk6.jfif");
    p12.load(":/image/images/play2.jpg");
    p22.load(":/image/images/play3.jpg");
}

//
//GameWindow函数进行参数的初始化以及按钮的相关参数的初始化以及设定计时器
//将按钮的信号与对应的信号处理函数进行绑定
//
GameWindow::GameWindow(QWidget *parent):QWidget(parent)
{
    srand((unsigned)time(NULL));//设置随机数的种子
    m = n = -1;
    deleteTime = false;
    stopButton = false;
    this->setFocusPolicy(Qt::StrongFocus);
    AllTime = 100;
    getReady = false;
    startTime = false;
    // 构建一个计时器
    QTimer *timer = new QTimer(this);
    // 将计时器的超时事件与 AnalogClock 的 update 函数绑定，即一旦计时器时间到了，就会调用一次 update 函数
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GameWindow::update));
    // 计时器开始计时，每次时间间隔 100 ms
    timer->start(50);
    // 设置窗口的标题
    setWindowTitle(tr("连连看"));
    // 设置 widget 大小
    resize(1000, 1000);
    //进行按钮图片的初始化
    MakeUp();
    timeOut1 = new QTimer(this);
    connect(timeOut1, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    timeOut1->start(1000);
    updateKey = new QTimer(this);
    connect(updateKey, SIGNAL(timeout()), this, SLOT(DealPressed()));
    connect(stop,SIGNAL(pressed()),this,SLOT(stopPslot()));
    connect(stop,SIGNAL(released()),this,SLOT(stopRslot()));
    connect(fileSave,SIGNAL(pressed()),this,SLOT(filesavePslot()));
    connect(fileSave,SIGNAL(released()),this,SLOT(filesaveRslot()));
    connect(conTinue,SIGNAL(pressed()),this,SLOT(continuePslot()));
    connect(conTinue,SIGNAL(released()),this,SLOT(continueRslot()));
}

//
//PaintMaP函数接受一个QPainter类型的变量的引用
//进行地图的绘制以及对hint特殊效果的处理
//
void GameWindow::PaintMaP(QPainter &painter)
{
    PowerJudge(hintPower);//判断hint效果是否有被触发
    if(hintPower.SWitchPower){
        HintPower(hintPo1,hintPo2);//将可以相消的两个点的坐标赋给hintPo1和hintPo2
    }
    else{
        hintPo1.x = -1;
        hintPo1.y = -1;
        hintPo2.x = -1;
        hintPo2.y = -1;
    }
    //对地图的绘制
    int Layer = 0;
    for(int i = 0;i <= m*n - 1; ++i){
        Layer = i / n;
        //对hint特殊效果选中的方块进行特殊绘制
        if(i / n == hintPo1.y && i % n == hintPo1.x && AllTime % 2 == 0){
            painter.setPen(Qt::red);
            painter.drawRect(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16);
            continue;
        }
        if(i / n == hintPo2.y && i % n == hintPo2.x && AllTime % 2 == 0){
            painter.setPen(Qt::red);
            painter.drawRect(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16);
            continue;
        }
        //对其余正常的方块进行特殊绘制
        if (gameMap[i / n][i % n] == 1){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p1);
            continue;
          }
        if (gameMap[i / n][i % n] == 2){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p2);
            continue;
          }
        if (gameMap[i / n][i % n] == 3){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p3);
            continue;
          }
        if (gameMap[i / n][i % n] == 4){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p4);
            continue;
          }
        if (gameMap[i / n][i % n] == 5){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p5);
            continue;
          }
        if (gameMap[i / n][i % n] == 6){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p6);
            continue;
          }
        if (gameMap[i / n][i % n] == 7){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p7);
            continue;
          }
        if (gameMap[i / n][i % n] == 8){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p8);
            continue;
          }
        if (gameMap[i / n][i % n] == 9){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p9);
            continue;
          }
        if (gameMap[i / n][i % n] == 10){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p10);
            continue;
          }
        if (gameMap[i / n][i % n] == 11){
            painter.drawPixmap(-n/2 * 16 + (i - Layer * n) * 16, -m/2 * 16 + Layer * 16,16,16,p11);
            continue;
          }
    }//地图初始化完成
}

//
//PaintFeedbackone函数接受一个QPainter类型的变量的引用
//处理玩家一选定的两个点，如果可以相消，则绘制连接的曲线
//
void GameWindow::PaintFeedbackone(QPainter &painter)
{
    //对选定反馈的书写
    if (sPlayer1.x != -1 && sPlayer1.y != -1){
        painter.drawRect(-n/2 * 16 + 16 * sPlayer1.x,-m/2 * 16 + 16 * sPlayer1.y,16,16);//激活效果的处理
    }
    if (sPlayer2.x != -1 && sPlayer2.y != -1){
        painter.setPen(Qt::blue);
        Point a,b;
        if(LineJudge(sPlayer1,sPlayer2,gameMap) && gameMap[sPlayer1.y][sPlayer1.x] == gameMap[sPlayer2.y][sPlayer2.x]){
            DealLine(P1,sPlayer1,sPlayer2);
            painter.drawLine(sPlayer1.x * 16 - n/2 * 16 + 8,sPlayer1.y * 16 - m/2 * 16 + 8,sPlayer2.x * 16 - n/2 * 16 + 8,sPlayer2.y * 16 - m/2 * 16 + 8);//绘制路径
            sPlayer1.x = -1;
            sPlayer1.y = -1;
            sPlayer2.x = -1;
            sPlayer2.y = -1;
        }
        else if(RectJudge(sPlayer1,sPlayer2,gameMap) && gameMap[sPlayer1.y][sPlayer1.x] == gameMap[sPlayer2.y][sPlayer2.x]){
            DealRect(P1,a,sPlayer1,sPlayer2);
            painter.setPen(Qt::blue);//绘制路径
            painter.drawLine(sPlayer1.x * 16 - n/2 * 16 + 8,sPlayer1.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            painter.drawLine(sPlayer2.x * 16 - n/2 * 16 + 8,sPlayer2.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            sPlayer1.x = -1;
            sPlayer1.y = -1;
            sPlayer2.x = -1;
            sPlayer2.y = -1;
        }
        else if(DoubleRectjudge(sPlayer1,sPlayer2,gameMap) && gameMap[sPlayer1.y][sPlayer1.x] == gameMap[sPlayer2.y][sPlayer2.x]){
            DealDourect(P1,a,b,sPlayer1,sPlayer2);
            painter.setPen(Qt::blue);//绘制路径
            painter.drawLine(sPlayer1.x * 16 - n/2 * 16 + 8,sPlayer1.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            painter.drawLine(a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8,b.x * 16 - n/2 * 16 + 8,b.y * 16 - m/2 * 16 + 8);
            painter.drawLine(sPlayer2.x * 16 - n/2 * 16 + 8,sPlayer2.y * 16 - m/2 * 16 + 8,b.x * 16 - n/2 * 16 + 8,b.y * 16 - m/2 * 16 + 8);
            sPlayer1.x = -1;
            sPlayer1.y = -1;
            sPlayer2.x = -1;
            sPlayer2.y = -1;
        }
        else{
            sPlayer1.x = -1;
            sPlayer1.y = -1;
            sPlayer2.x = -1;
            sPlayer2.y = -1;
        }
    }
}

//
//PaintFeedbacktwo函数接受一个QPainter类型的变量的引用
//处理玩家二选定的两个点，如果可以相消，则绘制连接的曲线
//
void GameWindow::PaintFeedbacktwo(QPainter &painter)
{
    //对选定反馈的书写
    if (sPlaytwo1.x != -1 && sPlaytwo1.y != -1){
        painter.drawRect(-n/2 * 16 + 16 * sPlaytwo1.x,-m/2 * 16 + 16 * sPlaytwo1.y,16,16);//激活效果的处理
    }
    if (sPlaytwo2.x != -1 && sPlaytwo2.y != -1){
        painter.setPen(Qt::blue);
        Point a,b;
        if(LineJudge(sPlaytwo1,sPlaytwo2,gameMap) && gameMap[sPlaytwo1.y][sPlaytwo1.x] == gameMap[sPlaytwo2.y][sPlaytwo2.x]){
            DealLine(P2,sPlaytwo1,sPlaytwo2);
            painter.drawLine(sPlaytwo1.x * 16 - n/2 * 16 + 8,sPlaytwo1.y * 16 - m/2 * 16 + 8,sPlaytwo2.x * 16 - n/2 * 16 + 8,sPlaytwo2.y * 16 - m/2 * 16 + 8);//绘制路径
            gameMap[sPlaytwo1.y][sPlaytwo1.x] = 0;
            gameMap[sPlaytwo2.y][sPlaytwo2.x] = 0;//消除后将对应的数组的值置零
            sPlaytwo1.x = -1;
            sPlaytwo1.y = -1;
            sPlaytwo2.x = -1;
            sPlaytwo2.y = -1;
        }
        else if(RectJudge(sPlaytwo1,sPlaytwo2,gameMap) && gameMap[sPlaytwo1.y][sPlaytwo1.x] == gameMap[sPlaytwo2.y][sPlaytwo2.x]){
            DealRect(P2,a,sPlaytwo1,sPlaytwo2);
            painter.setPen(Qt::blue);//绘制路径
            painter.drawLine(sPlaytwo1.x * 16 - n/2 * 16 + 8,sPlaytwo1.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            painter.drawLine(sPlaytwo2.x * 16 - n/2 * 16 + 8,sPlaytwo2.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            gameMap[sPlaytwo1.y][sPlaytwo1.x] = 0;
            gameMap[sPlaytwo2.y][sPlaytwo2.x] = 0;//消除后将对应的数组的值置零
            sPlaytwo1.x = -1;
            sPlaytwo1.y = -1;
            sPlaytwo2.x = -1;
            sPlaytwo2.y = -1;
        }
        else if(DoubleRectjudge(sPlaytwo1,sPlaytwo2,gameMap) && gameMap[sPlaytwo1.y][sPlaytwo1.x] == gameMap[sPlaytwo2.y][sPlaytwo2.x]){
            DealDourect(P2,a,b,sPlaytwo1,sPlaytwo2);
            painter.setPen(Qt::blue);//绘制路径
            painter.drawLine(sPlaytwo1.x * 16 - n/2 * 16 + 8,sPlaytwo1.y * 16 - m/2 * 16 + 8,a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8);
            painter.drawLine(a.x * 16 - n/2 * 16 + 8,a.y * 16 - m/2 * 16 + 8,b.x * 16 - n/2 * 16 + 8,b.y * 16 - m/2 * 16 + 8);
            painter.drawLine(sPlaytwo2.x * 16 - n/2 * 16 + 8,sPlaytwo2.y * 16 - m/2 * 16 + 8,b.x * 16 - n/2 * 16 + 8,b.y * 16 - m/2 * 16 + 8);
            gameMap[sPlaytwo1.y][sPlaytwo1.x] = 0;
            gameMap[sPlaytwo2.y][sPlaytwo2.x] = 0;//消除后将对应的数组的值置零
            sPlaytwo1.x = -1;
            sPlaytwo1.y = -1;
            sPlaytwo2.x = -1;
            sPlaytwo2.y = -1;
        }
        else{
            sPlaytwo1.x = -1;
            sPlaytwo1.y = -1;
            sPlaytwo2.x = -1;
            sPlaytwo2.y = -1;
        }
    }
}

//
//PaintPower函数接受一个QPainter类型的变量的引用
//根据特殊效果的位置信息绘制特殊效果的图标，并对特殊效果是否触发进行判定，并进行效果的处理
//
void GameWindow::PaintPower(QPainter &painter)
{
    for(int i = 0;i <= 2;++i){
        //根据模式和特殊效果的坐标决定是否绘制
        if(shufflePower.PowerPoint[i].x != -1 && shufflePower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + shufflePower.PowerPoint[i].x * 16, -m/2 * 16 + shufflePower.PowerPoint[i].y * 16,16,16,sk1);
        }
        if(TimePower.PowerPoint[i].x != -1 && TimePower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + TimePower.PowerPoint[i].x * 16, -m/2 * 16 + TimePower.PowerPoint[i].y * 16,16,16,sk2);
        }
        if(hintPower.PowerPoint[i].x != -1 && hintPower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + hintPower.PowerPoint[i].x * 16, -m/2 * 16 + hintPower.PowerPoint[i].y * 16,16,16,sk3);
        }
        if(getMode() == 1 && flashPower.PowerPoint[i].x != -1 && flashPower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + flashPower.PowerPoint[i].x * 16, -m/2 * 16 + flashPower.PowerPoint[i].y * 16,16,16,sk4);
        }
        if(getMode() == 2 && freezePower.PowerPoint[i].x != -1 && freezePower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + freezePower.PowerPoint[i].x * 16, -m/2 * 16 + freezePower.PowerPoint[i].y * 16,16,16,sk5);
        }
        if(getMode() == 2 && dizzyPower.PowerPoint[i].x != -1 && dizzyPower.PowerPoint[i].y != -1){
            painter.drawPixmap(-n/2 * 16 + dizzyPower.PowerPoint[i].x * 16, -m/2 * 16 + dizzyPower.PowerPoint[i].y * 16,16,16,sk6);
        }
    }
    //进行shuffle效果的触发判定，并进行效果的处理
    if(PowerJudge(shufflePower)){
        shufflePower.SWitchPower = false;
        rearrange(gameMap);
    }
    //进行TimePower效果的触发判定，并进行效果的处理
    if(PowerJudge(TimePower)){
        TimePower.SWitchPower = false;
        AllTime += 30;
    }
    //进行freezePower效果的触发判定
    PowerJudge(freezePower);
    //进行dizzyPower效果的触发判定
    PowerJudge(dizzyPower);
}

//
//DealMode函数不需要接受参数
//先进行模式的判定，如果flash效果触发，则根据点击位置的不同确定玩家的不同位置
//
void GameWindow::DealMode()
{
    if(this->getMode() == 1){
        PowerJudge(flashPower);
        if(flashPower.SWitchPower){
            //只考虑地图范围内的点击
            if(helpFlash.x >= 0 && helpFlash.y >= 0 && helpFlash.x <= n - 1 && helpFlash.y <= m - 1){
                int **s = helpArray(gameMap);
                if(s[helpFlash.y][helpFlash.x] == clabel){
                    //点击的位置人物可以到达
                    P1.lx = helpFlash.x;
                    P1.ly = helpFlash.y;
                }
                else if(s[helpFlash.y - 1][helpFlash.x] == clabel){
                    P1.lx = helpFlash.x;
                    P1.ly = helpFlash.y - 1;
                    if(sPlayer1.x == -1 && sPlayer1.y == -1){
                        sPlayer1.x = helpFlash.x;
                        sPlayer1.y = helpFlash.y;
                    }
                    else if(sPlayer1.x != helpFlash.x || sPlayer1.y != helpFlash.y){
                        sPlayer2.x = helpFlash.x;
                        sPlayer2.y = helpFlash.y;
                    }
                }
                else if(s[helpFlash.y + 1][helpFlash.x] == clabel){
                    P1.lx = helpFlash.x;
                    P1.ly = helpFlash.y + 1;
                    if(sPlayer1.x == -1 && sPlayer1.y == -1){
                        sPlayer1.x = helpFlash.x;
                        sPlayer1.y = helpFlash.y;
                    }
                    else if(sPlayer1.x != helpFlash.x || sPlayer1.y != helpFlash.y){
                        sPlayer2.x = helpFlash.x;
                        sPlayer2.y = helpFlash.y;
                    }
                }
                else if(s[helpFlash.y][helpFlash.x - 1] == clabel){
                    P1.lx = helpFlash.x - 1;
                    P1.ly = helpFlash.y;
                    if(sPlayer1.x == -1 && sPlayer1.y == -1){
                        sPlayer1.x = helpFlash.x;
                        sPlayer1.y = helpFlash.y;
                    }
                    else if(sPlayer1.x != helpFlash.x || sPlayer1.y != helpFlash.y){
                        sPlayer2.x = helpFlash.x;
                        sPlayer2.y = helpFlash.y;
                    }
                }
                else if(s[helpFlash.y][helpFlash.x + 1] == clabel){
                    P1.lx = helpFlash.x + 1;
                    P1.ly = helpFlash.y;
                    if(sPlayer1.x == -1 && sPlayer1.y == -1){
                        sPlayer1.x = helpFlash.x;
                        sPlayer1.y = helpFlash.y;
                    }
                    else if(sPlayer1.x != helpFlash.x || sPlayer1.y != helpFlash.y){
                        sPlayer2.x = helpFlash.x;
                        sPlayer2.y = helpFlash.y;
                    }
                }
                //对人物达到不了的位置进行处理
                helpFlash.x = -1;
                helpFlash.y = -1;
            }
        }
    }
}

//
//DealPower函数不需要接受参数
//进行地图的随机生成，并随机生成特殊效果的产生次数以及产生时间
//
void GameWindow::DealPower()
{
    if(getReady){
        //进行地图的随机生成
        MakeMaP();
        getReady = false;
        srand((unsigned)time(NULL));//设置随机数的种子
        //随机生成特殊效果的产生次数以及产生时间
        setPower(shufflePower,1,50,50);
        setPower(TimePower,2,0,100);
        setPower(hintPower,3,30,70);
        setPower(flashPower,4,80,20);
        setPower(freezePower,5,80,20);
        setPower(dizzyPower,6,80,20);
    }
}

//
//DrawBackground函数接受一个QPainter类型的变量的引用
//进行地图背景的绘制，以及游戏中相关信息的体现
//
void GameWindow::DrawBackground(QPainter &painter)
{
    //绘制游戏相关参数的大致框架
    QFont font("楷体",5,QFont::Bold,true);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(-100,-90,"剩余时间:  ");

    //进行剩余时间的绘制
    char* a = tranAlltime();
    painter.drawText(-50,-90,QString(a));
    if(getMode() == 2){
        //绘制文本文字
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(-20,-90,"P1: ");
        painter.drawText(40,-90,"P2: ");
    }
    else{
        //绘制文本文字
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(0,-90,"P1得分： ");
    }
    painter.drawRect(-n/2 * 16,-m/2 * 16,n * 16,m * 16);
    painter.translate(0,0);
    //根据特殊效果触发时间决定是否继续维持特殊效果
    if(AllTime <= hintcounter - 10)
        EndHint();
    if(AllTime <= flashcounter - 20)
        EndFlash();
    if(AllTime <= freezecounter - 3)
        EndFreeze();
    if(AllTime <= dizzycounter - 5)
        EndDizzy();
}

//
//DrawBackground函数接受一个QPainter类型的变量的引用
//进行分数在地图上的展现，并判断游戏是否结束
//
void GameWindow::DealGrade(QPainter &painter)
{
    if(EndJudge(gameMap)){
        //地图无解的情况的处理
        startTime = false;
        KeypressEvent.clear();
        emit EndGame();
    }
    if(AllTime == 0){
        //时间结束的情况的处理
        startTime = false;
        KeypressEvent.clear();
        emit TimeOut();
    }
    if(getMode() == 1){
        //分数的处理
        P1.translate();
        painter.drawText(50,-90,QString(P1.disPlay));
    }
    else{
        //分数的处理
        P1.translate();
        painter.drawText(0,-90,QString(P1.disPlay));
        P2.translate();
        painter.drawText(60,-90,QString(P2.disPlay));
    }

}

//
//paintEvent函数接受一个QPaintEvent类型的指针
//进行分数在地图上的展现，并判断游戏是否结束
//
void GameWindow::paintEvent(QPaintEvent *)
{
    DealPower();
    // 获取当前 widget 的宽度高度，取最小值作为表盘的边长
    int side = qMin(width(), height());
    // 基于该 widget 构建一个 painter，准备开始画图
    QPainter painter(this);
    // 设置 painter 的抗锯齿，让画出来的线更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置 painter 的坐标系装换
    painter.translate(0, 0);
    //加载背景图片
    QPixmap pix;
    pix.load(":/image/images/p11.jpg");//加载图片地址 可以是资源文件
    painter.drawPixmap(0, 0, width(), height(), pix);//绘制图片 横坐标、纵坐标、宽度、高度
    painter.setPen(Qt::black);  //边框色
    painter.drawRoundedRect(this->rect(), 1, 1); //圆角5像素
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    //绘制地图的相关背景以及相关参数(如剩余时间，得分等等)在地图中的展现
    DrawBackground(painter);
    //进行地图的绘制
    PaintMaP(painter);
    //进行人物的绘制
    painter.setPen(Qt::black);
    painter.drawPixmap(-n/2 * 16 + P1.lx * 16,-m/2 * 16 + P1.ly * 16,16,16,p12);
    if(this->getMode() == 2){
        painter.drawPixmap(-n/2 * 16 + P2.lx * 16,-m/2 * 16 + P2.ly * 16,16,16,p22);
    }
    //分别对玩家一和玩家二所激活的方块进行判定并反馈
    PaintFeedbackone(painter);
    PaintFeedbacktwo(painter);
    //对玩家的分数进行处理,并判断地图是否有解
    DealGrade(painter);
    //判断当前时间与特殊效果产生时间是否相同，相同则随机产生特殊效果的地址
    TimeJudge();
    //判断特殊效果出现了多长时间
    DisAppear();
    //进行特殊效果的绘制并判断是否被触发
    PaintPower(painter);
    //判断是否为单人模式，如果是，则处理flash功能
    DealMode();
}

//
//PowerJudge函数接受一个PowerMessage类型的引用
//判断特殊效果是否被触发，并记录触发玩家，返回特殊效果是否被触发对应的bool值
//
bool GameWindow::PowerJudge(PowerMessage &a)
{
    for (int  i = 0;i <= 2;++i) {
        //处理玩家一的情况
        if(P1.lx == a.PowerPoint[i].x && P1.ly == a.PowerPoint[i].y && a.PowerPoint[i].y != -1 && a.PowerPoint[i].x != -1){
            a.SWitchPower = true;
            a.User = 1;
            a.PowerPoint[i].x = -1;
            a.PowerPoint[i].y = -1;
            //根据特殊效果种类的不同选择不同的处理方式
            if(a.kind == 3){
                hintcounter = AllTime;
            }
            if(a.kind == 4){
                flashcounter = AllTime;
            }
            if(a.kind == 5){
                freezecounter = AllTime;
                freezePower.User = 1;
            }
            if(a.kind == 6){
                dizzycounter = AllTime;
                dizzyPower.User = 1;
            }
            return true;
        }
        //处理玩家二的情况
        else if(P2.lx == a.PowerPoint[i].x && P2.ly == a.PowerPoint[i].y && a.PowerPoint[i].y != -1 && a.PowerPoint[i].x != -1){
            a.SWitchPower = true;
            a.User = 2;
            a.PowerPoint[i].x = -1;
            a.PowerPoint[i].y = -1;
            //根据特殊效果种类的不同选择不同的处理方式
            if(a.kind == 3){
                hintcounter = AllTime;
            }
            if(a.kind == 5){
                freezecounter = AllTime;
                freezePower.User = 2;
            }
            if(a.kind == 6){
                dizzycounter = AllTime;
                dizzyPower.User = 2;
            }
            return true;
        }
    }
    return false;
}

//
//DealLine函数接受一个Player类型的引用，以及两个Point类型的变量
//处理直线相消的情况，并根据不同的图案增加不同的分值
//
bool GameWindow::DealLine(Player &m,Point a,Point b)
{
    if(LineJudge(a,b,gameMap)){
        if(gameMap[a.y][a.x] == 1){
            m.score += 50;
        }
        if(gameMap[a.y][a.x] >= 2 && gameMap[a.y][a.x] <= 9){
            m.score += 30;
        }
        if(gameMap[a.y][a.x] >= 10){
            m.score += 10;
        }
        gameMap[a.y][a.x] = 0;
        gameMap[b.y][b.x] = 0;//消除后将对应的数组的值置零
        return true;
    }
    else{
        return false;
    }
}

//
//DealRect函数接受一个Player类型的引用，以及三个Point类型的变量，其中一个点为引用
//处理拐一次弯相消的情况，并根据不同的图案增加不同的分值，将拐点坐标付给引用的变量
//
bool GameWindow::DealRect(Player &m,Point &a,Point b,Point c)
{
    //排除直线相消的情况
    if(LineJudge(c,b,gameMap))
        return false;
    if(RectJudge(b,c,gameMap)){
        //将拐点坐标付给引用的变量
        a.x = PointRect(b,c,gameMap).x;
        a.y = PointRect(b,c,gameMap).y;
        if(gameMap[b.y][b.x] == 1){
            m.score += 50;
        }
        if(gameMap[b.y][b.x] >= 2 && gameMap[b.y][b.x] <= 9){
            m.score += 30;
        }
        if(gameMap[b.y][b.x] >= 10){
            m.score += 10;
        }
        gameMap[c.y][c.x] = 0;
        gameMap[b.y][b.x] = 0;//消除后将对应的数组的值置零
        return true;
    }
    else
        return false;
}

//
//DealDourect函数接受一个Player类型的引用，以及四个Point类型的变量，其中两个点为引用
//处理拐两次弯相消的情况，并根据不同的图案增加不同的分值，将拐点坐标付给引用的变量
//
bool GameWindow::DealDourect(Player &a, Point &a1, Point &a2, Point b, Point c)
{
    //排除直线相消和拐一次弯的情况
    if(LineJudge(b,c,gameMap) || RectJudge(b,c,gameMap)){
        return false;
    }
    if(DoubleRectjudge(b,c,gameMap)){
        if(gameMap[b.y][b.x] == 1){
            a.score += 50;
        }
        if(gameMap[b.y][b.x] >= 2 && gameMap[b.y][b.x] <= 9){
            a.score += 30;
        }
        if(gameMap[b.y][b.x] >= 10){
            a.score += 10;
        }
        if(RightFind(a1,a2,b,c)){
            //向右寻路
            gameMap[c.y][c.x] = 0;
            gameMap[b.y][b.x] = 0;//消除后将对应的数组的值置零
            return true;
        }
        else if(LeftFind(a1,a2,b,c)){
            //向左寻路
            gameMap[c.y][c.x] = 0;
            gameMap[b.y][b.x] = 0;//消除后将对应的数组的值置零
            return true;
        }
        else if(UpFind(a1,a2,b,c)){
            //向上寻路
            gameMap[c.y][c.x] = 0;
            gameMap[b.y][b.x] = 0;//消除后将对应的数组的值置零
            return true;
        }
        else {
            //向下寻路
            DownFind(a1,a2,b,c);
            gameMap[b.y][b.x] = 0;
            gameMap[c.y][c.x] = 0;//消除后将对应的数组的值置零
            return true;
        }
    }
    else
        return false;
}

//
//RightFind函数接受四个Point类型的变量，其中两个点为引用
//a1从b点出发向右走，直到a1和c可以通过一次折线相消，将第二个转折点付给a2，返回true，否则返回false
//
bool GameWindow::RightFind(Point &a1,Point &a2,Point &b,Point &c)
{
    a1.x = b.x + 1;
    a1.y = b.y;
    while(true){
        if(a1.x <= n - 1 && gameMap[a1.y][a1.x] == 0){
            if(RectJudge(a1,c,gameMap)){
                //a1和c可以通过一次折线相消的情况
                a2.x = PointRect(a1,c,gameMap).x;
                a2.y = PointRect(a1,c,gameMap).y;
                return true;
            }
            else{
                //继续向右寻路
                a1.x++;
                if(a1.x > n - 1){
                    break;
                }
            }
        }
        else{
            return false;
        }
    }
    return false;
}

//
//LeftFind函数接受四个Point类型的变量，其中两个点为引用
//a1从b点出发左走，直到a1和c可以通过一次折线相消，将第二个转折点付给a2，返回true，否则返回false
//
bool GameWindow::LeftFind(Point &a1,Point &a2,Point &b,Point &c)
{
    a1.x = b.x - 1;
    a1.y = b.y;
    while(true){
        if(a1.x >= 0 && gameMap[a1.y][a1.x] == 0){
            if(RectJudge(a1,c,gameMap)){
                //a1和c可以通过一次折线相消的情况
                a2.x = PointRect(a1,c,gameMap).x;
                a2.y = PointRect(a1,c,gameMap).y;
                return true;
            }
            else{
                //继续向左寻路
                a1.x--;
                if(a1.x < 0){
                    break;
                }
            }
        }
        else{
            return false;
        }
    }
    return false;
}

//
//UpFind函数接受四个Point类型的变量，其中两个点为引用
//a1从b点出发向上走，直到a1和c可以通过一次折线相消，将第二个转折点付给a2，返回true，否则返回false
//
bool GameWindow::UpFind(Point &a1,Point &a2,Point &b,Point &c)
{
    a1.x = b.x;
    a1.y = b.y - 1;
    while(true){
        if(a1.y >= 0 && gameMap[a1.y][a1.x] == 0){
            if(RectJudge(a1,c,gameMap)){
                //a1和c可以通过一次折线相消的情况
                a2.x = PointRect(a1,c,gameMap).x;
                a2.y = PointRect(a1,c,gameMap).y;
                return true;
            }
            else{
                //继续向上寻路
                a1.y--;
                if(a1.y < 0){
                    break;
                }
            }
        }
        else{
            return false;
        }
    }
    return false;
}

//
//DownFind函数接受四个Point类型的变量，其中两个点为引用
//a1从b点出发向下走，直到a1和c可以通过一次折线相消，将第二个转折点付给a2，返回true，否则返回false
//
bool GameWindow::DownFind(Point &a1,Point &a2,Point &b,Point &c)
{
    a1.x = b.x;
    a1.y = b.y + 1;
    while(true){
        if(a1.y <= m - 1 && gameMap[a1.y][a1.x] == 0){
            if(RectJudge(a1,c,gameMap)){
                //a1和c可以通过一次折线相消的情况
                a2.x = PointRect(a1,c,gameMap).x;
                a2.y = PointRect(a1,c,gameMap).y;
                return true;
            }
            else{
                //继续向下寻路
                a1.y++;
                if(a1.y > m - 1){
                    break;
                }
            }
        }
        else{
            return false;
        }
    }
    return false;
}

//
//tranAlltime函数没有形式参数
//将记录分数的字符串格式化并返回对应的字符串
//
char *GameWindow::tranAlltime()
{
    int fir = AllTime / 100;
    int sec = (AllTime - fir * 100) / 10;
    int thi = AllTime % 10;
    char* a = new char[4];
    a[0] = fir + '0';
    a[1] = sec + '0';
    a[2] = thi + '0';
    a[3] = '\0';
    return a;
}

//
//~GameWindow函数没有形式参数
//归还申请的动态地址
//
GameWindow::~GameWindow()
{
    if(deleteTime){
        for(int i = 0;i < m;++i){
            delete [] gameMap[i];
        }
            delete [] gameMap;
    }
}

//
//keyPressEvent函数处理键盘事件的响应
//控制玩家一和玩家二的移动
//
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    //使用队列数据结构来存储用户触发的keyPressevent
    if(!event->isAutoRepeat()){
        KeypressEvent.insert(event->key());
    }
    if(!updateKey->isActive()){
        updateKey->start(50);
    }
}

//
//keyReleaseEvent函数处理键盘事件的响应
//控制玩家一和玩家二的移动
//
void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()){
        //如果用户没有一直按住按键，就弹出队列
        KeypressEvent.remove(event->key());
    }
    if(KeypressEvent.isEmpty()){
        updateKey->stop();
    }
}

//
//DealUp函数处理玩家向上移动事件
//根据玩家上面的位置的不同情况选择不同的处理方式
//
void GameWindow::DealUp(Player &a,Point &b,Point &c)
{
    if(a.ly <= 0) ;
    //已经在最上面则不响应
    else if(gameMap[a.ly - 1][a.lx] == 0){
        //上面是空位，则向上移动
        a.ly = (a.ly - 1);
    }
    else if(b.x != -1 && b.y != -1){
        //上面不是空位，则激活对应的方块
        c.x = a.lx;
        c.y = a.ly - 1;
        if(b.x == c.x && b.y == c.y){
            c.x = -1;
            c.y = -1;
        }
    }
    else{
        b.x = a.lx;
        b.y = a.ly - 1;
    }
}

//
//DealDown函数处理玩家向下移动事件
//根据玩家下面的位置的不同情况选择不同的处理方式
//
void GameWindow::DealDown(Player &a,Point &b,Point &c)
{
    if(a.ly >= m - 1) ;
    //已经在最下面则不响应
    else if(gameMap[a.ly + 1][a.lx] == 0){
        //下面是空位，则向下移动
        a.ly = (a.ly + 1);
    }
    else if(b.x != -1 && b.y != -1){
        //下面不是空位，则激活对应的方块
        c.x = a.lx;
        c.y = a.ly + 1;
        if(b.x == c.x && b.y == c.y){
            c.x = -1;
            c.y = -1;
        }
    }
    else{
        b.x = a.lx;
        b.y = a.ly + 1;
    }
}

//
//DealRight函数处理玩家向右移动事件
//根据玩家右面的位置的不同情况选择不同的处理方式
//
void GameWindow::DealRight(Player &a,Point &b,Point &c)
{
    if (a.lx >= n - 1);
    //已经在最右面则不响应
    else if(gameMap[a.ly][a.lx + 1] == 0){
        //右面是空位，则向右移动
        a.lx = (a.lx + 1);
    }
    else if(b.x != -1 && b.y != -1){
        //右面不是空位，则激活对应的方块
        c.x = a.lx + 1;
        c.y = a.ly;
        if(b.x == c.x && b.y == c.y){
            c.x = -1;
            c.y = -1;
        }
    }
    else{
        b.x = a.lx + 1;
        b.y = a.ly;
    }
}

//
//DealLeft函数处理玩家向左移动事件
//根据玩家左面的位置的不同情况选择不同的处理方式
//
void GameWindow::DealLeft(Player &a,Point &b,Point &c)
{
    if (a.lx <= 0);
    //已经在最左面则不响应
    else if(gameMap[a.ly][a.lx - 1] == 0){
        //左面是空位，则向左移动
        a.lx = (a.lx - 1);
    }
    else if(b.x != -1 && b.y != -1){
        //左面不是空位，则激活对应的方块
        c.x = a.lx - 1;
        c.y = a.ly;
        if(b.x == c.x && b.y == c.y){
            c.x = -1;
            c.y = -1;
        }
    }
    else{
        b.x = a.lx - 1;
        b.y = a.ly;
    }

}

//
//mousePressEvent函数处理鼠标事件的响应
//辅助实现flash特殊功能，获得鼠标点击的位置
//
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    if(flashPower.SWitchPower){
        if(event->button() == Qt::LeftButton){
            int x = event->x();
            int y = event->y();
            //进行坐标与位置间的转化
            helpFlash.x = (x - 100 - (11 - n) / 2 * 80) / 80;
            helpFlash.y = (y - 100 - (11 - m) / 2 * 80) / 80;
        }
    }
}

//
//setRaC函数接受两个整型常量
//初始化行数和列数
//
void GameWindow::setRaC(const int a,const int b){
    m = a;
    n = b;
    getReady  =true;//用于随机初始化地图判定的bool型变量
}

//
//LineJudge函数接受两个点的坐标（a和b）以及指针数组的地址
//返回两个点是否可以通过直线相连所对应的bool值
//
bool GameWindow::LineJudge(const Point a,const Point b,int **s)
{
    //讨论两个点x坐标相同的情况
    if(a.x == b.x){
        int min,max;
        if(a.y >=  b.y) {
            min = b.y;
            max = a.y;
        }
        else {
            min = a.y;
            max = b.y;
        }
        for (int i = min + 1;i <= max - 1;++i){
            if (s[i][a.x] != 0)
                return false;
        }
        return true;
    }
    //讨论两个点y坐标相同的情况
    else if(a.y == b.y){
        int min,max;
        if(a.x >=  b.x) {
            min = b.x;
            max = a.x;
        }
        else {
            min = a.x;
            max = b.x;
        }
        for (int i = min + 1;i <= max - 1;++i){
            if (s[a.y][i] != 0)
                return false;
        }
        return true;
    }
    //对其余情况的讨论
    else return false;
}

//
//RectJudge函数接受两个点的坐标（a和b）以及指针数组的地址
//返回两个点是否可以通过一次折线相连所对应的bool值
//
bool GameWindow::RectJudge(const Point a,const Point b,int **s){
    Point test1,test2;
    test1.x = a.x;
    test1.y = b.y;
    test2.x = b.x;
    test2.y = a.y;
    if (LineJudge(a,test1,s) && LineJudge(test1,b,s) && s[test1.y][test1.x] == 0)
        return true;
    else if(LineJudge(a,test2,s) && LineJudge(test2,b,s) && s[test2.y][test2.x] == 0)
        return true;
    else return false;
}

//
//PointRect函数接受两个点的坐标（a和b）以及指针数组的地址
//返回在RectJudge中判断的转折点的坐标，用于画路径
//
Point GameWindow::PointRect(const Point a,const Point b,int **s){
    Point Rpoint;
    Rpoint.x = a.x;
    Rpoint.y = b.y;
    if(LineJudge(Rpoint,a,s) && LineJudge(b,Rpoint,s) && s[Rpoint.y][Rpoint.x] == 0){
        //辅助使用LinJudge进行单拐点的判断
        return Rpoint;
    }
    else{
        Rpoint.x = b.x;
        Rpoint.y = a.y;
        return Rpoint;
    }
}

//
//helpArray函数接受指针数组的地址
//返回一个新的指针数组，其中该数组对连连看中人物所能到达的位置进行标记
//
int **GameWindow::helpArray(int **s1){
    int **s = new int*[m];
    for (int i = 0;i < m;i++)
            s[i]=new int[n];
    for(int i = 0;i <= n - 1;++i){
        s[0][i] = clabel;
        s[m - 1][i] = clabel;
    }
    for(int i = 0;i <= m - 1;++i){
        s[i][0] = clabel;
        s[i][n - 1] = clabel;
    }
    //先进行横向的判定
    for(int i = 1;i <= (m - 1)/2;++i){
        for(int j = i;j <= n - 1 - i;++j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
        //反向验证
        for(int j = n - 1 - i;j >= i;--j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
    }
    //再进行纵向的判定
    for(int i = 1;i <= (n - 1)/2;++i){
        for(int j = i;j <= m - 1 - i;++j){
            if(s1[j][i] == 0){
                if(s[j - 1][i] == clabel || s[j + 1][i] == clabel ||s[j][i - 1] == clabel || s[j][i + 1] == clabel){
                    s[j][i] = clabel;
                }
            }
            if(s1[j][n - 1 - i] == 0){
                if(s[j][n - i] == clabel || s[j][n - 2 - i] == clabel ||s[1 + j][n - 1 - i] == clabel || s[j - 1][n - 1 - i] == clabel){
                    s[j][n - 1 - i] = clabel;
                }
            }
        }
        //反向验证
        for(int j = m - 1 - i;j >= i;--j){
            if(s1[j][i] == 0){
                if(s[j - 1][i] == clabel || s[j + 1][i] == clabel ||s[j][i - 1] == clabel || s[j][i + 1] == clabel){
                    s[j][i] = clabel;
                }
            }
            if(s1[j][n - 1 - i] == 0){
                if(s[j][n - i] == clabel || s[j][n - 2 - i] == clabel ||s[1 + j][n - 1 - i] == clabel || s[j - 1][n - 1 - i] == clabel){
                    s[j][n - 1 - i] = clabel;
                }
            }
        }
    }
    //再进行横向的判定
    for(int i = 1;i <= (m - 1)/2;++i){
        for(int j = i;j <= n - 1 - i;++j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
        //反向验证
        for(int j = n - 1 - i;j >= i;--j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
    }
    //由内层出发进行判定
    for(int i = (m - 1)/2;i >= 1;--i){
        for(int j = i;j <= n - 1 - i;++j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
        //反向验证
        for(int j = n - 1 - i;j >= i;--j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
    }
    for(int i = (n - 1)/2;i >= 1;--i){
        for(int j = i;j <= m - 1 - i;++j){
            if(s1[j][i] == 0){
                if(s[j - 1][i] == clabel || s[j + 1][i] == clabel ||s[j][i - 1] == clabel || s[j][i + 1] == clabel){
                    s[j][i] = clabel;
                }
        }
            if(s1[j][n - 1 - i] == 0){
                if(s[j][n - i] == clabel || s[j][n - 2 - i] == clabel ||s[1 + j][n - 1 - i] == clabel || s[j - 1][n - 1 - i] == clabel){
                    s[j][n - 1 - i] = clabel;
                }
            }
        }
        //反向验证
        for(int j = m - 1 - i;j >= i;--j){
            if(s1[j][i] == 0){
                if(s[j - 1][i] == clabel || s[j + 1][i] == clabel ||s[j][i - 1] == clabel || s[j][i + 1] == clabel){
                    s[j][i] = clabel;
                }
            }
            if(s1[j][n - 1 - i] == 0){
                if(s[j][n - i] == clabel || s[j][n - 2 - i] == clabel ||s[1 + j][n - 1 - i] == clabel || s[j - 1][n - 1 - i] == clabel){
                    s[j][n - 1 - i] = clabel;
                }
            }
        }
    }
    for(int i = (m - 1)/2;i >= 1;--i){
        for(int j = i;j <= n - 1 - i;++j){
            if(s1[i][j] == 0){
               if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
        //反向验证
        for(int j = n - 1 - i;j >= i;--j){
            if(s1[i][j] == 0){
                if(s[i - 1][j] == clabel || s[i + 1][j] == clabel ||s[i][j - 1] == clabel || s[i][j + 1] == clabel){
                    s[i][j] = clabel;
                }
            }
            if(s1[m - 1 - i][j] == 0){
                if(s[m - 2 - i][j] == clabel || s[m - i][j] == clabel ||s[m - 1 - i][j - 1] == clabel || s[m - 1 - i][j + 1] == clabel){
                    s[m - 1 - i][j] = clabel;
                }
            }
        }
    }
    return s;
}

//
//DoubleRectjudge函数接受两个点的坐标（a和b）以及指针数组的地址
//返回两个点是否可以通过两次以内的折线相连所对应的bool值
//
bool GameWindow::DoubleRectjudge(const Point a,const Point b,int **s){
    if (LineJudge(a,b,s)){
        //两个点可通过直线相连的情况
        return true;
    }
    if (RectJudge(a,b,s)){
        //两个点可通过一次折线相连的情况
        return true;
    }
    int x1 = a.x,y1 = a.y;
    Point test;
    while(s[y1 + 1][x1] == 0 && y1 <= m - 2){
        test.x = x1;
        test.y = y1 + 1;
        if(RectJudge(test,b,s))
            return true;
        y1++;
        if(y1 == m - 1) break;
    }//向下寻路
    x1 = a.x,y1 = a.y;
    while(s[y1 - 1][x1] == 0 && y1 >= 1){
        test.x = x1;
        test.y = y1 - 1;
        if(RectJudge(test,b,s))
            return true;
        y1--;
        if(y1 == 0) break;
    }//向上寻路
    x1 = a.x,y1 = a.y;
    while(s[y1][x1 + 1] == 0 && x1 <= n - 2){
        test.x = x1 + 1;
        test.y = y1;
        if(RectJudge(test,b,s))
            return true;
        x1++;
        if(x1 == n - 1) break;
    }//向右寻路
    x1 = a.x,y1 = a.y;
    while(s[y1][x1 - 1] == 0 && x1 >= 1){
        test.x = x1 - 1;
        test.y = y1;
        if(RectJudge(test,b,s))
            return true;
        x1--;
        if(x1 == 0) break;
    }//向左寻路
    return false;
}

//
//EndJudge函数接受指针数组的地址
//返回该数组对应的地图是否还有解
//
bool GameWindow::EndJudge(int **s1){
    int **s = helpArray(s1);
    //用于统计相同点的坐标
    Point matchPoint[12][20];
    //将相同的坐标放在同一维数组里
    for(int i = 0;i <= m*n - 1;++i){
        if(s1[i / n][i % n] != 0){
            if(s[i / n - 1][i % n] == clabel || s[i / n + 1][i % n] == clabel || s[i / n][i % n - 1] == clabel || s[i / n][i % n + 1] == clabel){
                int exam = s1[i / n][i % n];
                for(int j = 0;j <= 19;++j){
                    if(matchPoint[exam][j].x == -1 && matchPoint[exam][j].y == -1){
                        matchPoint[exam][j].x = i % n;
                        matchPoint[exam][j].y = i / n;
                        break;
                    }

                }
            }
        }
    }
    //对人物所能到达的每种类型的方块进行一个遍历，检查是否能够消除
    for(int i = 1;i <= 11;++i){
        for(int j = 0;j <= 19;++j){
            if(matchPoint[i][j].x == -1 && matchPoint[i][j].y == -1){
                break;
            }
            for(int m = j+1;m <= 19;++m){
                if(matchPoint[i][m].x == -1 && matchPoint[i][m].y == -1){
                    break;
                }
                if(DoubleRectjudge(matchPoint[i][j],matchPoint[i][m],s1)){
                    return false;
                }
            }
        }
    }
    return true;
}

//
//setMode函数接受一个整型数
//根据得到的形参的值设置游戏模式
//
void GameWindow::setMode(int a)
{
    gameMode = a;
}

//
//swapRow函数接受指针数组的地址以及所要交换的行的编号
//在二维数组中交换所对应的行数
//
void GameWindow::swapRow(int **s,int a,int b){
    if(a >= m - 1 || a <= 0 || b >= m - 1 || b <= 0)
        return;
    if(a == b)
        return;
    for(int i = 1;i <= n - 2;++i){
        int tmp = s[a][i];
        s[a][i] = s[b][i];
        s[b][i] = tmp;
    }
}

//
//swapCol函数接受指针数组的地址以及所要交换的列的编号
//在二维数组中交换所对应的列数
//
void GameWindow::swapCol(int **s,int a,int b){
    if(a >= n - 1 || a <= 0 || b >= n - 1 || b <= 0)
        return;
    if(a == b)
        return;
    for(int i = 1;i <= m - 2;++i){
        int tmp = s[i][a];
        s[i][a] = s[i][b];
        s[i][b] = tmp;
    }
}

//
//rearrange函数接受指针数组的地址
//在二维数组中根据随机数，进行行和列的交换，实现地图重排的特殊效果
//
void GameWindow::rearrange(int **s)
{
    srand((unsigned)time(NULL));//设置随机数的种子
    //七十二变
    for(int i = 1;i <= 72;++i){
        if(rand() % 2 == 0){
            swapRow(s,1 + rand() % (m - 2),1 + rand() % (m - 2));
        }
        else
            swapCol(s,1 + rand() % (n - 2),1 + rand() % (n - 2));
    }
    sPlayer1.x = -1;
    sPlayer1.y = -1;
    sPlaytwo1.x = -1;
    sPlaytwo1.y = -1;
    if(gameMap[P1.ly][P1.lx] != 0){
        int count = n + 1;
        while(gameMap[count / n][count % n] != 0){
            count++;
            if((count + 1) % n == 0)
                count++;
        }
        int tmp = gameMap[P1.ly][P1.lx];
        gameMap[P1.ly][P1.lx] = gameMap[count / n][count % n];
        gameMap[count / n][count % n] = tmp;
    }
    if(gameMode == 2){
        if(gameMap[P2.ly][P2.lx] != 0){
            int count = n + 1;
            while(gameMap[count / n][count % n] != 0){
                count++;
                if((count + 1) % n == 0)
                    count++;
            }
            int tmp = gameMap[P2.ly][P2.lx];
            gameMap[P2.ly][P2.lx] = gameMap[count / n][count % n];
            gameMap[count / n][count % n] = tmp;
        }
    }
}

//
//MakeMaP函数没有形式参数
//随机生成玩家的位置以及地图
//
void GameWindow::MakeMaP(){
    srand((unsigned)time(NULL));//设置随机数的种子
    gameMap = new int*[m];
    for(int i = 0;i < m;++i){
        gameMap[i] = new int[n];
    }
    //随机生成玩家的位置
    if(rand() % 2 == 0){
        P1.lx = rand() % n;
        P1.ly = 0;
    }
    else{
        P1.lx = 0;
        P1.ly = rand() % m;
    }
    if(gameMode == 2){
        if(rand() % 2 != 0){
            P2.lx = rand() % n;
            P2.ly = 0;
        }
        else{
            P2.lx = 0;
            P2.ly = rand() % m;
        }
    }
    //随机地图的生成
    for(int i = 0;i <= (m - 2)*(n - 2) / 2 - 1;++i){
        gameMap[i / (n - 2) + 1][i % (n - 2) + 1] = rand()%12;
        gameMap[m - i / (n - 2) - 2][n - 2 - i % (n - 2)] = gameMap[i / (n - 2) + 1][i % (n - 2) + 1];
    }
    for(int i = 0;i <= n - 1;++i){
        gameMap[0][i] = 0;
        gameMap[m - 1][i] = 0;
    }
    for(int i = 0;i <= m - 1;++i){
        gameMap[i][0] = 0;
        gameMap[i][n - 1] = 0;
    }
    rearrange(gameMap);
    //若生成的地图无解，则进行地图的重排
    while(EndJudge(gameMap)){
        rearrange(gameMap);
    }
    deleteTime = true;
}

//
//UpdateTime函数作为信号的响应函数
//随机生成玩家的位置以及地图
//
void GameWindow::UpdateTime()
{
    if(AllTime > 0 && startTime){
        //游戏还在进行的情况
        AllTime--;
    }
    if(AllTime <= 0){
        //游戏结束的情况
        AllTime = 0;
        startTime = false;
    }
}

//
//getMode函数没有形式参数
//返回游戏模式
//
int GameWindow::getMode()
{
    return gameMode;
}

//
//startSwitch函数没有形式参数
//是游戏开始运行
//
void GameWindow::startSwitch()
{
    startTime = true;
}

//
//P1Marks函数没有形式参数
//返回P1的格式化后的分数
//
char* GameWindow::P1Marks()
{
    P1.translate();
    return P1.disPlay;
}

//
//P2Marks函数没有形式参数
//返回P2的格式化后的分数
//
char* GameWindow::P2Marks()
{
    P2.translate();
    return P2.disPlay;
}

//
//CompareMarks1函数没有形式参数
//比较P1和P2的分数，P1高则返回true，否则返回false
//
bool GameWindow::CompareMarks1()
{
    return P1.score > P2.score;
}

//
//CompareMarks2函数没有形式参数
//比较P1和P2的分数，P2高则返回true，否则返回false
//
bool GameWindow::CompareMarks2()
{
    return P1.score < P2.score;
}

//
//setScoreatime函数没有形式参数
//将游戏的相关参数设置为初始状态
//
void GameWindow::setScoreatime()
{
    P1.score = 0;
    P2.score = 0;
    AllTime = 300;
}

//
//PowerMessage函数接受两个int类型的参数，一个bool类型的参数
//分别用于初始化kind和User以及SWitchPower，并初始化特殊效果出现的次数
//
GameWindow::PowerMessage::PowerMessage(int k,int a,bool z):kind(k),User(a),SWitchPower(z)
{
    PowTi = rand() % 3;
    for(int i = 0;i <= 2;++i){
        PowerTime[i] = 0;
        BeingTime[i] = false;
    }
}

//
//DisAppear函数没有形式参数
//对所有的特殊效果进行消失判断
//
void GameWindow::DisAppear()
{
    DisJudge(shufflePower);
    DisJudge(TimePower);
    DisJudge(hintPower);
    DisJudge(flashPower);
    DisJudge(freezePower);
    DisJudge(dizzyPower);
}

//
//DisJudge函数接受PowerMessage类型的引用
//如果特殊效果出现的时间超过10s没有被触发，则消失
//
void GameWindow::DisJudge(PowerMessage &a)
{
    for (int i = 0;i <= 2;++i) {
        if(a.PowerTime[i] >= AllTime + 10){
            a.PowerPoint[i].x = -1;
            a.PowerPoint[i].y = -1;
        }
    }
}

//
//TimeJudge函数没有形式参数
//判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
//
void GameWindow::TimeJudge()
{
    for(int i = 0;i <= shufflePower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == shufflePower.PowerTime[i] && !shufflePower.BeingTime[i]){
            shufflePower.BeingTime[i] = true;
            LockLoc(shufflePower,i);
         }
    }
    for(int i = 0;i <= TimePower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == TimePower.PowerTime[i] && !TimePower.BeingTime[i]){
            TimePower.BeingTime[i] = true;
            LockLoc(TimePower,i);
         }
    }
    for(int i = 0;i <= flashPower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == flashPower.PowerTime[i] && !flashPower.BeingTime[i]){
            flashPower.BeingTime[i] = true;
            LockLoc(flashPower,i);
         }
    }
    for(int i = 0;i <= hintPower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == hintPower.PowerTime[i] && !hintPower.BeingTime[i]){
            hintPower.BeingTime[i] = true;
            LockLoc(hintPower,i);
         }
    }
    for(int i = 0;i <= freezePower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == freezePower.PowerTime[i] && !freezePower.BeingTime[i]){
            freezePower.BeingTime[i] = true;
            LockLoc(freezePower,i);
         }
    }
    for(int i = 0;i <= dizzyPower.PowTi;++i){
        //判断当前的时间是否是特殊效果出现的时间，如果是，则随机生成特殊效果的位置
        if(AllTime == dizzyPower.PowerTime[i] && !dizzyPower.BeingTime[i]){
            dizzyPower.BeingTime[i] = true;
            LockLoc(dizzyPower,i);
         }
    }
}

//
//LockLoc函数接受一个PowerMessage类型的引用和一个整型数
//随机生成特殊效果的位置，并进行判定后放在对应的位置(保证位置玩家能够到达)
//
void GameWindow::LockLoc(PowerMessage &a,int i)
{
    //随机生成重排特殊效果在地图上出现的位置
    int location = rand() % m*n;
    //获取玩家能够到达的位置
    int **s = helpArray(gameMap);
    for(int j = location - 1;j <= m*n - 1;++j){
       if(s[j / n][j % n] == clabel && a.kind != 1){
           a.PowerPoint[i].x = j % n;
           a.PowerPoint[i].y = j / n;
           break;
           }
       else if(s[j / n][j % n] == clabel){
           a.PowerPoint[i].x = n - 1;
           a.PowerPoint[i].y = j / n;
           break;
       }
       }
    for(int i = 0;i <= m - 1;++i){
        delete [] s[i];
    }
    delete [] s;

}

//
//hintPower函数接受指针数组的地址,引进两个输出参数
//找出玩家能够到达的位置并且能够相消的两个坐标赋给a,b
//
void GameWindow::HintPower(Point &a,Point &b){
    int **s = helpArray(gameMap);
    Point matchPoint[12][20];//用于统计相同点的坐标
    for(int i = 0;i <= m*n - 1;++i){
        if(gameMap[i / n][i % n] != 0){
            if(s[i / n - 1][i % n] == clabel || s[i / n + 1][i % n] == clabel || s[i / n][i % n - 1] == clabel || s[i / n][i % n + 1] == clabel){
                int exam = gameMap[i / n][i % n];
                for(int j = 0;j <= 19;++j){
                    if(matchPoint[exam][j].x == -1 && matchPoint[exam][j].y == -1){
                        matchPoint[exam][j].x = i % n;
                        matchPoint[exam][j].y = i / n;
                        break;
                    }

                }
            }
        }
    }
    //对人物所能到达的每种类型的方块进行一个遍历，检查是否能够消除
    for(int i = 1;i <= 11;++i){
        for(int j = 0;j <= 19;++j){
            if(matchPoint[i][j].x == -1 && matchPoint[i][j].y == -1){
                break;
            }
            for(int m = j+1;m <= 19;++m){
                if(matchPoint[i][m].x == -1 && matchPoint[i][m].y == -1){
                    break;
                }
                if(DoubleRectjudge(matchPoint[i][j],matchPoint[i][m],gameMap)){
                    //如果能够消除，就将对应的坐标分别赋给a,b
                    a.x = matchPoint[i][j].x;
                    a.y = matchPoint[i][j].y;
                    b.x = matchPoint[i][m].x;
                    b.y = matchPoint[i][m].y;
                    for(int i = 0;i <= m - 1;++i){
                        delete [] s[i];
                    }
                    delete [] s;
                    return;
                }
            }
        }
    }
}

//
//EndHint函数没有形式参数
//将特殊效果hint的“开关”关闭
//
void GameWindow::EndHint()
{
    hintPower.SWitchPower = false;
    hintcounter = 0;
}

//
//EndFlash函数没有形式参数
//将特殊效果flash的“开关”关闭
//
void GameWindow::EndFlash()
{
    flashPower.SWitchPower = false;
    flashcounter = 0;
}

//
//EndFreeze函数没有形式参数
//将特殊效果Freeze的“开关”关闭
//
void GameWindow::EndFreeze()
{
    freezePower.SWitchPower = false;
    freezePower.User = 3;
    freezecounter = 0;
}

//
//EndDizzy函数没有形式参数
//将特殊效果Dizzy的“开关”关闭
//
void GameWindow::EndDizzy()
{
    dizzyPower.SWitchPower = false;
    dizzyPower.User = 3;
    dizzycounter = 0;
}

//
//stopPslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果
//
void GameWindow::stopPslot()
{
    list.replace(5,"font:bold 35px");//改变字体的大小参数
    stop->setStyleSheet(list.join(';'));
}

//
//stopRslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果，并出现新的选项继续游戏或者存档
//
void GameWindow::stopRslot()
{
    list.replace(5,"font:bold 30px");//改变字体的大小参数
    stop->setStyleSheet(list.join(';'));
    stop->hide();
    conTinue->show();
    fileSave->show();
    timeOut1->stop();
    stopButton = true;
}

//
//filesavePslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果
//
void GameWindow::filesavePslot()
{
    list.replace(5,"font:bold 35px");//改变字体的大小参数
    fileSave->setStyleSheet(list.join(';'));
}

//
//filesaveRslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果
//
void GameWindow::filesaveRslot()
{
    list.replace(5,"font:bold 30px");//改变字体的大小参数
    fileSave->setStyleSheet(list.join(';'));
    //调用相关函数实现存档功能
    SaveinFile();
}

//
//continuePslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果
//
void GameWindow::continuePslot()
{
    list.replace(5,"font:bold 35px");//改变字体的大小参数
    conTinue->setStyleSheet(list.join(';'));
}

//
//continueRslot函数没有形式参数，是信号的响应函数
//调节按钮的参数，优化鼠标点击的效果，将保存游戏和载入游戏的选项hide，将暂停游戏按钮展现
//
void GameWindow::continueRslot()
{
    list.replace(5,"font:bold 30px");//改变字体的大小参数
    conTinue->setStyleSheet(list.join(';'));
    conTinue->hide();
    fileSave->hide();
    stop->show();
    timeOut1->start();
    stopButton = false;
}

//
//SaveinFile函数没有形式参数
//将游戏中的相关参数以格式化的形式写入到磁盘中
//
void GameWindow::SaveinFile()
{
    ofstream fileSave("Save");
    getReady = false;
    fileSave.write(reinterpret_cast<const char * >(&gameMode),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&n),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&m),sizeof (int));
    for(int i = 0;i <= m*n - 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&gameMap[i / n][i % n]),sizeof (int));
    }
    fileSave.write(reinterpret_cast<const char * >(&AllTime),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P1.lx),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P1.ly),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P1.score),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P2.lx),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P2.ly),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&P2.score),sizeof (int));
    for(int i = 0;i <= 2;++i){
        fileSave.write(reinterpret_cast<const char * >(&shufflePower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&TimePower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&hintPower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&flashPower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 2;++i){
        fileSave.write(reinterpret_cast<const char * >(&shufflePower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&TimePower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&hintPower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileSave.write(reinterpret_cast<const char * >(&flashPower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 2;++i){
        fileSave.write(reinterpret_cast<const char * >(&shufflePower.PowerPoint[i].x),sizeof (int));
        fileSave.write(reinterpret_cast<const char * >(&shufflePower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileSave.write(reinterpret_cast<const char * >(&TimePower.PowerPoint[i].x),sizeof (int));
        fileSave.write(reinterpret_cast<const char * >(&TimePower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileSave.write(reinterpret_cast<const char * >(&hintPower.PowerPoint[i].x),sizeof (int));
        fileSave.write(reinterpret_cast<const char * >(&hintPower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileSave.write(reinterpret_cast<const char * >(&flashPower.PowerPoint[i].x),sizeof (int));
        fileSave.write(reinterpret_cast<const char * >(&flashPower.PowerPoint[i].y),sizeof (int));
    }
    fileSave.write(reinterpret_cast<const char * >(&hintcounter),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&flashcounter),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&freezecounter),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&dizzycounter),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&shufflePower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&TimePower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&hintPower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&flashPower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&freezePower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&dizzyPower.User),sizeof (int));
    fileSave.write(reinterpret_cast<const char * >(&shufflePower.SWitchPower),sizeof (bool));
    fileSave.write(reinterpret_cast<const char * >(&TimePower.SWitchPower),sizeof (bool));
    fileSave.write(reinterpret_cast<const char * >(&hintPower.SWitchPower),sizeof (bool));
    fileSave.write(reinterpret_cast<const char * >(&flashPower.SWitchPower),sizeof (bool));
    fileSave.write(reinterpret_cast<const char * >(&freezePower.SWitchPower),sizeof (bool));
    fileSave.write(reinterpret_cast<const char * >(&dizzyPower.SWitchPower),sizeof (bool));
}

//
//ReadFile函数没有形式参数
//将游戏中的相关参数以格式化的形式从磁盘中读入
//
void GameWindow::ReadFile()
{
    ifstream fileRead("Save");
    fileRead.read(reinterpret_cast<char * >(&gameMode),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&n),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&m),sizeof (int));
    gameMap = new int*[m];
    for (int i = 0;i < m;++i) {
        gameMap[i] = new int[n];
    }
    deleteTime = true;
    for(int i = 0;i <= m*n - 1;++i){
        fileRead.read(reinterpret_cast<char * >(&gameMap[i / n][i % n]),sizeof (int));
    }
    startSwitch();
    fileRead.read(reinterpret_cast<char * >(&AllTime),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P1.lx),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P1.ly),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P1.score),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P2.lx),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P2.ly),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&P2.score),sizeof (int));
    for(int i = 0;i <= 2;++i){
        fileRead.read(reinterpret_cast<char * >(&shufflePower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&TimePower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&hintPower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&flashPower.PowerTime[i]),sizeof (int));
    }
    for(int i = 0;i <= 2;++i){
        fileRead.read(reinterpret_cast<char * >(&shufflePower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&TimePower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&hintPower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 1;++i){
        fileRead.read(reinterpret_cast<char * >(&flashPower.BeingTime[i]),sizeof (bool));
    }
    for(int i = 0;i <= 2;++i){
        fileRead.read(reinterpret_cast<char * >(&shufflePower.PowerPoint[i].x),sizeof (int));
        fileRead.read(reinterpret_cast<char * >(&shufflePower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileRead.read(reinterpret_cast<char * >(&TimePower.PowerPoint[i].x),sizeof (int));
        fileRead.read(reinterpret_cast<char * >(&TimePower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileRead.read(reinterpret_cast<char * >(&hintPower.PowerPoint[i].x),sizeof (int));
        fileRead.read(reinterpret_cast<char * >(&hintPower.PowerPoint[i].y),sizeof (int));
    }
    for(int i = 0;i <=1;++i){
        fileRead.read(reinterpret_cast<char * >(&flashPower.PowerPoint[i].x),sizeof (int));
        fileRead.read(reinterpret_cast<char * >(&flashPower.PowerPoint[i].y),sizeof (int));
    }
    fileRead.read(reinterpret_cast<char * >(&hintcounter),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&flashcounter),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&freezecounter),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&dizzycounter),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&shufflePower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&TimePower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&hintPower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&flashPower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&freezePower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&dizzyPower.User),sizeof (int));
    fileRead.read(reinterpret_cast<char * >(&shufflePower.SWitchPower),sizeof (bool));
    fileRead.read(reinterpret_cast<char * >(&TimePower.SWitchPower),sizeof (bool));
    fileRead.read(reinterpret_cast<char * >(&hintPower.SWitchPower),sizeof (bool));
    fileRead.read(reinterpret_cast<char * >(&flashPower.SWitchPower),sizeof (bool));
    fileRead.read(reinterpret_cast<char * >(&freezePower.SWitchPower),sizeof (bool));
    fileRead.read(reinterpret_cast<char * >(&dizzyPower.SWitchPower),sizeof (bool));
}

//
//setZero函数没有形式参数
//将游戏中的相关参数重新初始化，为下一轮游戏做准备
//
void GameWindow::setZero()
{
    srand((unsigned)time(NULL));
    P1.score = 0;
    P2.score = 0;
    AllTime = 100;
    Zero(shufflePower);
    Zero(TimePower);
    Zero(hintPower);
    Zero(flashPower);
    Zero(freezePower);
    Zero(dizzyPower);
    setPower(shufflePower,1,50,50);
    setPower(TimePower,2,0,100);
    setPower(hintPower,3,30,70);
    setPower(flashPower,4,80,20);
    setPower(freezePower,5,80,20);
    setPower(dizzyPower,6,80,20);
    if(rand() % 2 == 0){
        P1.lx = rand() % n;
        P1.ly = 0;
    }
    else{
        P1.ly = rand() % m;
        P1.lx = 0;
    }
    if(rand() % 2 == 0){
        P2.lx = rand() % n;
        P2.ly = 0;
    }
    else{
        P2.ly = rand() % m;
        P2.lx = 0;
    }
}

//
//Zero函数接受一个PowerMessage类型的引用
//将接受到的变量的各项指标初始化
//
void GameWindow::Zero(PowerMessage &a)
{
    for (int i = 0;i <= 2;++i) {
        a.BeingTime[i] = false;
        a.PowerTime[i] = 0;
        a.SWitchPower = false;
        a.PowerPoint[i].x = -1;
        a.PowerPoint[i].y = -1;
    }
    a.User = 3;
}

//
//setPower函数接受一个PowerMessage类型的引用,三个int类型的形式参数
//初始化特殊效果的生成时间
//
void GameWindow::setPower(PowerMessage &m,int a,int b,int c)
{
    m.kind = a;
    for(int i = 0;i <= m.PowTi;++i){
        m.PowerTime[i] = b + rand() % c;
    }
}

//
//DealPressed函数没有形式参数
//处理存储在按键队列的各个按键事件
//
void GameWindow::DealPressed()
{
    if(!KeypressEvent.isEmpty()){
        for(int key:KeypressEvent){
            switch (key) {
            case Qt::Key_W:
                if (freezePower.User != 2 && !stopButton){
                        if(dizzyPower.User == 2 && dizzyPower.SWitchPower == true){
                            DealDown(P1,sPlayer1,sPlayer2);
                        }
                        else{
                            DealUp(P1,sPlayer1,sPlayer2);
                        }
                    }
                break;
            case Qt::Key_S:
                if (freezePower.User != 2 && !stopButton){
                        if(dizzyPower.User == 2 && dizzyPower.SWitchPower == true){
                            DealUp(P1,sPlayer1,sPlayer2);
                        }
                        else{
                            DealDown(P1,sPlayer1,sPlayer2);
                        }
                    }
                break;
            case Qt::Key_A:
                if (freezePower.User != 2 && !stopButton){
                        if(dizzyPower.User == 2 && dizzyPower.SWitchPower == true){
                            DealRight(P1,sPlayer1,sPlayer2);
                        }
                        else{
                            DealLeft(P1,sPlayer1,sPlayer2);
                        }
                    }
                break;
            case Qt::Key_D:
                if (freezePower.User != 2 && !stopButton){
                        if(dizzyPower.User == 2 && dizzyPower.SWitchPower == true){
                            DealLeft(P1,sPlayer1,sPlayer2);
                        }
                        else{
                            DealRight(P1,sPlayer1,sPlayer2);
                        }
                    }
                break;
            case Qt::Key_Up:
                if (getMode() == 2 && freezePower.User != 1 && !stopButton){
                            if(dizzyPower.User == 1 && dizzyPower.SWitchPower == true){
                                DealDown(P2,sPlaytwo1,sPlaytwo2);
                            }
                            else{
                                DealUp(P2,sPlaytwo1,sPlaytwo2);
                            }
                        }
                break;
            case Qt::Key_Down:
                if (getMode() == 2 && freezePower.User != 1 && !stopButton){
                            if(dizzyPower.User == 1 && dizzyPower.SWitchPower == true){
                                DealUp(P2,sPlaytwo1,sPlaytwo2);
                            }
                            else{
                                DealDown(P2,sPlaytwo1,sPlaytwo2);
                            }
                        }
                break;
            case Qt::Key_Right:
                if (getMode() == 2 && freezePower.User != 1 && !stopButton){
                            if(dizzyPower.User == 1 && dizzyPower.SWitchPower == true){
                                DealLeft(P2,sPlaytwo1,sPlaytwo2);
                            }
                            else{
                                DealRight(P2,sPlaytwo1,sPlaytwo2);
                            }
                        }
                break;
            case Qt::Key_Left:
                if (getMode() == 2 && freezePower.User != 1 && !stopButton){
                            if(dizzyPower.User == 1 && dizzyPower.SWitchPower == true){
                                DealRight(P2,sPlaytwo1,sPlaytwo2);
                            }
                            else{
                                DealLeft(P2,sPlaytwo1,sPlaytwo2);
                            }
                        }
                break;
            }
        }
    }
}
