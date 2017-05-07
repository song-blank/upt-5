#include "upt_mainwindow.h"
#include <QApplication>
#include "form.h"
#include <QTextCodec>
#include <QDebug>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    UPT_MainWindow w;
    Form w;
    w.show();

   QTextCodec *codec = QTextCodec::codecForName("UTF-8");
   QTextCodec::setCodecForLocale(codec);
   QTextCodec::setCodecForCStrings(codec);
   QTextCodec::setCodecForTr(codec);
   QFont font("wenquanyi",6);
   a.setFont(font);

    return a.exec();
}
