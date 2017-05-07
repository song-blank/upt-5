#ifndef QUALITY_CONTROL_FORM_H
#define QUALITY_CONTROL_FORM_H

#include <QWidget>

namespace Ui {
class Quality_Control_Form;
}

class Quality_Control_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Quality_Control_Form(QWidget *parent = 0);
    ~Quality_Control_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::Quality_Control_Form *ui;
};

#endif // QUALITY_CONTROL_FORM_H
