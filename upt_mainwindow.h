#ifndef UPT_MAINWINDOW_H
#define UPT_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class UPT_MainWindow;
}

class UPT_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UPT_MainWindow(QWidget *parent = 0);
    ~UPT_MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UPT_MainWindow *ui;
};

#endif // UPT_MAINWINDOW_H
