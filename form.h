#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_configure_PBn_clicked();

    void on_powerButton_clicked();

    void on_auto_test_PBn_clicked();

    void on_mannal_test_PBn_clicked();

    void on_check_history_PBn_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
