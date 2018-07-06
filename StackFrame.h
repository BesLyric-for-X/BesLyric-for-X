#ifndef STACKFRAME_H
#define STACKFRAME_H

#include "BesFramelessWidget.h"

class StackFrame : public BesFramelessWidget
{
    Q_OBJECT

public:
    StackFrame(QWidget *parent = 0);
    ~StackFrame();
};

#endif // STACKFRAME_H
