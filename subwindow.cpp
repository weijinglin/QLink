#include "subwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
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
//SubWindow函数是SubWindow类的构造函数
//进行窗口中构件的构造（计时器，按钮），并进行槽函数的绑定
//
SubWindow::SubWindow(QWidget *parent):QWidget(parent)
{

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
    //设置按钮的参数
    list.append("background-image:url(:/image/images/button.jpg)");   //设置背景图片
    list.append("border-style:outset");                               // 边框风格
    list.append("border-width:3px");                                  // 边框宽度
    list.append("border-color:rgb(10,45,110)");                       // 边框颜色
    list.append("border-radius:20px");                                // 边框倒角
    list.append("font:bold 30px");
    //构建新的按钮
    startSolo = new QPushButton(this);
    startGroup = new QPushButton(this);
    Quit = new QPushButton(this);
    startSolo->setStyleSheet(list.join(';'));
    startGroup->setStyleSheet(list.join(';'));
    Quit->setStyleSheet(list.join(';'));
    //调节按钮的文本，大小，位置
    startSolo->setText("单人模式");
    startSolo->resize(200,50);
    startSolo->move(width()/2 - 100,height()/2 + 50);
    startGroup->setText("双人模式");
    startGroup->resize(200,50);
    startGroup->move(width()/2 - 100,height()/2 + 150);
    Quit->setText("返回上一步");
    Quit->resize(200,50);
    Quit->move(width()/2 - 100,height()/2 + 250);
    //利用信号与槽机制对于按钮和各个窗口类的行为进行绑定
    QObject::connect(startSolo,SIGNAL(pressed()),this,SLOT(startSoloSlot()));
    QObject::connect(startGroup,SIGNAL(pressed()),this,SLOT(startGroupSlot()));
    QObject::connect(Quit,SIGNAL(pressed()),this,SLOT(QuitSlot()));
    QObject::connect(startSolo,SIGNAL(released()),this,SLOT(startSoloRslot()));
    QObject::connect(startGroup,SIGNAL(released()),this,SLOT(startGroupRslot()));
    QObject::connect(Quit,SIGNAL(released()),this,SLOT(QuitRslot()));
}

//
//startSoloSlot函数没有形式参数，是startSolo按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void SubWindow::startSoloSlot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 35px");
    startSolo->setStyleSheet(list.join(';'));
}

//
//startGroupSlot函数没有形式参数，是startGroup按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void SubWindow::startGroupSlot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 35px");
    startGroup->setStyleSheet(list.join(';'));
}

//
//QuitSlot函数没有形式参数，是Quit按钮按下的信号响应函数
//优化鼠标点击按钮的效果
//
void SubWindow::QuitSlot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 35px");
    Quit->setStyleSheet(list.join(';'));
}

//
//startSoloRslot函数没有形式参数，是startSolo按钮松开的信号响应函数
//优化鼠标点击按钮的效果，抛出信号
//
void SubWindow::startSoloRslot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 30px");
    startSolo->setStyleSheet(list.join(';'));
    emit SoloSignal();//触发单人模式的信号
}

//
//startGroupSlot函数没有形式参数，是startGroup按钮松开的信号响应函数
//优化鼠标点击按钮的效果，抛出信号
//
void SubWindow::startGroupRslot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 30px");
    startGroup->setStyleSheet(list.join(';'));
    emit groupSignal();//触发双人模式的信号
}

//
//QuitSlot函数没有形式参数，是Quit按钮松开的信号响应函数
//优化鼠标点击按钮的效果，抛出信号
//
void SubWindow::QuitRslot(){
    //设置按钮字体的大小
    list.replace(5,"font:bold 30px");
    Quit->setStyleSheet(list.join(';'));
    emit backSignal();//抛出返回上一步的信号
}

//
//paintEvent函数接受一个QPaintEvent类型的指针
//绘制相应的提示信息以及大背景
//
void SubWindow::paintEvent(QPaintEvent *){
    // 基于该 widget 构建一个 painter，准备开始画图
    QPainter painter(this);
    // 设置 painter 的抗锯齿，让画出来的线更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    //设置按钮的字体
    QFont font("楷体",15,QFont::Bold,true);
    painter.setFont(font);
    QPixmap pix;
    //设置 painter 的坐标系转换
    painter.translate(0, 0);
    pix.load(":/image/images/p7.jpg");//加载图片地址 可以是资源文件
    painter.drawPixmap(0, 0, width(), height(), pix);//绘制图片 横坐标、纵坐标、宽度、高度

}

//
//析构函数
//
SubWindow::~SubWindow(){

}
