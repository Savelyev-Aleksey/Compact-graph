#ifndef SHORTPATHWRITER_H
#define SHORTPATHWRITER_H

#include "WriterBase.h"
#include "ShortPath.h"


class ShortPath;



class ShortPathWriter : public WriterBase
{
public:
    ShortPathWriter(ShortPath &shortPath);

    bool savePaths(const char* fileName, const NodeIdDeque* nodes = nullptr,
                   float pathLimit = 0, unsigned options = Option::NONE);

    bool writeExistPaths(const char* fileName, const NodeIdDeque* nodes,
                         unsigned options = Option::NONE);

protected:
    ShortPath* shortPath;

    void writeShortPath(FILE* fp, RootPathList::iterator pathIt,
                        unsigned options = Option::NONE);
};

#endif // SHORTPATHWRITER_H
