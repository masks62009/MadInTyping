#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtCore>
#include <QtGui>
#include "etymon.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    bool Suspend;
    bool Related;
    QPoint position;
    QSqlDatabase db;
    Etymon *etymonWidget;
    QStringList queryfromExamSQL(QString _Test);
    QString queryfromRelateWord(QString _Test);
    void callforEtymon(QString Title, QStringList Test);
    void initEtymon(QString _Title,QStringList _Test);
private slots:
    void on_action_KMP_triggered();
    void suspend();
};

#endif // MAINWINDOW_H
