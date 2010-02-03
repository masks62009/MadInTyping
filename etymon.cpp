#include "etymon.h"
#include "ui_etymon.h"

Etymon::Etymon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Etymon)
{
    ui->setupUi(this);
}

Etymon::~Etymon()
{
    delete ui;
}

void Etymon::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString Etymon::getExamTitle()
{
    return widgetTitle;
}

void Etymon::setExamTest(QStringList examTest)
{}

void Etymon::setExamTitle(QString examTitle)
{}

void Etymon::setExam(QStringList *examList)
{}

void Etymon::setStatusBar(QStatusBar *mainWindowStatusBar)
{}

void Etymon::setWrongWordCollect(bool wrongWordCollect)
{}

void Etymon::keyPressEvent(QKeyEvent *keyEvent)
{}

inline void Etymon::cleanWordEtymonPicture()
{}

inline void Etymon::showEtymonPicture(QString etymonPicture)
{}

inline void Etymon::showWordEtymonPicture(QStringList wordEtymonList)
{}

inline void Etymon::showWord(QString word,QString color)
{}

void Etymon::init()
{}
