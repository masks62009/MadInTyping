/**
  *     @file 這是嘸蝦米的對照表元件
  *
  *     class Boshiamy定義了主要對字根對照表的操作，包括取得漢字字根、
  *     比對是否是正確字根與字根表的讀取。實作方面將輸入法格式與內容隔開
  *     ，所以只要是符合 <QString 漢字 ,QStringList 字根表 >的模式
  *     可以使用。
  *
  *     @author masks62009@gmail.com
  */



#include "boshiamy.h"

#ifdef SQL_SUPPORT
/** @brief 把 iBus的表格檔轉換成字根
  *
  * 不是很漂亮的作法，不過它可以用，短時間內不會改動這一塊。
  *
  * @param int code 讀入的數字碼
  *
  * @return 傳回形別為 QString 的字根
  */
QString int2str(int code)
{
	QString temp;
	switch(code){
	case 0:
		temp = "0"; break;
	case 1:
		temp = "a"; break;
	case 2:
		temp = "b"; break;
	case 3:
		temp = "c"; break;
	case 4:
		temp = "d"; break;
	case 5:
		temp = "e"; break;
	case 6:
		temp = "f"; break;
	case 7:
		temp = "g"; break;
	case 8:
		temp = "h"; break;
	case 9:
		temp = "i"; break;
	case 10:
		temp = "j"; break;
	case 11:
		temp = "k"; break;
	case 12:
		temp = "l"; break;
	case 13:
		temp = "m"; break;
	case 14:
		temp = "n"; break;
	case 15:
		temp = "o"; break;
	case 16:
		temp = "p"; break;
	case 17:
		temp = "q"; break;
	case 18:
		temp = "r"; break;
	case 19:
		temp = "s"; break;
	case 20:
		temp = "t"; break;
	case 21:
		temp = "u"; break;
	case 22:
		temp = "v"; break;
	case 23:
		temp = "w"; break;
	case 24:
		temp = "x"; break;
	case 25:
		temp = "y"; break;
	case 26:
		temp = "z"; break;
	case 27:
		temp = "\'"; break;
	case 28:
		temp = ";"; break;
	case 29:
		temp = "`"; break;
	case 45:
		temp = "["; break;
	case 46:
		temp = "]"; break;
	case 55:
		temp = ","; break;
	case 56:
		temp = "."; break;
	}
	return temp;
}
#endif

/**
  *     @brief Boshiamy 元件的建立
  *
  *     沒有輸出與輸入值，一切都是預設好的，在表格模式是採用編入的 liu-uni-trail.txt
  *     來作為字根表來源，在 SQL_SUPPORT  模式則是用同一目錄下的 boshiamy_t.db 作為
  *     輸入來源。同時設定預設的比對模式是全碼模式。
  */
Boshiamy::Boshiamy()
{
#ifndef SQL_SUPPORT
        if(readfile(":/liu"))                                           ///< 採用內置的檔案
	{
    #ifdef DEBUG
		qDebug("read table file correct");
    #endif
	}
	else
	{
    #ifdef DEBUG
		qDebug("read table file incorrect");
    #endif
	}
#endif

#ifdef SQL_SUPPORT
	QDir dir;
        QString path = dir.currentPath().append(QDir::separator());     ///< 取得當前程式所在目錄
        path.append(iBusFileName);                                      ///< 加入預設的表格名稱
	if(readfile(path))
	{
    #ifdef DEBUG
		qDebug("read SQL file correct");
    #endif
	}
	else
        {
                                                                        ///< 若讀取失敗，可能要丟出一個warning box來
    #ifdef DEBUG
		qDebug("read SQL file incorrect");
    #endif
	}
#endif
        setStatus(B_Complete);                                          ///< 預設是全模式 ;
}

/**
  *     @brief 變動輸入法表格模式
  *
  *     因為 status是 protected的，所以為了讓呼叫的地方可以改變輸入法的表格模
  *     式，所以這邊會有一個傳入值 ，但不會有傳回值。
  *
  *     @param Status mode 要變更的輸入法模式，定義在 const.h 這邊
  */
void Boshiamy::setStatus(Status mode)
{
	status = mode;
}

/**
  *     @brief 傳回字根與漢字比對是否一致
  *
  *     bool compare()主要是傳回輸入的漢字字根 list 是否出現在 test 的字根表中
  *
  *     @param QString test 傳入的漢字
  *     @param QString list 傳入欲比對的字根
  *     @retval true  為 存在這個字根
  *     @retval false 為 不存在這個字根
  */
bool Boshiamy::compare(QString test, QString list)
{
        QMap<QString,QStringList>  check;                               ///< 這邊用的是複製方法取得輸入法表格，這樣其實比較浪費空間。用指標或許會比較好一點？？

#ifdef SQL_SUPPORT
	check = Complete;
#endif

#ifndef SQL_SUPPORT
	if(status == B_Simplified)
	{
		check = Simplified;
	}
	else if (status == B_Standard)
	{
		check = Standard;
	}
        else                                                            ///< status == B_Complete;
	{
		check = Complete;
	}
#endif
	if(check.value(test).contains(list.toUpper()))
		return true;
	else
		return false;

}

/**
  *     @brief 傳回指定漢字的預設模式第一個字根
  *
  *     QString getCode()作為反查漢字的字根之用，會依 status 採用不同的輸入法表格
  *
  *     @param QString word 欲反查的漢字
  *     @return QString 傳回漢字的指定輸入法表格中第一個字根
  */
QString Boshiamy::getCode(QString word)
{
	QString tmp;
#ifndef SQL_SUPPORT
        if(status == B_Simplified)
        {
                tmp  = Simplified.value(word).first();
        }else if (status == B_Standard)
        {
                tmp  = Standard.value(word).first();
        }else
        {
                tmp  = Complete.value(word).first();
        }
#endif
#ifdef SQL_SUPPORT
        tmp = Complete.value(word).first();                             ///< iBus表格只有 Complete模式可供使用
#endif
	return tmp;
}

/**
  *     @brief 讀取純文字格式的表格檔
  *
  *     bool readfile()是一個用 ifdef重載的函式，這個是讀取純文字檔的版本
  *     由於純文字的表格比較多資訊，所以它有比較多的資料要處理；包括建議碼、
  *     簡碼與全碼模式。因為這個檔案是 Qt 的資源模式載入的，所以其實沒有什麼
  *     錯誤會需要處理的。大致上就是簡單的讀進來、排好，乖乖的用就好了。
  *
  *     @param QString filename 要讀入的檔案名稱，不寫死的原因是… 嗯…就是不想寫死
  *     @retval true  讀取正常 ///< 理論上都應該是這個啦！
  *     @retval false 讀取失敗
  */
#ifndef SQL_SUPPORT
bool Boshiamy::readfile(QString filename)
{
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
                in.readLine((qint64)80);                                ///< 略掉第一行;
		while(!in.atEnd())
		{
                        QString line =in.readLine((qint64)64);          ///< 讀入的每一行;
			QString key = line.section("",0,5).simplified(), ///< 字根;
                                word = line.section('~',1,1);           ///< 漢字;
                        /**
                          *  bool standard = (line.section('~',3,3).compare("v",Qt::CaseInsensitive)) ? false : true ;
                          *  bool simplify = (line.section('~',4,4).compare("v",Qt::CaseInsensitive)) ? false : true ;
                          *  QString::compare()的用法很特別，它是如果傳入的字根比較多字，傳回正值，兩者相同傳回零，傳入字根較少字
                          *  則傳回負值。  所以這邊只要用  !(QString::compare())就可以了…
                          */
			bool standard = !(line.section('~',3,3).compare("v"));
			bool simplify = !(line.section('~',4,4).compare("v"));

                        if(Complete.contains(word))                     ///< 若這個字碼有出現過了，則用插入的方式;
			{
				QStringList tmp;
				tmp = Complete.value(word);
				tmp.append(key);
				Complete.insert(word,tmp);
				if(standard)
				{
					tmp = Standard.value(word);
					tmp.append(key);
					Standard.insert(word,tmp);
				}
				if(simplify)
				{
					tmp = Simplified.value(word);
					tmp.append(key);
					Simplified.insert(word,tmp);
				}
			}
                        else                                            ///<  若這個字碼沒有出現過，則用新增的方式;
			{
				QStringList tmp;
				tmp.append(key);
				Complete.insert(word,tmp);
				if(standard)
					Standard.insert(word,tmp);
				if(simplify)
					Simplified.insert(word,tmp);
			}
		}
	}
	else
	{
        #ifdef DEBUG
		qDebug("file opening error");
                                                                        ///<  do something...;
        #endif
		return false;
	}
	return true;
}
#endif

/**
  *     @brief 讀取 iBus的sqlite表格檔
  *
  *     同樣的，外觀一樣，所以其它地方要改的就不會太多。
  *     這個是用到了 QtSQL的功能去作讀檔的動作，因為 iBus的表格檔並沒有欄位
  *     標明這個碼是建議碼或是簡碼什麼的，取而代之的是打字頻率。這個要再作處理
  *     不是不行，就只是覺得有點麻煩，所以我就寫了一個簡單的排序，也就是
  *     void swapList()，把讀進來的字根表依長度作排序。當然這個可能會出錯。
  *     也不一定是習慣用碼，但至少一碼字兩碼字會是正常的…我猜這樣應該也就可以
  *     了吧！
  *
  *     @param filename ibus 表格檔案的絕對位置
  *     @retval true  一切正常
  *     @retval false 有錯誤發生； 因為這個是有可能發生的，比方說 SQL query
  *                     有錯，或是 SQL 表格格式有問題等等鳥事都有可能會發生，
  *                     所以在 Boshiamy 的 建構子那邊，在 call這段的地方最
  *                     好還是作點什麼…比方說錯誤訊息回報等等等的…
  */
#ifdef SQL_SUPPORT
bool Boshiamy::readfile(QString filename)
{
	if(QFile::exists(filename))
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",filename);
		if(db.open())
		{
			QSqlQuery query;
			if(query.exec("select mlen,m0,m1,m2,m3,m4,phrase from phrases")){
				while(query.next()){
					int codelen = query.value(0).toInt();
					QString code,text;
                                        switch(codelen)                 ///< 不同的字根長度，讀入相關的值
					{
					case 1:
						code = int2str(query.value(1).toInt());
						break;
					case 2:
						code = int2str(query.value(1).toInt());
						code.append(int2str(query.value(2).toInt()));
						break;
					case 3:
						code = int2str(query.value(1).toInt());
						code.append(int2str(query.value(2).toInt()));
						code.append(int2str(query.value(3).toInt()));
						break;
					case 4:
						code = int2str(query.value(1).toInt());
						code.append(int2str(query.value(2).toInt()));
						code.append(int2str(query.value(3).toInt()));
						code.append(int2str(query.value(4).toInt()));
						break;
					case 5:
						code = int2str(query.value(1).toInt());
						code.append(int2str(query.value(2).toInt()));
						code.append(int2str(query.value(3).toInt()));
						code.append(int2str(query.value(4).toInt()));
						code.append(int2str(query.value(5).toInt()));
						if (query.value(4).toInt() == 45 && query.value(5).toInt()==46)
                                                        code.clear();   ///< 如果是 Luke加上去的識別碼，就刪掉它… ;
						break;
					default:
						break;
					}
					text = query.value(6).toString();
					if(!code.isEmpty())
					{
						QStringList tmp;
						if(Complete.contains(text))
						{
							tmp = Complete.value(text);
							tmp.append(code);
							Complete.insert(text,tmp);
						}
						else
						{
							tmp.append(code);
							Complete.insert(text,tmp);
						}
					}
				}
			}
			else
			{
        #ifdef DEBUG
				qDebug("SQL query error");
                                qDebug()<<query.lastError();            ///< 養成習慣，有錯就輸出…
        #endif
				return false;
			}
		}
		else
		{
        #ifdef DEBUG
			qDebug("ERROR: Sql Opening fault...");
                        qDebug()<< db.lastError();                       ///< 養成習慣，有錯就輸出
        #endif
			return false;
		}
	}
	else
	{
        #ifdef DEBUG
		qDebug("file opening error");
                                                                        ///< 檔案不見了…  do something...;
        #endif
		return false;
	}
        swapList();                                                     ///< 把讀進來的表格作排序
	return true;
}

/**
  *     @brief 排序輸入法表格 ( 僅 iBus表格檔有此需要）
  *
  *     沒有傳入值也沒有傳出值， void swapList()會將 Complete的字根表格照字根長度
  *     排序。
  */
void Boshiamy::swapList()
{
	foreach(QString str,Complete.keys())
	{
		QStringList items = Complete.value(str);
		QMap<int,QStringList> map;
		foreach(QString item,items)
		{
			QStringList tmp;
			if(map.contains(item.length()))
			{
				tmp = map.value(item.length());
				tmp.append(item);
				map.insert(item.length(),tmp);
			}
			else
			{
				tmp.append(item);
				map.insert(item.length(),tmp);
			}
		}
		QStringList tmp;
		for(int i = 1 ; i < 6 ; i++)
		{
			if(map.contains(i))
				tmp.append(map.value(i));
		}
		Complete.insert(str,tmp);
	}
}
#endif
