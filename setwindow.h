#ifndef SETWINDOW_H
#define SETWINDOW_H

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

class SetWindow : public QWidget
{
    Q_OBJECT

public:
    SetWindow(QWidget *parent = nullptr);
    ~SetWindow();
    int getRow();
    int getCol();
    QLineEdit *editCol;
    QLineEdit *editRow;
    QPushButton *ensure;

protected:
    int m,n;
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    QStringList list;
private slots:
    void ensureSlots();
    void ensureRslots();

signals:
    void ensureSignal();
};



#endif // SETWINDOW_H
