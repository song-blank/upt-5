#ifndef OLD_BAR_CODE_FORM_H
#define OLD_BAR_CODE_FORM_H

#include <QWidget>

namespace Ui {
class old_Bar_Code_Form;
}

class old_Bar_Code_Form : public QWidget
{
    Q_OBJECT

public:
    explicit old_Bar_Code_Form(QWidget *parent = 0);
    ~old_Bar_Code_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::old_Bar_Code_Form *ui;
};

#endif // OLD_BAR_CODE_FORM_H
