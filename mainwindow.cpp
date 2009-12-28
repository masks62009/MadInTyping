/**
  *     @file  這是主視窗的內容
  *
  *     主要視窗的活動，包括對事件的反應都寫在這邊，目前是把打字測驗的部份寫死在這邊
  *     但未來應該要把它獨立開來，用這邊來呼叫打字測驗的東東。
  *
  *     @author masks62009@gmail.com
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
  *     @brief 建立主視窗跟相關的一些元件
  *
  *     MainWindow的建構子，作為初始化整個視窗的開頭，他先呼叫 ui->setupUi()
  *     把 Qt Designer畫出來的 From給建立起來，再來是呼叫 Boshiamy的建構子
  *     這邊設定Boshiamy的字根對照表是『建議碼』，然後建立計時器跟定時器，把定時
  *     器的 signal跟 MainWindow的函式 void showTime()連接起來。
  *     設定 MainWindow的 Status bar把 status這個 Label給連過去。
  *     設定一些基本常數，清空鍵盤暫存，如果外掛的打字範本不存在就呼叫原始的打字範本
  *     呼叫 drawLayout()完成所有畫圖的工作。
  */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	check = new Boshiamy();
        check->setStatus(B_Standard);
	timer = new QTimer(this);
	time = new QTime();
	connect( timer,SIGNAL( timeout() ),this,SLOT( showTime() ) );
	status = new QLabel();
	statusBar()->addPermanentWidget(status);
        Col = 9;                                                        ///< 預設每行是 10個;
	Buff = "";
/// 這底下是後來加上去的。
	QString filename = QDir::currentPath() + QDir::separator() + "text.txt";
	QFile file(filename);
	if( file.open(QIODevice::ReadOnly) )
	{
		QTextStream in(&file);
		QString SampleText = in.readAll();
		setText(SampleText);
	}
	else{
		setText(Sample_Text);
	}
	At = 0;
	correct = 0,incorrect = 0,recorrect=0;
	drawLayout();
}

/**
  *     @brief 供定時器呼叫的函式
  *
  *     每 1 秒固定更新一次 Status Bar的內容
  */
void MainWindow::showTime()
{
	QString out = "正確：  "+QString::number(correct+recorrect)+" 字  錯字：  "+QString::number(incorrect)+" 字  平均每分鐘 "+QString::number( (correct * 60000)/( time->elapsed() ) )+" 字" ;
	status->setText(out);
}

/**
  *     @brief 設定打字範本的單字集合
  *
  *     呼叫 QStringList text2List() 將傳入的字串變成單字集合，放入打字範本的集合裡
  *
  *     @param QString text 傳入的打字範本字串
  */
void MainWindow::setText(QString text)
{
	Text = text2List(text);
}

/**
  *     @brief 將字串變成單字的集合
  *
  *     將傳入的字串轉換成一個一個單字的字集
  *
  *     @param QString text 輸入的中文字串
  *     @return 傳回單字的集合
  */
QStringList MainWindow::text2List(QString text)
{
	QStringList tmp;
	for(int i = 0 ; i<( text.size() ); i++)
	{
                tmp.append(text.data()[i]);                             ///< QString 的每個單字，依照 TextCodec的設定，可以用 QString::data()[]取得每一個字的字元，在 Utf-8下可以取出一個中文字，在預設下會取出一個 ascii字元
	}
	return tmp;
}

/**
  *     @brief 畫出每個元件，並將所有打字範本字集的單字排入 Grid網格中
  *
  *     這邊是靠手刻出來的，每個數字都還有待微調，這不是確定版本就是了…
  *     它先建立了一個捲動區，將捲動區的內部設成 widget，再把 widget的佈局設定成 Grid
  *     底下有一行是在設定Grid框架每一個欄位的邊界，跟欄寬，再用最笨的方法把每個單字給
  *     排上 Grid去，當一行填滿之後，就會補上一整行的空白 QLabel，並把這些 QLabel放進
  *     LabelList裡，供之後的鍵盤事件來處理填字的事情
  *
  */
void MainWindow::drawLayout()
{
	QFont font,smallfont;
	font.setPointSize(26);
	smallfont.setPointSize(13);
	Scroll = new QScrollArea(ui->centralWidget);
	Scroll->setGeometry( QRect(0, 0, 650, 380) );
	Scroll->setWidgetResizable(true);
	QWidget *widget = new QWidget(Scroll);
	widget->setGeometry( QRect(0, 0, 650, 380) );
	Grid =new QGridLayout(widget);

	for (int i = 0 ; i < Col ; i++)
	{
		Grid->setColumnMinimumWidth(i,40);
		Grid->setMargin(5);
	}
	int row = 0,
	    col = -1;
	foreach(QString str, Text)
	{
		QLabel * label = new QLabel();
		label->setText(str);
		label->setFont(font);
		if(col < Col)
		{
			col++;
		}
		else
		{
			row++;
			for(int i = 0 ; i <=Col ; i++)
			{
				QLabel * textLabel = new QLabel();
				textLabel->setFont(smallfont);
				Grid->addWidget(textLabel,row,i,Qt::AlignLeft);
				LabelList.append(textLabel);
			}
			row++,col=0;
		}
		Grid->addWidget(label,row,col,Qt::AlignLeft);
	}
	for(int i = 0 ; i <=col ; i++)
	{
		QLabel *textLabel = new QLabel();
		textLabel->setFont(smallfont);
		Grid->addWidget(textLabel,(row+1),i,Qt::AlignLeft);
		LabelList.append(textLabel);
	}
	Scroll->setWidget(widget);
	widget->setLayout(Grid);
}

/**
  *     @brief 打字完成之後顯示的畫面
  *
  *     將打字結束之後的成果用 QMessageBox顯示出來，並切斷定時器的連結。
  */
void MainWindow::awakeEnd()
{
	int elapsed=time->restart();
	QString out = "正確：  "+QString::number(correct+recorrect)+" 字  錯字：  "+QString::number(incorrect)+" 字  平均每分鐘 "+QString::number( (correct * 60000)/(elapsed) )+" 字" ;
	disconnect( timer,SIGNAL( timeout() ),this,SLOT( showTime() ) );
	status->setText(out);
	QMessageBox::about(this,"練習完成",out);
}

/**
  *     @brief 解構子
  *
  *     沒什麼好談的…  就算什麼都沒 delete掉， Qt 的 GC也會把它通通關掉吧…
  */
MainWindow::~MainWindow()
{
	delete ui;
}
/**
  *     @brief 當軟體的地區屬性改變，呼叫 Qt Designer的翻譯出來
  *
  *     在這個程式沒用到…
  *
  *     @param QEvent *e Qt的事件傳遞信號
  */
void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch ( e->type() ) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
/**
  *     @brief 整個程式的運作都在這邊，包括計時器的啟動與鍵盤事件的反應
  *
  *     這邊是比較亂的地方，未來應該會更亂，每個視窗都會在這邊處理鍵盤事件的反應
  *     前面是檢查定時器是不是以經被啟動了，若沒有，則開始一秒一次的計時動作。
  *     因為所有的視窗反應都是按下按鍵才開始計時，所以這邊是完全不用改動的地方。
  *     底下開始是對鍵盤事件的反應，這邊會開始對應不同的視窗產生不同的反應，目前
  *     只有範本打字視窗一個，所以很單純。
  *     若是空白鍵，就作出比對鍵盤 Buffer區跟那個漢字的字碼是否相同，是則換下一
  *     個漢字，若否則看是錯幾次了，錯一次就清空 Buffer再試一次。若是錯兩次則直
  *     接輸出正確答案。 now永遠指向目前的打字 Label區，靠的是 At這個List指標
  *     如果是刪除或是倒退鍵，就刪掉Buffer裡一個字元，若是其它合法的字，就加上去
  *     直到超過最大字根數。
  *
  *     @param QKeyEvent *key Qt的鍵盤事件
  */
void MainWindow::keyPressEvent(QKeyEvent *key)
{
	static int count = 0 ;
	QLabel * now = LabelList.at(At);
	if( !( timer->isActive() ) )
	{
		timer->start(1000);
		time->start();
	}
	if(key->key() == Qt::Key_Space)
	{
		if( check->compare(Text.at(At),Buff) )
		{
			QString out;
			At++;
			if(count == 1)
			{
				out ="<font color = green>" + Buff.toUpper() + "</font>";
				recorrect++;
			}
			if(count == 0)
			{
				out="<font color = blue>" + Buff.toUpper() + "</font>";
				correct++;
			}
			now->setText(out);

			if ( At >= LabelList.count() )
			{
				awakeEnd();
                                At--;                                   ///< 把 At停止增加 ;
			}
                        /// 這一行很重要，它可以捲動捲軸區，把目前下一個打字視窗給拉到可視範圍中。
                        Scroll->ensureWidgetVisible( LabelList.at(At)  );  ///< 把下一個 Label給翻上來… ;
			Buff.clear();
			count = 0;
		}
		else
		{
			count++;
			if(count == Max_Count)
			{
				incorrect++;
				count = 0;
				Buff = check->getCode( Text.at(At) );
				At++;
			}
			QString out = "<font color = red>" + Buff.toUpper() + "</font>";
			now->setText(out);
			Buff.clear();
		}
	}
	else if( (key->key() == Qt::Key_Backspace) || (key->key() == Qt::Key_Delete) )
	{
		if( Buff.isEmpty() )
		{

		}
		else
		{
			Buff.remove( (Buff.size()-1),1 );
		}
		now->setText( Buff.toUpper() );
	}
	else if( (key->key() >=Qt::Key_A) && (key->key() <= Qt::Key_Z) )
	{
		if(Buff.size() >= Max_KeyCode)
		{
			///<  超過最大字根數了…  不增加 Buff，略過

		}
		else
		{
			Buff.append( key->text().toUpper() );
			now->setText( Buff.toUpper() );
		}
	}
	else if( (key->key() == Qt::Key_Comma) || (key->key() == Qt::Key_Period) || (key->key() == Qt::Key_BracketLeft) || (key->key() == Qt::Key_BracketRight) || (key->key() == Qt::Key_Apostrophe) || (key->key() == Qt::Key_Semicolon) || (key->key() == Qt::Key_Colon) )
	{
		if(Buff.size() > Max_KeyCode)
		{

		}
		else
		{
			Buff.append( key->text() );
			now->setText( Buff.toUpper() );
		}
	}
	else
	{

	}
}
