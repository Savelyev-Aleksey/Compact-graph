#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>
#include <string>

#include "types.h"
#include "FileTypes.h"

#include "ReaderBase.h"
#include "Worker.h"

#include "GraphBaseFacade.h"
#include "ShortPathFacade.h"
#include "FileProjectionsFacade.h"



/**
 * @brief The Graph class is main facade for access in all method of graph.
 * Facade divides into sereveral logical facades:
 * GraphBaseFacade - facade for basic graph oprations (access, read, write)
 * ShortPathFacade - facade for short path oprations
 * FileProjectionsFacade - facade for projections operations
 *
 * Each small facade can be used as an individual interface. But for proper
 * work of ShortPathFacade and FileProjectionsFacade needs connected
 * GraphBaseFacade for operate with graph data.
 *
 * This class is recomends to connet with GUI class implentations.
 * For some special tasks this class can be an example for more specific
 * lighter class implemetation (example exclude short paths facade).
 */
class Graph :
        virtual public ReaderBase,
        virtual public Worker,
        public GraphBaseFacade,
        public ShortPathFacade,
        public FileProjectionsFacade
{
public:
    Graph();
    virtual ~Graph();

    virtual void clear();

    // Statistics
    UintMap* getEccentriciyStatisticByShortPaths() const;
    UintMap* getEccentriciyStatisticByProjections() const;

    // Reader
    static bool isCanRead(const char* typeStr);
    bool readFile(const char* fileName);

    // For ShortPath method
    void updateParametersByShortPaths();

    // For Projection method
    void updateParametersByProjections();

protected:
    // Reader for text files
    bool readTxtFile(const char* fileName);

    virtual void updateFileName(const char* newName);
};

#endif // GRAPH_H
