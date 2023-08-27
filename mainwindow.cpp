#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
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
#include <time.h>
#include <cmath>
#include <qwidget.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <QToolBar>

using namespace std;

//
//PrepareforMap函数没有形式参数
//用于设置按钮的各个参数
//
void MainWindow::PrepareforMap()
{
    //设置按钮的参数
    list.append("background-image:url(:/image/images/button.jpg)");   //设置背景图片
    list.append("border-style:outset");                               // 边框风格
    list.append("border-width:3px");                                  // 边框宽度
    list.append("border-color:rgb(10,45,110)");                       // 边框颜色
    list.append("border-radius:20px");                                // 边框倒角
    list.append("font:bold 30px");                                    // 字体
}

//
//PainterClocksecond函数接受一个QPainter类型的引用和一个QTime类型的变量
//用于画秒针和时钟
//
void MainWindow::PainterClocksecond(QPainter &painter, QTime a)
{
    //表示秒针
    QPoint secondHand[4] = {
        QPoint(3, 8),
        QPoint(3, -90),
        QPoint(-3, -90),
        QPoint(-3, 8)
    };
    // 为秒针分别准备一个颜色，RGB 格式
    QColor secondColor(127, 0, 0);
    painter.setPen(Qt::NoPen);
    // 此处打算画秒针，于是将 brush 设置为时针颜色
    painter.setBrush(secondColor);
    // 将此时此刻的 painter 保存下来，后面可以用 restore 恢复到此时的状态
    painter.save();
    // 根据当前的小时数和分钟数，旋转 painter 所看的坐标系
    painter.rotate(6.0 * (a.minute() * 60 + a.second()));
    // 根据给定的点，绘制出闭合多边形（三个点就是三角形）
    painter.drawConvexPolygon(secondHand, 4);
    // 恢复之前保存的状态（相当于把刚刚的坐标系旋转操作的效果去除了）
    painter.restore();


    // 设置 painter 所使用的 pen（可以认为是边框颜色，此处无边框）
    painter.setPen(Qt::NoPen);
}

//
//PainterClockminute函数接受一个QPainter类型的引用和一个QTime类型的变量
//用于画分针和时钟
//
void MainWindow::PainterClockminute(QPainter &painter,QTime a)
{
    // 同理表示分针，注意此处长度不一样
    QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    QColor minuteColor(0, 0, 127, 191);
    // 准备开始画分针了
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    // 将此时此刻的 painter 保存下来，后面可以用 restore 恢复到此时的状态
    painter.save();
    // 根据当前的小时数和分钟数，旋转 painter 所看的坐标系
    painter.rotate(6.0 * (a.minute() + a.second() / 60.0));
    // 根据给定的点，绘制出闭合多边形（三个点就是三角形）
    painter.drawConvexPolygon(minuteHand, 3);
    // 恢复之前保存的状态（相当于把刚刚的坐标系旋转操作的效果去除了）
    painter.restore();

    // 5 分钟刻度
    painter.setPen(minuteColor);
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

}

//
//PainterClockhour函数接受一个QPainter类型的引用和一个QTime类型的变量
//用于画时针和时钟
//
void MainWindow::PainterClockhour(QPainter &painter,QTime a)
{
    // 三个点，组成一个三角形，表示时针
    QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    QColor hourColor(127 , 0, 127);
    // 设置 painter 所使用的 pen（可以认为是边框颜色，此处无边框）
    painter.setPen(Qt::NoPen);
    // 设置 painter 所使用的 brush（可以认为是填充颜色）
    // 此处打算先画时针，于是将 brush 设置为时针颜色
    painter.setBrush(hourColor);

    // 将此时此刻的 painter 保存下来，后面可以用 restore 恢复到此时的状态
    painter.save();
    // 根据当前的小时数和分钟数，旋转 painter 所看的坐标系
    painter.rotate(30.0 * ((a.hour() + a.minute() / 60.0)));
    // 根据给定的点，绘制出闭合多边形（三个点就是三角形）
    painter.drawConvexPolygon(hourHand, 3);
    // 恢复之前保存的状态（相当于把刚刚的坐标系旋转操作的效果去除了）
    painter.restore();

    painter.setPen(hourColor);
    // 一个 12 个小时刻度，可以到
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        // 旋转，这个所以下次 drawLine 的参数没变，但是位置变了
        painter.rotate(30.0);
    }

}

//
//MakeUp函数没有形式参数
//构建三个按钮，并设置他们的风格，位置，文本以及大小
//
void MainWindow::MakeUp()
{    
    //设置按钮参数
    PrepareforMap();
    //构建新的按钮
    start = new QPushButton(this);
    conTinue = new QPushButton(this);
    Quit = new QPushButton(this);
    start->setStyleSheet(list.join(';'));
    conTinue->setStyleSheet(list.join(';'));
    Quit->setStyleSheet(list.join(';'));
    //设置按钮的文本
    start->setText("开始游戏");
    //设置按钮的大小
    start->resize(200,50);
    //设置按钮的位置
    start->move(width()/2 - 100,height()/2 + 50);
    //设置按钮的文本
    conTinue->setText("载入游戏");
    //设置按钮的大小
    conTinue->resize(200,50);
    //设置按钮的位置
    conTinue->move(width()/2 - 100,height()/2 + 150);
    //设置按钮的文本
    Quit->setText("退出游戏");
    //设置按钮的大小
    Quit->resize(200,50);
    //设置按钮的位置
    Quit->move(width()/2 - 100,height()/2 + 250);
}

//
//EndPainter函数接受一个QPainter类型的引用
//进行地图无解导致游戏结束后画面的绘制
//
void MainWindow::EndPainter(QPainter &painter)
{
    //设置画笔的字体
    QFont font("楷体",25,QFont::Bold,true);
    painter.setFont(font);
    //设置画笔的颜色
    painter.setPen(Qt::black);
    //绘制文本文字
    painter.setFont(font);
    //设置画笔的颜色
    painter.setPen(Qt::black);
    //单人模式下对最后结束页面的绘制
    if(gameWidget.getMode() == 1){
        painter.drawText(0,height() - 50,"最后的得分：");
        painter.drawText(300,height() - 50,QString(gameWidget.P1Marks()));
        painter.drawText(700,height() - 50,"地图无解");
    }
    //双人模式下对最后结束页面的绘制
    if(gameWidget.getMode() == 2){
        if(gameWidget.CompareMarks1()){
            //P1获胜的情况
            painter.drawText(0,height() - 50,"优胜者：P1--");
            painter.drawText(350,height() - 50,QString(gameWidget.P1Marks()));
            painter.drawText(700,height() - 50,"地图无解");
        }
        else if(gameWidget.CompareMarks2()){
            //P2获胜的情况
            painter.drawText(0,height() - 50,"优胜者：P2--");
            painter.drawText(350,height() - 50,QString(gameWidget.P2Marks()));
            painter.drawText(700,height() - 50,"地图无解");
        }
        else{
            //平局的情况
            painter.drawText(0,height() - 50,"平局：得分--");
            painter.drawText(350,height() - 50,QString(gameWidget.P1Marks()));
            painter.drawText(700,height() - 50,"地图无解");
        }
    }

}

//
//TimeOutpainter函数接受一个QPainter类型的引用
//进行时间到导致游戏结束后画面的绘制
//
void MainWindow::TimeOutpainter(QPainter &painter)
{
    //设置画笔的字体
    QFont font("楷体",25,QFont::Bold,true);
    painter.setFont(font);
    //设置画笔的颜色
    painter.setPen(Qt::black);
    //绘制文本文字
    painter.setFont(font);
    //设置画笔的颜色
    painter.setPen(Qt::black);
    //单人模式下对最后结束页面的绘制
    if(gameWidget.getMode() == 1){
        painter.drawText(0,height() - 50,"最后的得分：");
        painter.drawText(300,height() - 50,QString(gameWidget.P1Marks()));
        painter.drawText(700,height() - 50,"时间到");
    }
    //双人模式下对最后结束页面的绘制
    if(gameWidget.getMode() == 2){
        if(gameWidget.CompareMarks1()){
            //P1获胜的情况
            painter.drawText(0,height() - 50,"优胜者：P1--");
            painter.drawText(350,height() - 50,QString(gameWidget.P1Marks()));
            painter.drawText(700,height() - 50,"时间到");
        }
        else if(gameWidget.CompareMarks2()){
            //P2获胜的情况
            painter.drawText(0,height() - 50,"优胜者：P2--");
            painter.drawText(350,height() - 50,QString(gameWidget.P2Marks()));
            painter.drawText(700,height() - 50,"时间到");
        }
        else{
            //平局的情况
            painter.drawText(0,height() - 50,"平局：得分");
            painter.drawText(350,height() - 50,QString(gameWidget.P1Marks()));
            painter.drawText(700,height() - 50,"时间到");
        }
    }
}

//
//EndMovie函数没有形式参数
//终止游戏结束后庆祝画面的放映
//
void MainWindow::EndMovie()
{
    //结束动画的设置
    endCartoon->stop();
    //设置动画为不可见
    ui->label->setVisible(false);
    //设置按钮的文本
    start->setText("开始游戏");
    //设置按钮可见
    start->show();
    //设置按钮的文本
    conTinue->setText("载入游戏");
    //设置按钮可见
    conTinue->show();
    //设置按钮的文本
    Quit->setText("退出游戏");
    //设置按钮可见
    Quit->show();
    //对游戏界面的相关参数置零
    gameWidget.setZero();
    endGame = false;
}

//
//PaintClock函数接受一个QPainter类型的引用
//对于开始界面大背景的绘制
//
void MainWindow::PaintClock(QPainter &painter)
{
    QPixmap pix;
    painter.translate(0, 0);
    pix.load(":/image/images/p7.jpg");//加载图片地址 可以是资源文件
    painter.drawPixmap(0, 0, width(), height(), pix);//绘制图片 横坐标、纵坐标、宽度、高度
    painter.setPen(Qt::black);  //边框色
    painter.drawRoundedRect(this->rect(), 1, 1); //圆角5像素
    painter.translate(width() / 2, height() / 2);
}

//
//MainWindow函数是主窗口类的构造函数
//进行事件的绑定和开始界面中相关构件的创建
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    // 构建一个计时器
    QTimer *timer = new QTimer(this);
    // 将计时器的超时事件与 AnalogClock 的 update 函数绑定，即一旦计时器时间到了，就会调用一次 update 函数
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    // 计时器开始计时，每次时间间隔 100 ms
    timer->start(100);
    // 设置窗口的标题
    setWindowTitle(tr("连连看"));
    // 设置 widget 大小
    resize(1000, 1000);
    //构建三个按钮
    MakeUp();
    help = new QAction(tr("help"),this);
    help->setToolTip("help");
    help->setStatusTip("stop the game");
    help->setShortcut(Qt::Key::Key_P);
    connect(help,&QAction::triggered,this,&MainWindow::Rule);
    Load = new QAction(tr("load"),this);
    Load->setToolTip("load");
    Load->setStatusTip("load the old game");
    Load->setShortcut(Qt::Key::Key_R);
    connect(Load,&QAction::triggered,this,&MainWindow::conTinueS);
    QMenu *option = menuBar()->addMenu(tr("options"));
    option->addAction(help);
    option->addSeparator();
    option->addAction(Load);
    //利用信号与槽机制对于按钮和各个窗口类的行为进行绑定
    connect(start,SIGNAL(pressed()),this,SLOT(startSlot()));
    connect(conTinue,SIGNAL(pressed()),this,SLOT(conTinueSlot()));
    connect(Quit,SIGNAL(pressed()),this,SLOT(QuitSlot()));
    connect(start,SIGNAL(released()),this,SLOT(startRslot()));
    connect(conTinue,SIGNAL(released()),this,SLOT(conTinueRslot()));
    connect(Quit,SIGNAL(released()),this,SLOT(QuitRslot()));
    connect(&subWidget,SIGNAL(backSignal()),this,SLOT(backSlot()));
    connect(&subWidget,SIGNAL(SoloSignal()),this,SLOT(SoloSlot()));
    connect(&subWidget,SIGNAL(groupSignal()),this,SLOT(groupSlot()));
    connect(&setWidget,SIGNAL(ensureSignal()),this,SLOT(ensureSlot()));
    connect(&gameWidget,SIGNAL(EndGame()),this,SLOT(EndSlot()));
    connect(&gameWidget,SIGNAL(TimeOut()),this,SLOT(TimeoutSlot()));
    // 构建一个计时器
    time3 = new QTimer();
    //设置时间间隔
    time3->setInterval(5000);
    // 将计时器的超时事件与 AnalogClock 的 update 函数绑定，即一旦计时器时间到了，就会调用一次 update 函数
    connect(time3, &QTimer::timeout, this, QOverload<>::of(&MainWindow::timeOut1));
    //设置动画的大小，放映速度以及位置
    endCartoon = new QMovie(":/image/images/endcar.gif");
    endCartoon->setSpeed(75);
    QSize size(width(),height());
    endCartoon->setScaledSize(size);
    ui->label->setGeometry(0,0,width(),height() - 100);
    ui->label->setMovie(endCartoon);
    ui->label->show();
}
// MainWindow 这个 widget 的绘制事件，在 update 被调用时，此函数会被调用

//
//paintEvent函数接受一个QPaintEventr类型的指针
//对于开始界面大背景的绘制
//
void MainWindow::paintEvent(QPaintEvent *)
{
    //开始界面的情况
    if(!endGame){
        QColor circleColor(0, 127, 0);
        // 获取当前 widget 的宽度高度，取最小值作为表盘的边长
        int side = qMin(width(), height());
        // 获取当前时间，后面我们将根据该时间，确定时针和分针的指向
        QTime time;
        time = QTime::currentTime();
        // 基于该 widget 构建一个 painter，准备开始画图
        QPainter painter(this);
        // 设置 painter 的抗锯齿，让画出来的线更平滑
        painter.setRenderHint(QPainter::Antialiasing);
        PaintClock(painter);
        painter.scale(side / 200.0, side / 200.0);
        //分别话画时针，分针，秒针
        PainterClockhour(painter,time);
        PainterClockminute(painter,time);
        PainterClocksecond(painter,time);
    }
    //游戏结束的处理
    if(endGame){
        //单人模式的情况
        start->hide();
        conTinue->hide();
        Quit->hide();
        // 基于该 widget 构建一个 painter，准备开始画图
        QPainter painter(this);
        // 设置 painter 的抗锯齿，让画出来的线更平滑
        painter.setRenderHint(QPainter::Antialiasing);
        // 设置 painter 的坐标系装换
        //时间到的情况
        if(timeOuts){
            ui->label->setVisible(true);
            endCartoon->start();
            TimeOutpainter(painter);
        }
        //无解的情况
        else if(canFind){
            ui->label->setVisible(true);
            endCartoon->start();
            EndPainter(painter);
        }
        else{
            //结束动画的放映
            EndMovie();
        }
    }
}

//
//timeOut1函数处理超时事件的响应
//
void MainWindow::timeOut1(){
    time3->stop();
    timeOuts = false;
    canFind = false;
}

//
//析构函数
//
MainWindow::~MainWindow()
{
    delete ui;
}

//
//startSlot函数没有形式参数，是start按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void MainWindow::startSlot()
{
    //设置字体的大小
    list.replace(5,"font:bold 35px");
    start->setStyleSheet(list.join(';'));
}

//
//conTinueSlot函数没有形式参数，是conTinue按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void MainWindow::conTinueSlot()
{
    //设置字体的大小
    list.replace(5,"font:bold 35px");
    conTinue->setStyleSheet(list.join(';'));
}

//
//QuitSlot函数没有形式参数，是Quit按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void MainWindow::QuitSlot()
{
    //设置字体的大小
    list.replace(5,"font:bold 35px");
    Quit->setStyleSheet(list.join(';'));
}

//
//startRslot函数没有形式参数，是start按钮松开的信号响应函数
//优化鼠标点击按钮的效果，并弹出新的窗口
//
void MainWindow::startRslot()
{
    //设置字体的大小
    list.replace(5,"font:bold 30px");
    start->setStyleSheet(list.join(';'));
    subWidget.show();
    this->hide();
}

//
//conTinueRslot函数没有形式参数，是conTinue按钮松开的信号响应函数
//优化鼠标点击按钮的效果，读档并弹出新的窗口
//
void MainWindow::conTinueRslot()
{
    //设置字体的大小
    list.replace(5,"font:bold 30px");
    conTinue->setStyleSheet(list.join(';'));
    gameWidget.ReadFile();
    this->hide();
    gameWidget.show();
}

//
//QuitRslot函数没有形式参数，是Quit按钮松开的信号响应函数
//优化鼠标点击按钮的效果，并退出程序
//
void MainWindow::QuitRslot()
{
    //设置字体的大小
    list.replace(5,"font:bold 30px");
    Quit->setStyleSheet(list.join(';'));
    this->close();
}

//
//backSlot函数没有形式参数，是返回上一步的按钮按下的信号响应函数
//
void MainWindow::backSlot()
{
    this->show();
    subWidget.hide();
}

//
//SoloSlot函数没有形式参数，是确定单人模式后的信号响应函数
//隐藏其他窗口，弹出地图初始化行数和列数的窗口
//
void MainWindow::SoloSlot()
{
    this->hide();
    subWidget.hide();
    setWidget.show();
    gameWidget.setMode(1);
    emit SoloSignals();
}

//
//groupSlot函数没有形式参数，是确定双人模式后的信号响应函数
//隐藏其他窗口，弹出地图初始化行数和列数的窗口
//
void MainWindow::groupSlot()
{
    this->hide();
    subWidget.hide();
    setWidget.show();
    gameWidget.setMode(2);
    emit groupSignals();
}

//
//ensureSlot函数没有形式参数，是确定行数和列数后的信号响应函数
//隐藏其他窗口，弹出游戏窗口
//
void MainWindow::ensureSlot()
{
    setWidget.hide();
    gameWidget.setRaC(setWidget.getRow(),setWidget.getCol());
    gameWidget.show();
    gameWidget.startSwitch();
}

//
//EndSlot函数没有形式参数，是确定地图后的信号响应函数
//隐藏其他窗口，弹出主窗口，绘制对应的游戏结束画面
//
void MainWindow::EndSlot()
{
    gameWidget.hide();
    this->show();
    time3->start();
    endGame = true;
    canFind = true;
}

//
//TimeoutSlot函数没有形式参数，是确定时间到后的信号响应函数
//隐藏其他窗口，弹出主窗口，绘制对应的游戏结束画面
//
void MainWindow::TimeoutSlot()
{
    gameWidget.hide();
    this->show();
    time3->start();
    endGame = true;
    timeOuts = true;  
}

void MainWindow::conTinueS()
{
    gameWidget.ReadFile();
    this->hide();
    gameWidget.show();
}

void MainWindow::Rule()
{
    QMessageBox::about(this,tr("game rules"),"就是练练看啊(doge)");
}
