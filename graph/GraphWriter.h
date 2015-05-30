#ifndef GRAPHWRITER_H
#define GRAPHWRITER_H

#include <cstdio>
#include "types.h"
#include "FileTypes.h"
#include "WriterBase.h"


class GraphBase;

class GraphWriter : public WriterBase
{
public:
    GraphWriter(const GraphBase &graph);
    ~GraphWriter();

    bool writeEdges(const char* fileName,
                    cuint options = (cuint) Option::PRINT_VALUE);
    bool writeBracketsFlat(const char* fileName,
                           cuint options = (cuint) Option::PRINT_VALUE);
    bool writeBrackets(const char* fileName, cuint startNodeId,
                       cuint pathLimit = 0,
                       cuint options = (cuint) Option::PRINT_VALUE);

protected:
    const GraphBase* graph;

    FILE* openFile(const char* fileName);
    bool isNodeVisited(const std::deque<unsigned> &list, unsigned nodeId) const;
};

#endif // GRAPHWRITER_H
