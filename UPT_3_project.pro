######################################################################
# Automatically generated by qmake (2.01a) ?? 4? 20 16:59:32 2017
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += auto_check_fault_form.h \
           auto_check_form.h \
           auto_test_form.h \
           configure_form.h \
           cyusb.h \
           form.h \
           old_bar_code_form.h \
           quality_control_form.h \
           serial_device_controller.h \
           test_info_input_form.h \
           test_result_history_form.h \
           UPT_global.h \
           upt_mainwindow.h \
           upt_qthread.h \
           upt_test_result_form.h \
           usb_device_controller.h
FORMS += auto_check_fault_form.ui \
         auto_check_form.ui \
         auto_test_form.ui \
         configure_form.ui \
         form.ui \
         old_bar_code_form.ui \
         quality_control_form.ui \
         test_info_input_form.ui \
         test_result_history_form.ui \
         upt_mainwindow.ui \
         upt_test_result_form.ui
SOURCES += auto_check_fault_form.cpp \
           auto_check_form.cpp \
           auto_test_form.cpp \
           configure_form.cpp \
           form.cpp \
           main.cpp \
           old_bar_code_form.cpp \
           quality_control_form.cpp \
           serial_device_controller.cpp \
           test_info_input_form.cpp \
           test_result_history_form.cpp \
           UPT_global.cpp \
           upt_mainwindow.cpp \
           upt_qthread.cpp \
           upt_test_result_form.cpp \
           usb_device_controller.cpp
RESOURCES += ico.qrc image.qrc

LIBS += -L`pwd`/lib -lcyusb
