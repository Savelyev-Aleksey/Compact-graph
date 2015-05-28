#ifndef PROJECTIONSWRITER_H
#define PROJECTIONSWRITER_H


#include <cstddef>
#include <cstdio>

#include "types.h"
#include "WriterBase.h"


class Projections;
class Projection;



class ProjectionsWriter : public WriterBase
{
public:
    ProjectionsWriter(Projections& projections);
    ~ProjectionsWriter();

    bool saveProjections(const char* fileName,
                         cuint options = (cuint) Option::NONE);
    bool saveProjection(const char* fileName, size_t rootNode,
                        cuint options = (cuint) Option::NONE);

protected:
    Projections* projections;

    bool writeProjection(FILE* f, const Projection* projection,
         cuint options = (cuint) Option::NONE, bool isProgress = true);
};

#endif // PROJECTIONSWRITER_H
