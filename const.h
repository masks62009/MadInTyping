/**
  *     @file 這是整個程式都會用到的常數定義
  *
  *     定義內容包括預設的 ibus表格名稱、最大錯次容許次數、
  *     最大字根數、輸入法比對模式。
  *
  *     @author masks62009@gmail.com
  */
#ifndef CONST_H
#define CONST_H

/// iBus輸入法表格檔名
#define iBusFileName    "boshiamy_t.db"

/// 測試用字串，正式版不會出現這個
#define Sample_Text     "這是框架的小小測試，主要是用來測出到底這個小程式是不是可以正確的運作。目前是把是所有的字、詞的判讀用另外的元件包起來。所以要改變輸入法表格就不會太麻煩，需要注意的是，它的範例文章所有標點符號都必須要是全形標點，不然在斷字方面是會出錯的… "

/// 定義最長字根字數
#define Max_KeyCode     5

/// 定義最多錯誤次數
#define Max_Count       2

/// 輸入表格的型別 ，  依次是  標準碼  簡碼  全碼  三種
typedef enum {B_Standard, B_Simplified, B_Complete}  Status;


/// 除錯用定義，用來開啟一些 compiler 時的選項
// #define DEBUG
// #define SQL_SUPPORT

#endif // CONST_H
