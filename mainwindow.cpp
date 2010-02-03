#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString dbAbsolutePathway = QDir::currentPath();
    qDebug()<<dbAbsolutePathway;
    dbAbsolutePathway =dbAbsolutePathway + QDir::separator() + "examSQL.sqlitedb";
    qDebug()<<dbAbsolutePathway;
    //qDebug() << QDir::exists(dbAbsolutePathway);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbAbsolutePathway);
    qDebug()<< db.open();
    etymonWidget = NULL;
    Suspend = false;
    position = this->pos();
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_action_KMP_triggered()
{
    QString title = "形";
    QStringList test = queryfromExamSQL(title);
    callforEtymon(title,test);
}

void MainWindow::callforEtymon(QString Title, QStringList Test)
{
    if(etymonWidget != NULL)
    {
        if(!Suspend)
        {
            QMessageBox msgbox;
            QString test = "確定要離開 "+etymonWidget->getExamTitle()+" 嗎？ ";
            msgbox.setText(test);
            msgbox.setInformativeText("還沒打完呢！");
            msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgbox.setDefaultButton(QMessageBox::No);
            int ret = msgbox.exec();
            if(ret == QMessageBox::No)
                return;
        }
        delete etymonWidget;
        etymonWidget = NULL;
    }
    Suspend = false;
    initEtymon(Title,Test);
}

void MainWindow::initEtymon(QString _Title, QStringList _Test)
{
    etymonWidget = new Etymon(this->centralWidget());
    this->setGeometry(QRect(position,QSize(640,300)));
    etymonWidget->setExamTitle(_Title);
    etymonWidget->setExamTest(_Test);
    etymonWidget->setStatusBar(this->statusBar());
    etymonWidget->show();
    etymonWidget->setFocus(Qt::ActiveWindowFocusReason);
    connect(etymonWidget,SIGNAL(suspend()),this,SLOT(suspend()));
}

void MainWindow::suspend()
{
    if(etymonWidget != NULL)
    {
        if(Related)
        {
            QString etymonTitle = etymonWidget->getExamTitle();
            QString relatedWord = queryfromRelateWord(etymonTitle);
            qDebug()<<relatedWord;
            //callTypingWidget(etymonTitle,relatedWord);
        }
    }
    Suspend = true;
}

QStringList MainWindow::queryfromExamSQL(QString _Test)
{
    QSqlQuery query;
    QString queryContentforTest = "select id,exam,list,explain,correct from examDB where classify = '" + _Test + "'";
    QStringList resultofQueryfromTest;

    query.exec(queryContentforTest);
    while(query.next())
    {
        QString singleResult = "";
        for(int i = 0 ; i < 5 ; i++)
        {
            singleResult.append(query.value(i).toString());
            singleResult.append(";");
        }
        resultofQueryfromTest.append(singleResult);
    }
    return resultofQueryfromTest;
}
QString MainWindow::queryfromRelateWord(QString _Test)
{
    QSqlQuery query;
    QString queryContentforTest = "select relatedwords from relatedDB where classify = '" + _Test + "'";
    QString relatedWords ="";
    query.exec(queryContentforTest);
    relatedWords = query.value(0).toString();
    return relatedWords;
}
