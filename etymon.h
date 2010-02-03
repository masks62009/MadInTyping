#ifndef ETYMON_H
#define ETYMON_H

#include <QWidget>
#include <QtGui>
#include <QtCore>

namespace Ui {
    class Etymon;
}

class Etymon : public QWidget {
    Q_OBJECT
public:
    Etymon(QWidget *parent = 0);
    ~Etymon();
    QString getExamTitle();
    void setExamTitle(QString examTitle);
    void setExamTest(QStringList examTest);
    void setExam(QStringList *examList);
    void setStatusBar(QStatusBar *mainWindowStatusBar);
    void setWrongWordCollect(bool wrongWordCollect);
    //void setRelatedWords(QString examRelatedContents);
signals:
    void suspend();
protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent * keyEvent);
private:
    Ui::Etymon *ui;
    QStatusBar *statusBar;
    QString widgetTitle;
    QString relatedWords;
    bool hasWrongWordCollect;
    QStringList *currentExamList;
    QStringList *wrongExamList;
    void init();
    QLabel *wordEtymon[5];

    inline void cleanWordEtymonPicture();
    inline void showEtymonPicture(QString etymonPicture);
    inline void showWordEtymonPicture(QStringList wordEtymonList);
    inline void showWord(QString word,QString color);
};

#endif // ETYMON_H
