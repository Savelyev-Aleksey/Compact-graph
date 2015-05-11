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

    bool readFile(const char* fileName);
    bool readShortPath(FILE* fp, FileTypes::Type typeId);

protected:
    ShortPath* shortPath;

};

#endif // SHORTPATHREADER_H
