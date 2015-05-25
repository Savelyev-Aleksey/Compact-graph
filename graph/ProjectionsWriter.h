#ifndef PROJECTIONSWRITER_H
#define PROJECTIONSWRITER_H


#include <cstddef>
#include <cstdio>

#include "WriterBase.h"


class Projections;
class Projection;



class ProjectionsWriter : public WriterBase
{
public:
    ProjectionsWriter(Projections& projections);
    ~ProjectionsWriter();

    bool saveProjections(const char* fileName, unsigned options = Option::NONE);
    bool saveProjection(const char* fileName, size_t rootNode,
                        unsigned options = Option::NONE);

protected:
    Projections* projections;

    bool writeProjection(FILE* f, const Projection* projection,
                         unsigned options = Option::NONE);
};

#endif // PROJECTIONSWRITER_H
