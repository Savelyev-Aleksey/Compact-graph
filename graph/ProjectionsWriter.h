#ifndef PROJECTIONSWRITER_H
#define PROJECTIONSWRITER_H


#include <cstddef>
#include <cstdio>

#include "types.h"
#include "WriterBase.h"
#include "Projection.h"


class FileProjections;



class ProjectionsWriter : public WriterBase
{
public:
    ProjectionsWriter(FileProjections& projections);
    ~ProjectionsWriter();

    bool saveProjections(const char* fileName,
                         cuint options = (cuint) Option::NONE);
    bool saveProjection(const char* fileName, const Projection* pr,
                        cuint options = (cuint) Option::NONE);

    bool saveShortPaths(const char* fileName, const ProjShortPaths* paths);

protected:
    FileProjections* projections;

    bool writeProjection(FILE* f, const Projection* projection,
         cuint options = (cuint) Option::NONE, bool isProgress = true);

    void writeParameters(FILE* f, const Projection* pr);
};

#endif // PROJECTIONSWRITER_H
