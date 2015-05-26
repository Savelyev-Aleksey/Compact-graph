#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>

#include "types.h"
#include "FileTypes.h"

#include "ReaderBase.h"
#include "WriterBase.h"
#include "GraphBase.h"

#include "Worker.h"

#include "ShortPath.h"

class Node;
class ShortPath;
class Projections;
class Projection;

class Graph : public ReaderBase, public Worker
{
public:
    Graph();
    virtual ~Graph();

    // Common methods

    size_t getRadius() const;
    size_t getDiameter() const;

    void updateParameters();

    // Statistics
    UlongMap* getNodeDegreeStatistic() const;
    UlongMap* getEccentriciyStatistic() const;
    UlongMap* compactStatistic(UlongMap* map, unsigned short limit) const;
    size_t statisticParamSum(const UlongMap* map) const;

    // Readers
    bool readFile(const char* fileName);
    bool readFile(FILE* f, FileTypes::Type typeId);


public:
    // GraphBase methods

    bool isGraphEmpty() const;
    size_t nodeCount() const;
    size_t edgeCount() const;

    const InfoDeque* getAllInfo() const;

    // Generators
    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

    // Node finders
    const Node* findNode(size_t nodeId) const;
    std::deque<size_t>* findNearNode(size_t nodeId) const;


    // Graph writers
    virtual bool writeEdges(const char* fileName, cuint options =
                    WriterBase::PRINT_VALUE );

    virtual bool writeBracketsFlat(const char* fileName, cuint options =
                            WriterBase::PRINT_VALUE );

    virtual bool writeBrackets(const char* fileName, const size_t startNodeId,
                       const size_t pathLimit = 0,
                       cuint options = WriterBase::PRINT_VALUE);

public:
    // ShortPath methods

    bool isShortPathEmpty() const;
    bool isPathExist(size_t nodeId) const;
    size_t getShortPathCount() const;

    virtual void generateAllShortPaths(float pathLimit = 0);

    // ShortPath writers
    bool saveShortPaths(const char* fileName, const NodeIdDeque* nodes =nullptr,
                        float pathLimit = 0, cuint options = Option::NONE);

    bool writeExistShortPaths(const char* fileName, const NodeIdDeque* nodes,
                              cuint options = Option::NONE);

public:
    // Projections methods

    void createAllProjections();
    void createProjection(size_t nodeId);

    bool isProjectionExist(size_t nodeId) const;
    size_t projectionsCount() const;

    const Projection* getProjection(size_t nodeId) const;

    // Projections writers
    virtual bool saveProjections(const char* fileName,
                                 unsigned options = Option::NONE);
    virtual bool saveProjection(const char* fileName, size_t rootNode,
                                unsigned options = Option::NONE);


protected:
    GraphBase* graph;
    ShortPath* shortPath;
    Projections* projections;

    size_t radius;
    size_t diameter;

    void clearGraph();

};

#endif // GRAPH_H
