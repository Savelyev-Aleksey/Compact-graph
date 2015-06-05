#ifndef GRAPHREADER_H
#define GRAPHREADER_H

#include <cstdio>
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

    static const char* const types[];

    enum class Type : unsigned
    {
        // Type id must be in same order with types[]
        NODE_NODE,
        NODE_NODE_VALUE,
        BRACKETS_FLAT,
        BRACKETS_FLAT_VALUE,
        BRACKETS,
        BRACKETS_VALUE,
        // Always must be last - used in loop
        UNDEFINED
    };

    static Type getType(const char* type);
    static bool isCanRead(const char* type);

    FILE* openFile(const char* fileName, Type& typeId);
    bool readFile(FILE* f, Type typeId);

protected:
    GraphBase* graph;


    bool readEdges(FILE* f, bool readValue);
    bool readBrackets(FILE* f, bool readValue);
};

#endif // GRAPHREADER_H
