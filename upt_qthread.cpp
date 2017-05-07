#include "upt_qthread.h"


UPT_QThread::UPT_QThread()
{

}

UPT_QThread::UPT_QThread(int cmd, void *param)
{
    _cmd = cmd;
    _param = param;
}

UPT_QThread::UPT_QThread(int cmd, void *in_param, void *out_param)
{
    _cmd = cmd;
    _in_param = in_param;
    _out_param = out_param;
}

void UPT_QThread::run()
{
    emit cmd_finished_signal(_cmd, status);
}
