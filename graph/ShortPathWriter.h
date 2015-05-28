#ifndef SHORTPATHWRITER_H
#define SHORTPATHWRITER_H

#include "types.h"
#include "WriterBase.h"
#include "ShortPath.h"


class ShortPath;



class ShortPathWriter : public WriterBase
{
public:
    ShortPathWriter(ShortPath &shortPath);

    bool savePaths(const char* fileName, const NodeIdDeque* nodes = nullptr,
                   float pathLimit = 0, cuint options = (cuint) Option::NONE);

    bool writeExistPaths(const char* fileName, const NodeIdDeque* nodes,
                         cuint options = (cuint) Option::NONE);

protected:
    ShortPath* shortPath;

    void writeShortPath(FILE* fp, RootPathList::iterator pathIt,
                        cuint options = (cuint) Option::NONE);
};

#endif // SHORTPATHWRITER_H
