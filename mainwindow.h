/**
  *     @file 主程式的標頭檔
  *
  *     定義 MainWindow的內容
  *
  *     @author masks62009@gmail.com
  */

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

/**
  *     @brief 主視窗的宣告
  *
  *     QT下的主要視窗，可以是一個單一的 Widget也可以是一個包含 menubar toolbar 跟 status
  *     bar的 MainWindow，這邊因為要跟之後的東東結合在一起，所以選擇繼承的是 MainWindow
  *
  */
class MainWindow : public QMainWindow {
Q_OBJECT
public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        Boshiamy * check;                                               ///< 以後也會一直用到的字根檢查元件
        void drawLayout();                                              ///< 畫出視窗外觀
        void setText(const QString text);                               ///< 設定要用的文章範本
        void awakeEnd();                                                ///< 打字結束後出現的畫面，同時一併結束計時

public slots:
        void showTime();                                                ///< 計時器與同步更新 statusbar的功能，未來要把更新 statusbar的功能獨立開來

protected:
        void changeEvent(QEvent *e);                                    ///< QT Creator自己生成的東東，沒什麼想改它的慾望…
        void keyPressEvent(QKeyEvent *);                                ///< 主要回應所有的鍵盤事件，計算按錯幾次， widget換行等等的事，其實應該精簡一點才是
        QStringList text2List(const QString text);                      ///< 把打字範本轉換成漢字的集合，就是把 QString 轉換成一個一個漢字的 List

/// -------------------------------------------------------------------------------------------------------------
/// 視窗組件
        QGridLayout *Grid;                                              ///< 堆放 QLabel的框架
        QScrollArea *Scroll;                                            ///< 提供捲動畫面的零件
        QList<QLabel *> LabelList;                                      ///< QLabel 指標的集合  QList好像不容許包裝繼承自 QWidget的物件，但是能裝它的指標
        QStringList Text;                                               ///< 打字範本的單字集合
        QTimer *timer;                                                  ///< 提供計時器的定時呼叫
        QTime *time;                                                    ///< 實際計算經過時間的計時器
        int Col;                                                        ///< 定義每行最多幾個字
        int At;                                                         ///< 計算目前打到那邊，用來取出 LabelList裡的指標
        int correct;                                                    ///< 正確幾個字
        int incorrect;                                                  ///< 錯誤幾個字
        int recorrect;                                                  ///< 修正後正確的字數
        QString Buff;                                                   ///< 目前鍵盤的 Buffer 區
        QLabel *status;                                                 ///< 對映 status bar 的內容

private:
        Ui::MainWindow *ui;                                             ///< QT Designer畫出來的組件
};

#endif // MAINWINDOW_H
