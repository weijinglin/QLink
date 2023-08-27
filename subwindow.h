#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTime>
#include <QLabel>
#include <qvalidator.h>
#include <qlineedit.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SubWindow; }
QT_END_NAMESPACE

class SubWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubWindow(QWidget *parent = nullptr);
    ~SubWindow();

protected:
    QStringList list;
    QPushButton *startSolo,*startGroup,*Quit;
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:

private slots:
    void startSoloSlot();
    void startGroupSlot();
    void QuitSlot();
    void startSoloRslot();
    void startGroupRslot();
    void QuitRslot();

signals:
    void SoloSignal();
    void groupSignal();
    void backSignal();

private:
    Ui::SubWindow *ui;
};



#endif // SUBWINDOW_H
