#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTime>
#include <QObject>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <QLabel>
#include <qvalidator.h>
#include <qlineedit.h>
#include "subwindow.h"
#include <QVector3D>
#include <QOpenGLFunctions_4_0_Compatibility>

struct Point{
    int x;
    int y;
    Point(int a = -1,int b = -1):x(a),y(b){}
    ~Point(){}
};

class Player{
public:
    int lx;
    int ly;
    int score;
    char disPlay[5];
    Player(int x = -1,int y = -1,int z = 0):lx(x),ly(y),score(z){}
    ~Player(){}
    //函数translate将类中的分数转化为字符串且最高位的数字不为0(除了0分的情况外)
    void translate(){
        disPlay[0] = score / 1000 + '0';
        disPlay[1] = score / 100 % 10 + '0';
        disPlay[2] = score / 10 % 10 + '0';
        disPlay[3] = score % 10 + '0';
        disPlay[4] = '\0';
        if(disPlay[0] == '0'){
            for(int i = 0;i <= 3;++i){
                disPlay[i] = disPlay[i + 1];
            }
        }
        if(disPlay[0] == '0'){
            for(int i = 0;disPlay[i] != '\0';++i){
                disPlay[i] = disPlay[i + 1];
            }
        }
        if(disPlay[0] == '0'){
            for(int i = 0;disPlay[i] != '\0';++i){
                disPlay[i] = disPlay[i + 1];
            }
        }
    }
};

class GameWindow : public QWidget
{
     Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    QPushButton *stop;
    QPushButton *fileSave,*conTinue;
    void setMode(int );
    int getMode();
    void startSwitch();
    char* P1Marks();
    char* P2Marks();
    void ReadFile();
    bool CompareMarks1();
    bool CompareMarks2();
    void setScoreatime();
    void setZero();
    void setRaC(const int a,const int b);

protected:
    struct PowerMessage{
        int PowTi;//特殊效果出现的次数
        int PowerTime[3];//确定效果出现的时间
        Point PowerPoint[3];//确定效果出现的位置
        bool BeingTime[3];//确定效果是否已经出现
        int kind;//确定效果的种类
        int User;//用于双人模式中确定收到特殊效果的对象
        bool SWitchPower;//确定效果是否使用
        int Loc;//确定特殊效果出现的位置

    public:
        PowerMessage(int k = 0,int a = 3,bool z = false);
    };
    static const int clabel = 100;
    int m,n;
    QTimer *timeOut1;
    int hintcounter;
    int flashcounter;
    int freezecounter;
    int dizzycounter;
    int AllTime;
    int gameMode;
    int **gameMap;
    Point sPlayer1,sPlayer2;
    Point sPlaytwo1,sPlaytwo2;
    Point helpFlash;
    Player P1,P2;
    bool stopButton;
    bool getReady;
    bool startTime;
    bool deleteTime;
    void Zero(PowerMessage &a);
    void MakeUp();
    void PaintMaP(QPainter &event);
    void PaintFeedbackone(QPainter &event);
    void PaintFeedbacktwo(QPainter &event);
    void PaintPower(QPainter &painter);
    void DrawBackground(QPainter &painter);
    void DealGrade(QPainter &painter);
    void DealMode();
    void DealPower();
    void LockLoc(PowerMessage &a,int i);
    bool RightFind(Point &a1,Point &a2,Point &b,Point &c);
    bool LeftFind(Point &a1,Point &a2,Point &b,Point &c);
    bool UpFind(Point &a1,Point &a2,Point &b,Point &c);
    bool DownFind(Point &a1,Point &a2,Point &b,Point &c);
    void MakeMaP();
    bool DealLine(Player &a,Point,Point);
    bool DealRect(Player &,Point &a,Point b,Point c);
    bool DealDourect(Player &a,Point &a1,Point &a2,Point b,Point c);
    char *tranAlltime();
    void rearrange(int **s);
    void swapRow(int **s,int a,int b);
    void swapCol(int **s,int a,int b);
    bool LineJudge(const Point a,const Point b,int **s);
    bool RectJudge(const Point a,const Point b,int **s);
    bool DoubleRectjudge(const Point a,const Point b,int **s);
    Point PointRect(const Point a,const Point b,int **s);
    int **helpArray(int **s1);
    bool EndJudge(int **s1);
    void TimeJudge();
    void DisJudge(PowerMessage &a);
    void DisAppear();
    void HintPower(Point &a,Point &b);
    bool PowerJudge(PowerMessage &a);
    void DealUp(Player &a,Point &b,Point &c);
    void DealDown(Player &a,Point &b,Point &c);
    void DealRight(Player &a,Point &b,Point &c);
    void DealLeft(Player &a,Point &b,Point &c);
    void SaveinFile();
    void EndHint();
    void EndFlash();
    void EndFreeze();
    void EndDizzy();
    void setPower(PowerMessage &m,int a,int b,int c);
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    QStringList list;
    QPixmap p12;
    QPixmap p22;
    QPixmap p1;
    QPixmap p2;
    QPixmap p3;
    QPixmap p4;
    QPixmap p5;
    QPixmap p6;
    QPixmap p7;
    QPixmap p8;
    QPixmap p9;
    QPixmap p10;
    QPixmap p11;
    QPixmap sk1;
    QPixmap sk2;
    QPixmap sk3;
    QPixmap sk4;
    QPixmap sk5;
    QPixmap sk6;
    Point hintPo1,hintPo2;
    QSet<int> KeypressEvent;
    QTimer *updateKey;
    PowerMessage shufflePower,TimePower,hintPower,flashPower,freezePower,dizzyPower;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private slots:
    void UpdateTime();
    void DealPressed();
    void stopPslot();
    void stopRslot();
    void filesavePslot();
    void filesaveRslot();
    void continuePslot();
    void continueRslot();

signals:
    void EndGame();
    void TimeOut();


};

#endif // GAME_H
