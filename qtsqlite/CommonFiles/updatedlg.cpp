#include "updatedlg.h"
#include "ui_updatedlg.h"

UpdateDlg::UpdateDlg(QString url,QWidget *parent) : QDialog(parent), ui(new Ui::UpdateDlg) {
    ui->setupUi(this);
    m_url = url;
    QObject::connect(ui->btnUpdate, SIGNAL(clicked()),this, SLOT(doDownload()));
}

void UpdateDlg::showThis(QStringList list) {
    ui->pb1->setValue(0);
    ui->pb2->setValue(0);
    ui->tableWidget->setColumnWidth(0,0);
    ui->tableWidget->setColumnWidth(1,300);
    ui->tableWidget->setRowCount(list.size());
    QTableWidgetItem *newItem;
    for (int i = 0; i < list.size(); ++i) {
       // QStringList t = list.at(i).split(";");

        newItem = new QTableWidgetItem(list.at(i));
        newItem->setCheckState(Qt::Checked);
        ui->tableWidget->setItem(i,1,newItem);
    }

    this->exec();
}

void UpdateDlg::doDownload() {
    QString dir = QCoreApplication::applicationDirPath();
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                         QCoreApplication::applicationDirPath(),
                                         QFileDialog::ShowDirsOnly
                                         | QFileDialog::DontResolveSymlinks);

    int checked = 0;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i,1)->checkState() == Qt::Checked) {
            checked += 1;
        }
    }
    ui->pb2->setMaximum(checked);
    DownloadManager dl1;

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i,1)->checkState() == Qt::Checked) {
            ui->pb1->setValue(0);
            QString url = m_url+ui->tableWidget->item(i,1)->text();
            QString fileName = ui->tableWidget->item(i,1)->text().simplified();
            int p = fileName.lastIndexOf("/");
            fileName = fileName.remove(0,p+1);
            dl1.fileName = dir+"/"+fileName;

            ui->lblInfo->setText(tr("Downloading %0. ..").arg(ui->tableWidget->item(i,1)->text()));
            dl1.setTarget(url.replace(" ","%20").simplified());
            QObject::connect(&dl1, SIGNAL(downloadingProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
            dl1.download(false);

            QEventLoop loop;
            connect(&dl1,SIGNAL(done()),&loop,SLOT(quit()));
            loop.exec();

            ui->pb2->setValue(ui->pb2->value()+1);
        }
    }


    emit this->accepted();
    this->close();
}

void UpdateDlg::downloadProgress(qint64 recieved, qint64 total) {
    ui->pb1->setValue(recieved);
    ui->pb1->setMaximum(total);
}

UpdateDlg::~UpdateDlg() {
    delete ui;
}
