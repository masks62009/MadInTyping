/**
  *     @file 嘸蝦米字根元件的宣告檔
  *
  *     定義了主要的字根反查與比對功能，讀取功能則用編譯的方式
  *     決定採用那種來源。
  *
  *     @author masks62009@gmail.com
  */
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

#ifdef SQL_SUPPORT
QString int2str(int code);  ///< 將ibus表格的數字字根表轉換成英文字根表。
#endif

/**
  *     @brief Boshiamy class 控制字根讀取、比對與反查
  *
  *     將字根表的動作從視窗動作中獨立出來。在一開始就先載入，可以省下一些時間
  *     與操作的方便性。
  */

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
        QMap<QString,QStringList> Standard;             ///< 標準碼表格
        QMap<QString,QStringList> Simplified;           ///< 簡碼表格
#endif

        QMap<QString,QStringList> Complete;             ///< 全碼表格，在 iBus底下限制只能使用這個表
        Status status;                                  ///< 比對時採用的表格模式

#ifdef SQL_SUPPORT
        void swapList();
#endif
};

#endif // BOSHIAMY_H
