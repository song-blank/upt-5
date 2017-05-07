#ifndef TEST_RESULT_HISTORY_FORM_H
#define TEST_RESULT_HISTORY_FORM_H

#include <QWidget>

namespace Ui {
class test_Result_History_Form;
}

class test_Result_History_Form : public QWidget
{
    Q_OBJECT

public:
    explicit test_Result_History_Form(QWidget *parent = 0);
    ~test_Result_History_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::test_Result_History_Form *ui;
};

#endif // TEST_RESULT_HISTORY_FORM_H
