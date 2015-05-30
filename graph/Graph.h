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

    typedef const WriterBase::Option WOption;

    // Common methods

    unsigned getRadius() const;
    unsigned getDiameter() const;
    unsigned getGraphGirth() const;

    void clearGraph();

    // Statistics
    UintMap* getNodeDegreeStatistic() const;
    UintMap* getEccentriciyStatisticByShortPaths() const;
    UintMap* getEccentriciyStatisticByProjections() const;
    UintMap* compactStatistic(UintMap* map, unsigned short limit) const;
    unsigned statisticParamSum(const UintMap* map) const;

    // Readers
    bool readFile(const char* fileName);
    bool readFile(FILE* f, FileTypes::Type typeId);


public:
    // GraphBase methods

    bool isGraphEmpty() const;
    unsigned nodeCount() const;
    unsigned edgeCount() const;

    const InfoDeque* getAllInfo() const;

    // Generators
    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

    // Node finders
    const Node* findNode(unsigned nodeId) const;
    uint_vec* findNearNode(unsigned nodeId) const;


    // Graph writers
    virtual bool writeEdges(const char* fileName, cuint options =
                            (cuint) WOption::PRINT_VALUE );

    virtual bool writeBracketsFlat(const char* fileName, cuint options =
                                   (cuint) WOption::PRINT_VALUE );

    virtual bool writeBrackets(const char* fileName, cuint startNodeId,
                               cuint pathLimit = 0,
                               cuint options = (cuint) WOption::PRINT_VALUE);

public:
    // ShortPath methods

    void updateParametersByShortPaths();

    bool isShortPathEmpty() const;
    bool isPathExist(unsigned nodeId) const;
    unsigned shortPathsCount() const;

    virtual void generateAllShortPaths(float pathLimit = 0);

    // ShortPath writers
    bool saveShortPaths(const char* fileName, const NodeIdDeque* nodes =nullptr,
         float pathLimit = 0, cuint options = (cuint) WOption::NONE);

    bool writeExistShortPaths(const char* fileName, const NodeIdDeque* nodes,
                              cuint options = (cuint) WOption::NONE);

public:
    // Projections methods

    void updateParametersByProjections();

    void createProjection(unsigned nodeId);

    bool isProjectionExist(unsigned nodeId) const;
    unsigned projectionsCount() const;

    const Projection* getProjection(unsigned nodeId) const;

    virtual void createAllProjections();

    // Projections writers
    virtual bool saveProjections(const char* fileName,
                                 cuint options = (cuint) WOption::NONE);
    virtual bool saveProjection(const char* fileName, unsigned rootNode,
                                cuint options = (cuint) WOption::NONE);


protected:
    GraphBase* graph;
    ShortPath* shortPath;
    Projections* projections;

    unsigned radius;
    unsigned diameter;
    unsigned girth;

};

#endif // GRAPH_H
