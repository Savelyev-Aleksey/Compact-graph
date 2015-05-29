#ifndef PROJECTIONSREADER_H
#define PROJECTIONSREADER_H


#include <cstdio>
#include "FileTypes.h"
#include "ReaderBase.h"

class GraphBase;
class Projections;


class ProjectionsReader : public ReaderBase
{
public:
    ProjectionsReader(Projections& projections);

    bool readFile(const char* fileName);
    bool readProjections(FILE* fp, FileTypes::Type typeId);

protected:
    Projections* projections;
};

#endif // PROJECTIONSREADER_H
