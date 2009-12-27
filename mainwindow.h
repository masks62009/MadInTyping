#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#import <QtGui>
#import <QtCore>
#import "boshiamy.h"
#import "const.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        Boshiamy * check;
        void drawLayout();
        void setText(const QString text);
        void awakeEnd();

public slots:
        void showTime();

protected:
        void changeEvent(QEvent *e);
        void keyPressEvent(QKeyEvent *);
        QStringList text2List(const QString text);

        QGridLayout *Grid;
        QScrollArea *Scroll;
        QList<QLabel *> LabelList;
        QStringList Text;
        QTimer *timer;
        QTime *time;
        int Col;
        int At;
        int correct;
        int incorrect;
        int recorrect;
        QString Buff;
        QLabel *status;

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
