#ifndef WRITERBASE_H
#define WRITERBASE_H

#include "Worker.h"

class WriterBase : public Worker
{
public:
    enum Option
    {
        NONE            = 0x00,
        PRINT_INFO      = 0x01,
        PRINT_VALUE     = 0x02,
        PRINT_INDENTS   = 0x04
        //              = 0x08
        //              = 0x10
        //              = 0x20
    };
    WriterBase();
};

#endif // WRITERBASE_H
