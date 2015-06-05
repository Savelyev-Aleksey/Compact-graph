#include <cstdint>
#include <iostream>
#include <functional>
#include <cmath>

#include "Graph.h"

#include "GraphReader.h"
#include "GraphWriter.h"
#include "GraphGenerator.h"

#include "ShortPathRootElem.h"
#include "ShortPathReader.h"
#include "ShortPathWriter.h"

#include "Projection.h"
#include "FileProjections.h"
#include "ProjectionsWriter.h"
#include "ProjectionsReader.h"



Graph::Graph() :
    ReaderBase(),
    Worker(),
    GraphBaseFacade(),
    ShortPathFacade(*graph),
    FileProjectionsFacade(*graph)
{}



Graph::~Graph()
{
    delete shortPath;
    delete projections;
    delete graph;
}



void Graph::clear()
{
    GraphBaseFacade::clear();
    ShortPathFacade::clear();
    FileProjectionsFacade::clear();
}



/**
 * @brief Graph::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UintMap* Graph::getEccentriciyStatisticByShortPaths() const
{
    return shortPath->getEccentriciyStatistic();
}



UintMap* Graph::getEccentriciyStatisticByProjections() const
{
    return projections->getEccentriciyStatistic();
}



/**
 * @brief Graph::updateParametersByShortPaths - update calculating graph
 * parameters by short paths info.
 * There are radius and diameter.
 */
void Graph::updateParametersByShortPaths()
{
    RootPathList *nodes = shortPath->getShortPathNodes();
    if (!nodes->size())
    {
        return;
    }
    auto it = nodes->begin(), end = nodes->end();
    unsigned min, max, eccentr;
    min = max = it->second->getEccentricity();
    for (++it; it != end; ++it)
    {
        eccentr = it->second->getEccentricity();
        if (eccentr > max)
            max = eccentr;
        else if (eccentr < min)
        {
            min = eccentr;
        }
    }
    radius = min;
    diameter = max;
}



/**
 * @brief Graph::updateParametersByProjections - update calculating graph
 * parameters by projections info.
 * There are radius and diameter.
 */
void Graph::updateParametersByProjections()
{
    const ProjectionsList *nodes = projections->getList();
    if (!nodes->size())
    {
        return;
    }
    auto it = nodes->begin(), end = nodes->end();
    unsigned min, max, eccentr;
    min = max = (*it)->getEccentricity();
    for (++it; it != end; ++it)
    {
        eccentr = (*it)->getEccentricity();
        if (eccentr > max)
            max = eccentr;
        else if (eccentr < min)
        {
            min = eccentr;
        }
    }
    radius = min;
    diameter = max;
    girth = projections->getGraphGirth();
}



/**
 * @brief Graph::readFile - read file contain nodes. If file not recognised
 * error will logged. This is class wrappers functions for read all file types
 * used in this project.
 * @param fileName - string contain file name
 * @return true if read successful
 */
bool Graph::readFile(const char* fileName)
{
    // if other file formats will be added
    // add file type definition here
    return readTxtFile(fileName);
}



bool Graph::readTxtFile(const char *fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        lastError = Error::READ;
        std::clog << "[!!!] Error: Can't read file (" << fileName << ")\n";
        return false;
    }

    char typeStr[200];
    fgets(typeStr, 200, f);

    bool result = false;
    bool found = false;

    auto graphTypeId = GraphBaseFacade::getType(typeStr);
    if (graphTypeId != GraphBaseFacade::Type::UNDEFINED)
    {
        clear();
        found = true;
        result = GraphBaseFacade::readFile(f, graphTypeId);
        if (result)
        {
            FileProjectionsFacade::updateGraphFileName(
                                   GraphBaseFacade::fileName);
        }
    }
    if (!found)
    {
        auto type = ShortPathFacade::getType(typeStr);
        if (type != ShortPathFacade::Type::UNDEFINED)
        {
            clear();
            found = true;
            result = ShortPathFacade::readFile(f, type);
        }
    }
    if (!found)
    {
        auto type = FileProjectionsFacade::getType(typeStr);
        if (type != FileProjectionsFacade::Type::UNDEFINED)
        {
            clear();
            found = true;
            result = FileProjectionsFacade::readFile(f,type);
        }
    }
    fclose(f);
    if (!found)
    {
        lastError = Error::TYPE;
    }
    return result;
}



bool Graph::isCanRead(const char *typeStr)
{
    if (GraphBaseFacade::isCanRead(typeStr))
        return true;
    if (ShortPathFacade::isCanRead(typeStr))
        return true;
    if (FileProjectionsFacade::isCanRead(typeStr))
        return true;

    return false;
}
