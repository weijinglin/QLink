#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTime>
#include <QObject>
#include <QLabel>
#include <qvalidator.h>
#include <qlineedit.h>
#include "subwindow.h"
#include "setwindow.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPushButton *start,*conTinue,*Quit;
    void Dealth1();
    void Dealth2();
//    QTimer *time2;
    QTimer *time3;
    QMovie *endCartoon;
    QLabel *label;

protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    bool endGame = false;
    bool canFind = false;
    bool timeOuts = false;
    QStringList list;
    SubWindow subWidget;
    SetWindow setWidget;
    GameWindow gameWidget;
    QAction *help;
    QAction *Load;
    void Rule();
    void conTinueS();
    void MakeUp();
    void PrepareforMap();
    void PaintClock(QPainter &painter);
    void PainterClockhour(QPainter &painter,QTime a);
    void PainterClockminute(QPainter &painter,QTime a);
    void PainterClocksecond(QPainter &painter,QTime a);
    void EndPainter(QPainter &painter);
    void TimeOutpainter(QPainter &painter);
    void EndMovie();
private slots:
//    void timeOut(); // 定时器超时函数
    void timeOut1();
    void startSlot();
    void conTinueSlot();
    void QuitSlot();
    void startRslot();
    void conTinueRslot();
    void QuitRslot();
    void backSlot();
    void SoloSlot();
    void groupSlot();
    void ensureSlot();
    void EndSlot();
    void TimeoutSlot();

signals:
    void SoloSignals();
    void groupSignals();

private:
    Ui::MainWindow *ui;
};




#endif // MAINWINDOW_H
