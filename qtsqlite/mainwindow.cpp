#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QSqlDatabase>

#include <QApplication>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include <QSqlTableModel>
#include <QMessageBox>
#include <QtSql>


#include <QTableView>
#include <QTableWidget>

#include "qabstractitemmodel.h"

#include <QKeyEvent>
#include <QEvent>

#include <shamsi/qdateconvertor.h>

QSqlQueryModel *model2 = new QSqlQueryModel;
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","first");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    #ifdef __linux__
      db.setDatabaseName("./checkrecived.sqlite");
    #elif _WIN32
      db.setDatabaseName("..\\bcksqlite\\checkrecived.sqlite");
    #else
      db.setDatabaseName("..\\bcksqlite\\checkrecived.sqlite");
    #endif

      db.setDatabaseName("H:\\saeed\\SAEED PROJECT\\Qt\\sqlite\\bcksqlite\\checkrecived.sqlite");
    //db.setDatabaseName("..\\checkrecived.sqlite");
    db.open();
    QSqlQuery query(db);
    //QSqlQuery query; در کیوت از این کد هم برای استفاده ضمنی از کانکشن دیفالت می توان استفاده کرد اما در صورت استفاده از چندین بانک اطلاعاتی این کد باعث بروز مشکلاتی می شود بنابراین راه حل کلی تر بالا توصیه می شود

    query.exec("SELECT tblPartyAccount.PartyAccount as namefamilyparty,tblCheckIncoming.id,tblCheckIncoming.DescCheck"
               ", tblCheckIncoming.NameBank, tblCheckIncoming.NumberCheck, tblCheckIncoming.AmountCheck"
               ", tblCheckIncoming.DateIssuance,tblCheckIncoming.DateUsance,"
               " tblCheckIncoming.pass  "
               " FROM tblPartyAccount INNER JOIN tblCheckIncoming ON tblPartyAccount.[code] = tblCheckIncoming.[PartyAccount];");
    while (query.next())
    {
        qDebug() << "NUMBER" << query.value(0).toString()
                    << "TEXT" << query.value(1).toString();
    }

      fillTableView();
      fillcombo();
      ui->comboBox->installEventFilter(this);
      ui->checkBox->installEventFilter(this);
      ui->save->installEventFilter(this);
      ui->add->installEventFilter(this);
       //connect(ui->checkBox, SIGNAL(returnPressed), ui->save, SLOT(setFocus()));


}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
  if (target == ui->checkBox || target == ui->comboBox || target == ui->save || target == ui->add ) {
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);

            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key()== Qt::Key_Return) {
                if(target == ui->save){
                  ui->save->click();
                  //on_save_clicked();
                  return true;
                }
                else if(target == ui->add){
                   // on_add_clicked();
                    ui->add->click();
                    return true;
                }
                else{
                this->focusNextPrevChild(true);
                //this->focusNextChild();
                event->accept();
                return true;
                }
            }

        }

     }else{
  return QObject::eventFilter(target, event);
  }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_clicked()
{
    ui->lineEdit->setText("");ui->lineEdit_2->text()=""; ui->lineEdit_3->text()="";ui->lineEdit_4->text()="";
    ui->lineEdit_5->clear();ui->comboBox->setCurrentIndex(0);ui->lineEdit_6->clear(); ui->checkBox->setCheckState(Qt::Unchecked);
    ui->comboBox->setFocus();
}

void MainWindow::on_close_clicked()
{

}

void MainWindow::on_save_clicked()
{
   /* Here's an example of positional binding:

            QSqlQuery query;
            query.prepare("INSERT INTO employee (id, name, salary) "
                          "VALUES (?, ?, ?)");
            query.addBindValue(1001);
            query.addBindValue("Thad Beaumont");
            query.addBindValue(65000);
            query.exec();
            */
    QSqlQuery query;
    query.prepare("INSERT INTO tblCheckIncoming(id,DescCheck,NameBank,NumberCheck,AmountCheck,DateIssuance,PartyAccount,DateUsance,pass) "
                  "VALUES (NULL,:DescCheck, :NameBank, :NumberCheck, :AmountCheck,:DateIssuance,:PartyAccount,:DateUsance,:pass)");
    query.bindValue(":DescCheck", ui->lineEdit->text());
    query.bindValue(":NameBank", ui->lineEdit_2->text());
    query.bindValue(":NumberCheck", ui->lineEdit_3->text());
    query.bindValue(":AmountCheck", ui->lineEdit_4->text());
    query.bindValue(":DateIssuance", ui->lineEdit_5->text());
    query.bindValue(":PartyAccount", model2->record(ui->comboBox->currentIndex()).value("code"));
    query.bindValue(":DateUsance", ui->lineEdit_6->text());
    if(ui->checkBox->isChecked() )
     query.bindValue(":pass", -1); // -1 is true
    else
     query.bindValue(":pass", 0); // zero = false
    query.exec();
    fillTableView();
   /* QString v1=ui->comboBox->currentText();
     qint8 v2=ui->spinBox->value();
    ui->lineEdit->text();
    ui->lineEdit2->setText(str);
    ui->lineEdit_2->text()=str;*/

    QMessageBox::information(NULL,"توجه","اطلاعات به درستی ثبت گردید","Ok");
    ui->add->setFocus();
}

void MainWindow::on_select_clicked()
{

    QModelIndex index;

    index = ui->tableView->currentIndex();
    index = index.sibling(index.row(), 0); // 0 is the code of id column

  //   ui->lineEdit->setText((ui->comboBox->currentIndex()).toString());


  //ui->tableView->selectionModel()->currentIndex().row()


      qDebug() << "current value"<< model2->record(ui->comboBox->currentIndex()).value("code").toString()<<index.data().toString()
                  <<index.sibling(index.row(), 1).data().toString();

    //  ui->comboBox->currentIndex() = index.sibling(index.row(), 1).data().toString();
      ui->lineEdit->setText(index.sibling(index.row(), 2).data().toString());
      ui->lineEdit_2->setText(index.sibling(index.row(), 3).data().toString());
      ui->lineEdit_3->setText(index.sibling(index.row(), 4).data().toString());
      ui->lineEdit_4->setText(index.sibling(index.row(), 5).data().toString());
      ui->lineEdit_5->setText(index.sibling(index.row(), 6).data().toString());
      ui->lineEdit_6->setText(index.sibling(index.row(), 7).data().toString());

/*
    QSqlQuery query;
    query.prepare("update person set firstname = ? where id = ?");
    query.addBindValue(firstName);
    query.addBindValue(personId);
    return query.exec();*/
}
void MainWindow::fillcombo(){
    /*    QSqlQueryModel sqlAuthors;
        sqlAuthors.setQuery("SELECT nr, lname FROM authors;");
        ui.authorsComboBox->setModel(&sqlAuthors);
        //ui.authorsComboBox->setModelColumn(0);*/

  model2->setQuery("SELECT PartyAccount,code from tblPartyAccount",db);
  model2->setHeaderData(1,Qt::Horizontal, tr("code"));
 // model2->setHeaderData(0, Qt::Horizontal, tr("طرف حساب"));
  QTableView *view2 = new QTableView;

  view2->setSelectionBehavior(QAbstractItemView::SelectRows);
  view2->setSelectionMode(QAbstractItemView::SingleSelection);


  ui->comboBox->setModel(model2);
  ui->comboBox->setView(view2);
  view2->setColumnHidden(1, true);
  // view2->setColumnWidth(0,0);
  // --------->>here now for my combobox i am hidding " id coloumn " ....SO that showing only names in combobox.
}
void MainWindow::fillTableView(){
    //get the table

    //*** method two
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT tblCheckIncoming.id,tblPartyAccount.PartyAccount as namefamilyparty,tblCheckIncoming.DescCheck"
                    ", tblCheckIncoming.NameBank, tblCheckIncoming.NumberCheck, tblCheckIncoming.AmountCheck"
                    ", tblCheckIncoming.DateIssuance, tblCheckIncoming.DateUsance,"
                    " tblCheckIncoming.pass  "
                    " FROM tblPartyAccount INNER JOIN tblCheckIncoming ON tblPartyAccount.[code] = tblCheckIncoming.[PartyAccount];",db);

    //******** end method two

    /*** method firest
      model = new QSqlTableModel(this,db);

      model->setTable("tblCheckIncoming");
      model->setEditStrategy(QSqlTableModel::OnManualSubmit);
      model->select();
      */ //*end method first
  /*    model->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("کد"));
      model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("نام طرف حساب"));
      model->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("شرح چک"));
      model->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("نام بانک"));
      model->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("شماره چک"));
      model->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("مبلغ چک"));
      model->setHeaderData(6,Qt::Horizontal,QString::fromUtf8("تاریخ صدور چک"));
      model->setHeaderData(7,Qt::Horizontal,QString::fromUtf8("تاریخ سررسید چک"));
      model->setHeaderData(8,Qt::Horizontal,QString::fromUtf8("پاس شده"));*/

      ui->tableView->setModel(model);
   //   ui->tableView->hideColumn(0); for hideColumn

      /* for my work no Relate you!
      I have checked at it is

      table->verticalHeader()->hide();

      to get the vertical header and hide it. And

      table->setSelectionBehavior(QAbstractItemView::SelectRows)
      */
}

/*
Hi all! I solved my problem but I don't know if it is the best form to
get a one value of column in one selected row. I have a hide column in
the grid called "id" and I would like to get this value, so the code
is following:

void group::deleteRowTable(void)
{
   QVariant variant
   QModelIndex index;
   int rownumber;
   QSqlQuery *query = new QSqlQuery;


   index = group_ui->tableView->currentIndex();
   rownumber = index.row();
   index = index.sibling(rownumber, 0); // 0 is the code of id column

   variant = index.data();

   query->prepare("delete from dsm_group where id = ?");

   query->addBindValue(variant.toString());
   query->exec();
   refrashTableView();
}*/

/*bool MainWindow::eventFilter(QObject *obj, QEvent *event)
 {
     if (obj ==  ui->comboBox) {
         if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress )
         {
             QKeyEvent *keyEvent =(QKeyEvent*)(event);
             qDebug() << "Ate key press";
             ui->lineEdit->setFocus();
            // textEdit->setText("keyEvent->key()");
             return true;
         }
         else
         {
             return false;
         }
     } else {
         // pass the event on to the parent class
         return QMainWindow::eventFilter(obj, event);
     }
 }
 *///کار کرد اما من یه چیز دیگه ایی می خواستم

void MainWindow::on_print_clicked()
{
/*
// QSqlDatabase::removeDatabase("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("..\\bcksqlite\\example.sqlite");
    db.open();
  /*  QSqlQuery query;
    query.exec("SELECT books.title, books.year, books.rating, genres.name as genre, authors.name "
               " FROM books"
               " INNER JOIN genres ON genres.id = books.genre"
               " INNER JOIN authors ON authors.id = books.author;");
    while (query.next())
    {
        qDebug() << "NUMBER" << query.value(0).toString()
                    << "TEXT" << query.value(1).toString();
    }
    QSqlQuery *query = new QSqlQuery(db);*/
    #ifdef __linux__
         QString fileName = "./example10.xml";
    #elif _WIN32
         QString fileName = "..\\bcksqlite\\example10.xml";
    #else

    #endif


    QtRPT *report = new QtRPT(this);
    if (report->loadReport(fileName) == false) {
        qDebug()<<"Report's file not found";
    }
    report->printExec();
//report->
  //this->focusNextPrevChild(true);

  //return true;*/
}


void MainWindow::on_pushButton_clicked()
{
  // QDateConvertor shamsi = new QDateConvertor();
   //ui->lineEdit_5->setText(shamsi.Today());
}
