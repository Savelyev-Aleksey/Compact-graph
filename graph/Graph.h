#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>

#include "types.h"
#include "FileTypes.h"
#include "ReaderBase.h"
#include "WriterBase.h"
#include "GraphBase.h"
#include "ShortPath.h"


class Node;

class Graph : public ReaderBase
{
public:
    Graph();
    ~Graph();

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

    bool isGraphEmpty();

    const InfoDeque* getAllInfo() const;

    // Generators
    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

    // Node finders
    const Node* findNode(size_t nodeId) const;
    std::deque<size_t>* findNearNode(size_t nodeId) const;


    // Graph writers
    bool writeEdges(const char* fileName, const unsigned options =
                    WriterBase::Option::PRINT_VALUE) const;

    bool writeBracketsFlat(const char* fileName, const unsigned options =
                           WriterBase::Option::PRINT_VALUE) const;

    bool writeBrackets(const char* fileName, const size_t startNodeId,
                       const size_t pathLimit = 0, const unsigned options =
                       WriterBase::Option::PRINT_VALUE) const;

public:
    // ShortPath methods

    bool isShortPathEmpty();
    bool isPathExist(size_t nodeId);
    size_t getShortPathCount() const;

    void generateAllShortPaths(float pathLimit = 0);

    // ShortPath writers
    bool saveShortPaths(const char* fileName, const NodeIdDeque* nodes =nullptr,
                   float pathLimit = 0, unsigned options = Option::NONE) const;

    bool writeExistShortPaths(const char* fileName, const NodeIdDeque* nodes,
                         unsigned options = Option::NONE) const;


protected:
    GraphBase* graph;
    ShortPath* shortPath;

    size_t radius;
    size_t diameter;

    void clearGraph();

};

#endif // GRAPH_H
