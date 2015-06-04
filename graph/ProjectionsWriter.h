#ifndef PROJECTIONSWRITER_H
#define PROJECTIONSWRITER_H


#include <cstddef>
#include <cstdio>

#include "types.h"
#include "WriterBase.h"


class FileProjections;
class Projection;



class ProjectionsWriter : public WriterBase
{
public:
    ProjectionsWriter(FileProjections& projections);
    ~ProjectionsWriter();

    bool saveProjections(const char* fileName,
                         cuint options = (cuint) Option::NONE);
    bool saveProjection(const char* fileName, unsigned rootNode,
                        cuint options = (cuint) Option::NONE);

protected:
    FileProjections* projections;

    bool writeProjection(FILE* f, const Projection* projection,
         cuint options = (cuint) Option::NONE, bool isProgress = true);

    void writeParameters(FILE* f, const Projection* pr);
};

#endif // PROJECTIONSWRITER_H
