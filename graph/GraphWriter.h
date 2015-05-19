#ifndef GRAPHWRITER_H
#define GRAPHWRITER_H

#include <cstdio>
#include "FileTypes.h"
#include "WriterBase.h"


class GraphBase;

class GraphWriter : public WriterBase
{
public:
    GraphWriter(const GraphBase &graph);
    ~GraphWriter();

    bool writeEdges(const char* fileName,
                    const unsigned options = Option::PRINT_VALUE);
    bool writeBracketsFlat(const char* fileName,
                           const unsigned options = Option::PRINT_VALUE);
    bool writeBrackets(const char* fileName, const size_t startNodeId,
                       const size_t pathLimit = 0,
                       const unsigned options = Option::PRINT_VALUE);

protected:
    const GraphBase* graph;

    FILE* openFile(const char* fileName);
    bool isNodeVisited(const std::deque<size_t> &list, size_t nodeId) const;
};

#endif // GRAPHWRITER_H
