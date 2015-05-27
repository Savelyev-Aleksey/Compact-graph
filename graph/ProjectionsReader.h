#ifndef PROJECTIONSREADER_H
#define PROJECTIONSREADER_H


#include <cstdio>
#include "FileTypes.h"
#include "ReaderBase.h"
#include "Projections.h"

class GraphBase;


class ProjectionsReader : public ReaderBase, public Projections
{
public:
    ProjectionsReader(GraphBase& graph);

    bool readFile(const char* fileName);
    bool readProjections(FILE* fp, FileTypes::Type typeId);

protected:
};

#endif // PROJECTIONSREADER_H
