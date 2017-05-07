#ifndef AUTO_TEST_FORM_H
#define AUTO_TEST_FORM_H

#include <QWidget>

namespace Ui {
class auto_Test_Form;
}

class auto_Test_Form : public QWidget
{
    Q_OBJECT

public:
    explicit auto_Test_Form(QWidget *parent = 0);
    ~auto_Test_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::auto_Test_Form *ui;
};

#endif // AUTO_TEST_FORM_H
