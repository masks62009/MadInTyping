#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	check = new Boshiamy();
	check->setStatus(B_Complete);
	timer = new QTimer(this);
	time = new QTime();
	connect( timer,SIGNAL( timeout() ),this,SLOT( showTime() ) );
	status = new QLabel();
	statusBar()->addPermanentWidget(status);
	Col = 9; ///< 預設每行是 12個;
	Buff = "";
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

void MainWindow::showTime()
{
	QString out = "正確：  "+QString::number(correct+recorrect)+" 字  錯字：  "+QString::number(incorrect)+" 字  平均每分鐘 "+QString::number( (correct * 60000)/( time->elapsed() ) )+" 字" ;
	status->setText(out);
}

void MainWindow::setText(QString text)
{
	Text = text2List(text);
}

QStringList MainWindow::text2List(QString text)
{
	QStringList tmp;
	for(int i = 0 ; i<( text.size() ); i++)
	{
		tmp.append(text.data()[i]);
	}
	return tmp;
}

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
		//Grid->setColumnStretch(i,1);
		Grid->setMargin(5);
	}
	int row = 0,
	    col = -1;
	foreach(QString str, Text)
	{
		QLabel * label = new QLabel();
		label->setText(str);
		label->setFont(font);
		//label->setGeometry(0,0,45,30);
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
				//textLabel->setGeometry(0,0,45,30);
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
	//ui->scrollAreaWidgetContents->setLayout(Grid);
	Scroll->setWidget(widget);
	widget->setLayout(Grid);
}

void MainWindow::awakeEnd()
{
	int elapsed=time->restart();
	QString out = "正確：  "+QString::number(correct+recorrect)+" 字  錯字：  "+QString::number(incorrect)+" 字  平均每分鐘 "+QString::number( (correct * 60000)/(elapsed) )+" 字" ;
	disconnect( timer,SIGNAL( timeout() ),this,SLOT( showTime() ) );
	status->setText(out);
	QMessageBox::about(this,"練習完成",out);
}

MainWindow::~MainWindow()
{
	delete ui;
}

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
				At--; ///< 把 At停止增加 ;
			}
			Scroll->ensureWidgetVisible( LabelList.at(At) );  ///< 把下一個 Label給翻上來… ;
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
