#include "boshiamy.h"
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
Boshiamy::Boshiamy()
{
#ifndef SQL_SUPPORT
	if(readfile(":/liu"))
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
	QString path = dir.currentPath().append(QDir::separator());
	path.append(iBusFileName);
	if(readfile(path))
	{
    #ifdef DEBUG
		qDebug("read SQL file correct");
    #endif
	}
	else
	{
    #ifdef DEBUG
		qDebug("read SQL file incorrect");
    #endif
	}
#endif
        setStatus(B_Complete); ///< 預設是全模式 ;
}

void Boshiamy::setStatus(Status mode)
{
        status = mode;
}

bool Boshiamy::compare(QString test, QString list)
{
	QMap<QString,QStringList>  check;

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
	else ///< status == B_Complete;
	{
		check = Complete;
	}
#endif
        if(check.value(test).contains(list.toUpper()))
		return true;
	else
		return false;

}

QString Boshiamy::getCode(QString word)
{
        QString tmp;
#ifndef SQL_SUPPORT
        tmp  = Standard.value(word).first();
#endif
#ifdef SQL_SUPPORT
        tmp = Complete.value(word).first();
#endif
        return tmp;
}

#ifndef SQL_SUPPORT

bool Boshiamy::readfile(QString filename)
{
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		in.readLine((qint64)80); ///< 略掉第一行;
                while(!in.atEnd())
                {
			QString line =in.readLine((qint64)64); ///< 讀入的每一行;
                        QString key = line.section("",0,5).simplified(), ///< 字根;
				word = line.section('~',1,1); ///< 漢字;
                        /*
                        bool standard = (line.section('~',3,3).compare("v",Qt::CaseInsensitive)) ? false : true ;
                        bool simplify = (line.section('~',4,4).compare("v",Qt::CaseInsensitive)) ? false : true ;
                        */
                        bool standard = !(line.section('~',3,3).compare("v"));
                        bool simplify = !(line.section('~',4,4).compare("v"));

			if(Complete.contains(word)) ///< 若這個字碼有出現過了，則用插入的方式;
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
			else ///<若這個字碼沒有出現過，則用新增的方式;
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
					switch(codelen)
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
							code.clear();         ///< 如果是 Luke加上去的識別碼，就刪掉它… ;
						break;
					default:
						break;
					}
					text = query.value(6).toString();
                                        if(! code.isEmpty())
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
				qDebug()<<query.lastError();
#endif
                                return false;
			}
		}
		else
		{
#ifdef DEBUG
			qDebug("ERROR: Sql Opening fault...");
			qDebug()<< db.lastError();
#endif
                        return false;
		}
	}
	else
	{
#ifdef DEBUG
		qDebug("file opening error");
		///< do something...;
#endif
                return false;
	}
        swapList();
        return true;
}

void Boshiamy::swapList()
{
        foreach(QString str,Complete.keys())
        {
                QStringList items = Complete.value(str);
                QMap<int,QStringList> map;
                foreach(QString item ,items)
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
