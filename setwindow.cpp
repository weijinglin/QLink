#include "setwindow.h"
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

//
//SetWindow函数是窗口类的构造函数
//构造文本编辑框和计时器以及确定按钮
//
SetWindow::SetWindow(QWidget *parent):QWidget(parent)
{
    m = n = -1;
    // 构建一个计时器
    QTimer *timer = new QTimer(this);
    // 将计时器的超时事件与 AnalogClock 的 update 函数绑定，即一旦计时器时间到了，就会调用一次 update 函数
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SetWindow::update));
    // 计时器开始计时，每次时间间隔 100 ms
    timer->start(100);
    // 设置窗口的标题
    setWindowTitle(tr("连连看"));
    // 设置 widget 大小
    resize(1000, 1000);
    //设置按钮的参数
    list.append("background-image:url(:/image/images/button.jpg)");   //设置背景图片
    list.append("border-style:outset");                               // 边框风格
    list.append("border-width:3px");                                  // 边框宽度
    list.append("border-color:rgb(10,45,110)");                       // 边框颜色
    list.append("border-radius:20px");                                // 边框倒角
    list.append("font:bold 30px");

    ensure = new QPushButton(this);
    ensure->setStyleSheet(list.join(';'));
    ensure->setText("确定");
    ensure->resize(200,50);
    ensure->move(400,800);

    //构建一个文本编辑器
    editCol = new QLineEdit(this);
    editRow = new QLineEdit(this);
    //设置文本编辑器的位置
    editCol->setGeometry(400,400,200,50);
    editRow->setGeometry(400,600,200,50);
    //设置提示性信息
    editCol->setPlaceholderText("输入n的值(5<=n<=10)");
    editRow->setPlaceholderText("输入m的值(5<=m<=10)");

    connect(ensure,SIGNAL(pressed()),this,SLOT(ensureSlots()));
    connect(ensure,SIGNAL(released()),this,SLOT(ensureRslots()));
}

//
//paintEvent函数接受一个QPaintEvent类型的指针
//绘制相应的提示信息以及大背景
//
void SetWindow::paintEvent(QPaintEvent *){
    // 基于该 widget 构建一个 painter，准备开始画图
    QPainter painter(this);
    // 设置 painter 的抗锯齿，让画出来的线更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font("楷体",15,QFont::Bold,true);
    //设置画笔的字体
    painter.setFont(font);
    QPixmap pix;
    // 设置 painter 的坐标系转换
    painter.translate(0, 0);
    pix.load(":/image/images/p7.jpg");//加载图片地址 可以是资源文件
    painter.drawPixmap(0, 0, width(), height(), pix);//绘制图片 横坐标、纵坐标、宽度、高度

    painter.drawText(350,750,"m,n至少有一个为偶数");
}

//
//ensureSlots函数没有形式参数，是ensure按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void SetWindow::ensureSlots(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 35px");
    ensure->setStyleSheet(list.join(';'));
}

//
//ensureSlots函数没有形式参数，是ensure按钮松开的信号响应函数
//优化鼠标点击按钮的效果，获取游戏地图的行数和列数
//
void SetWindow::ensureRslots(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 30px");
    ensure->setStyleSheet(list.join(';'));
    m = editRow->text().toInt();
    n = editCol->text().toInt();
    //进行行数和列数的规范性检验
    if(m >= 5 && m <= 10 && n >= 5 && n <= 10 && m*n % 2 == 0){
        //获取游戏地图的行数和列数
        m = editRow->text().toInt();
        n = editCol->text().toInt();
        editCol->clear();
        editRow->clear();
        emit ensureSignal();
    }
}

//
//getRow函数没有形式参数
//返回地图的行数
//
int SetWindow::getRow()
{
    return m;
}

//
//getCol函数没有形式参数
//返回地图的列数
//
int SetWindow::getCol()
{
    return n;
}

//
//析构函数
//
SetWindow::~SetWindow(){

}
