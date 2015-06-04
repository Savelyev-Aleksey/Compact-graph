#ifndef SHORTPATHREADER_H
#define SHORTPATHREADER_H


#include <cstdio>
#include "FileTypes.h"
#include "ReaderBase.h"

class ShortPath;


class ShortPathReader : public ReaderBase
{
public:
    ShortPathReader(ShortPath& shortPath);

    static const char* const types[];

    enum class Type : unsigned
    {
        // Type id must be in same order with types[]
        BRACKETS_SHORT_PATH_VALUE,
        // Always must be last - used in loop
        UNDEFINED
    };


    static bool isCanRead(const char* type);

    FILE* openFile(const char* fileName, Type& typeId);
    bool readShortPath(FILE* fp, Type typeId);

protected:
    ShortPath* shortPath;

};

#endif // SHORTPATHREADER_H
