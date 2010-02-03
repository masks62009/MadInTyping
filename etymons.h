#ifndef ETYMONS_H
#define ETYMONS_H

#include <QWidget>
#include <QStatusBar>
#include <QLabel>

namespace Ui {
    class Etymons;
}
class Etymons : public QWidget
{
Q_OBJECT
public:
    Etymons(QWidget *parent = 0);
    ~Etymons();
    QString getExamTitle();
    void setExamTitle(QString examTitle);
    void setExamTest(QStringList examTest);
    void setExam(QStringList *examList);
    void setStatusBar(QStatusBar *mainWindowStatusBar);
    void setWrongWordCollect(bool wrongWordCollect);
    void setRelatedWords(QString examRelatedContents);
signals:
    void suspend();
public slots:
private:
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
protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent * keyEvent);

private:
    Ui::Etymons *ui;
};

#endif // ETYMONS_H
