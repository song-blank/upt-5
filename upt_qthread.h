#ifndef UPT_QTHREAD_H
#define UPT_QTHREAD_H

#include "upt_qthread.h"
#include <QThread>


/*****************************
用于各单元函数创建阻塞式线程，并将线程执行结果以带参数的信号的方式发送出去。
*****************************/

class UPT_QThread : public QThread
{
    Q_OBJECT
public:
    UPT_QThread();
    UPT_QThread(int cmd, void *param);
    UPT_QThread(int cmd, void *in_param, void *out_param);

protected:
    void run();

signals:
    void cmd_finished_signal(int cmd, int status);

private:
    int _cmd;
    void *_param;
    void *_in_param, *_out_param;
    int status;
};


#endif // IL_QTHREAD_H





