#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsqltablemodel.h>

#include <qtrpt.h>
#include <CommonClasses.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();


private slots:
    void on_add_clicked();

    void on_close_clicked();

    void on_save_clicked();

    void on_select_clicked();

    void on_print_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    void fillcombo();
    void fillTableView();

protected:
     bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // MAINWINDOW_H
