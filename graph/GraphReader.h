#ifndef GRAPHREADER_H
#define GRAPHREADER_H

#include <cstdio>
#include "FileTypes.h"
#include "ReaderBase.h"

class GraphBase;



/**
 * @brief The GraphReader class - This is reader class contain reader functions.
 */
class GraphReader : public ReaderBase
{
public:
    GraphReader(GraphBase& graph);
    ~GraphReader();

    bool readFile(const char* fileName);
    bool readFile(FILE* f, FileTypes::Type typeId);

protected:
    GraphBase* graph;

    bool readEdges(FILE* f, bool readValue);
    bool readBrackets(FILE* f, bool readValue);
};

#endif // GRAPHREADER_H
