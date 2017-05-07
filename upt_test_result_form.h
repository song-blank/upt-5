#ifndef UPT_TEST_RESULT_FORM_H
#define UPT_TEST_RESULT_FORM_H

#include <QWidget>

namespace Ui {
class UPT_Test_Result_Form;
}

class UPT_Test_Result_Form : public QWidget
{
    Q_OBJECT

public:
    explicit UPT_Test_Result_Form(QWidget *parent = 0);
    ~UPT_Test_Result_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::UPT_Test_Result_Form *ui;
};

#endif // UPT_TEST_RESULT_FORM_H
