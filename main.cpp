/**
  *     @file 程式的進入點
  *
  *     什麼都不用改的地方…
  *
  *     @author masks62009@gmail.com
  */

#include <QtGui/QApplication>
#include "mainwindow.h"

/// 只有這一行要注意的  ↓
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
        /// 對應的是這一行↓  把內容文字的編碼對應到跟系統設定的一樣，也就是 Utf-8
	QTextCodec::setCodecForCStrings( QTextCodec::codecForLocale() );
	MainWindow w;
	w.show();
	return a.exec();
}
