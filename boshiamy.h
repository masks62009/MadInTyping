#ifndef BOSHIAMY_H
#define BOSHIAMY_H

#ifdef SQL_SUPPORT
    #include <QtSql>
    #include <QDir>
#endif

#include <QString>
#include <QMap>
#include <QStringList>
#include <QFile>
#import <QtCore>

#ifndef SQL_SUPPORT

#include <QTextStream>

#endif

#import "const.h"

QString int2str(int code);

class Boshiamy
{
public:
        Boshiamy();

        bool readfile(const QString filename);
        bool compare(const QString test, const QString list);
        void setStatus(Status mode);
        QString getCode(QString word);

protected:

#ifndef SQL_SUPPORT
        QMap<QString,QStringList> Standard;
        QMap<QString,QStringList> Simplified;
#endif
        QMap<QString,QStringList> Complete;

#ifdef SQL_SUPPORT
        void swapList();
#endif

#ifndef SQL_SUPPORT
        Status status;
#endif

};

#endif // BOSHIAMY_H
