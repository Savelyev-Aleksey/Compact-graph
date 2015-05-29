
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
#include "Projections.h"
#include "ProjectionsWriter.h"
#include "ProjectionsReader.h"



Graph::Graph() :
    ReaderBase(),
    Worker(),
    graph(new GraphBase()),
    shortPath(new ShortPath(*graph)),
    projections(new Projections(*graph)),
    radius(0),
    diameter(0)
{ }



Graph::~Graph()
{
    delete shortPath;
    delete projections;
    delete graph;
}


void Graph::clearGraph()
{
    shortPath->clear();
    projections->clear();
    graph->clear();
    diameter = 0;
    radius = 0;
}



size_t Graph::getRadius() const
{
    return radius;
}



size_t Graph::getDiameter() const
{
    return diameter;
}



/**
 * @brief Graph::getNodeDegreeStatistic - Return graph degree statistic.
 * @return map of Node degree and matched count.
 */
UlongMap* Graph::getNodeDegreeStatistic() const
{
    return graph->getNodeDegreeStatistic();
}



/**
 * @brief Graph::getEccentriciyStatistic - create eccentricity statistic.
 * Show eccentricity and mathed count in graph.
 * @return map with eccentricity and matched count.
 */
UlongMap* Graph::getEccentriciyStatisticByShortPaths() const
{
    return shortPath->getEccentriciyStatistic();
}


UlongMap* Graph::getEccentriciyStatisticByProjections() const
{
    return projections->getEccentriciyStatistic();
}



/**
 * @brief Graph::compactStatistic - Shrink statistic for readable view.
 * It shrink large data to limit lines. It used in minimum three lines data.
 * If map size less limit copy map returned.
 * If map size or limit less than 3 copy map returned.
 * @param map - large map to shrink
 * @param limit - limit lines for map
 * @return copy of map shrinked to limit size
 */
UlongMap* Graph::compactStatistic(UlongMap *map, unsigned short limit) const
{
    if (!map || limit < 3)
    {
        return nullptr;
    }
    size_t mapSize = map->size();
    if (mapSize < 3 || mapSize <= limit)
    {
        return new UlongMap(*map);
    }
    UlongMap* compact = new UlongMap();

    size_t count = mapSize - 2;
    unsigned short lines = limit - 2;
    size_t i = mapSize - 1;
    size_t pos, current = i;
    UlongMap::const_reverse_iterator it = map->rbegin(), end = map->rend();
    UlongMap::iterator compactIt;
    compact->insert(*it);

    for (++it, --i; it != end; ++it, --i)
    {
        pos = static_cast<size_t>( std::ceil( (double) i / count * lines ) );
        if (pos != current)
        {
            current = pos;
            // Insert new elemnt in list
            compactIt = compact->insert(*it).first;
        }
        else
        {
            // update inserted element with nearest elements
            compactIt->second += it->second;
        }
    }
    // Insert first element
    return compact;
}



size_t Graph::statisticParamSum(const UlongMap *map) const
{
    if (!map)
    {
        return 0;
    }
    size_t sum = 0;
    for (auto it = map->begin(), end = map->end(); it != end; ++it)
    {
        sum += it->second;
    }
    return sum;
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
    size_t min, max, eccentr;
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
    size_t min, max, eccentr;
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
 * @brief Graph::writeEdges - Facade function.
 * More info in GraphWriter::writeEdges.
 * @param fileName - file to save graph
 * @param options - available options look in GraphWriter::writeEdges
 * @return - true if file accessed to write and graph written.
 */
bool Graph::writeEdges(const char* fileName, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool result = writer.writeEdges(fileName, options);
    completeProcess();
    return result;
}


/**
 * @brief Graph::writeBracketsFlat - Facade function.
 * More info in GraphWriter::writeBracketsFlat.
 * @param fileName - file to save graph
 * @param options - available options look in GraphWriter::writeBracketsFlat
 * @return - true if file accessed to write and graph written.
 */
bool Graph::writeBracketsFlat(const char* fileName, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool result = writer.writeBracketsFlat(fileName, options);
    completeProcess();
    return result;
}



/**
 * @brief writeBrackets - Facade function.
 * More info in GraphWriter::writeBrackets.
 * @param fileName - file to save graph.
 * @param startNodeId
 * @param pathLimit
 * @param options - available options look in GraphWriter::writeBrackets.
 * @return - true if file accessed to write and graph written.
 */
bool Graph::writeBrackets(const char* fileName, const size_t startNodeId,
                          const size_t pathLimit, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool res = writer.writeBrackets(fileName, startNodeId, pathLimit, options);
    completeProcess();
    return res;
}



/**
 * @brief Graph::generateHypercube - Facade for GraphGenerator function
 * @param dimention - Hypercube dimention
 * @param weight - edge weight
 */
void Graph::generateHypercube(unsigned dimention, float weight)
{
    clearGraph();
    GraphGenerator generator(*graph);
    generator.generateHypercube(dimention, weight);
}



/**
 * @brief Graph::generateTorus - Facade for GraphGenerator function
 * @param bigRadius
 * @param smallRadius
 * @param weight - edge weight
 */
void Graph::generateTorus(unsigned bigRadius, unsigned smallRadius,float weight)
{
    clearGraph();
    GraphGenerator generator(*graph);
    generator.generateTorus(bigRadius, smallRadius, weight);
}



bool Graph::isGraphEmpty() const
{
    return graph->isEmpty();
}



size_t Graph::nodeCount() const
{
    return graph->nodeCount();
}



size_t Graph::edgeCount() const
{
    return graph->edgeCount();
}



bool Graph::isShortPathEmpty() const
{
    return shortPath->isEmpty();
}



bool Graph::isPathExist(size_t nodeId) const
{
    return shortPath->isPathExist(nodeId);
}



const Node* Graph::findNode(size_t nodeId) const
{
    return graph->getNode(nodeId);
}



size_t Graph::shortPathsCount() const
{
    return shortPath->getShortPathCount();
}



void Graph::generateAllShortPaths(float pathLimit)
{
    startProcess(shortPath);
    shortPath->generateAllShortPaths(pathLimit);
    completeProcess();
}



/**
 * @brief Graph::findNearNode - Find nodes, where near node.
 * For example if node not found, find near nodes.
 * Search range nodeId - 5 .. nodeId + 5.
 * @param nodeId - center of search range.
 * @return deque with node id:
 * first no greater nodeId - 5
 * last no less nodeId + 5
 */
size_vec* Graph::findNearNode(size_t nodeId) const
{
    NodeMap* list = graph->getNodeMap();
    if (!list->size())
    {
        return nullptr;
    }

    size_t start = nodeId < 5 ? 0 : nodeId - 5;
    size_t end = nodeId > SIZE_MAX - 5 ? SIZE_MAX : nodeId + 5;
    auto itLess = list->lower_bound(start);
    if (itLess == list->end())
        --itLess;
    auto itMore = list->upper_bound(end);

    size_vec* nodes = new size_vec;
    for (; itLess != itMore; ++itLess)
    {
        nodes->push_back(itLess->first);
    }
    return nodes;
}



const InfoDeque* Graph::getAllInfo() const
{
    return graph->getAllInfo();
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
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        lastError = Error::READ;
        std::clog << "[!!!] Critical: Can't read file (" << fileName << ")\n";
        return false;
    }

    char typeStr[200];
    fgets(typeStr, 200, f);
    FileTypes::Type typeId = FileTypes::typeId(typeStr);
    if (typeId == FileTypes::Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!!!] Critical: File type is unknown " << typeStr
                  << std::endl;
        fclose(f);
        return false;
    }

    clearGraph();

    graph->scanInfo(f);
    bool result = readFile(f, typeId);
    fclose(f);

    if (result)
    {
        lastError = Error::NONE;
        std::clog << "File (" << fileName << ") readed" << std::endl;
    }

    graph->logStatus();

    return result;
}



/**
 * @brief Graph::readFile - read file known format.
 * It's can read graph writen by node node edge value or in brackets style.
 * It's can read short path graph file.
 * @param f - FILE handle
 * @param typeId - file type id
 * @return true if file readed successful
 */
bool Graph::readFile(FILE *f, FileTypes::Type typeId)
{
    bool result;

    bool outWarning = options & (unsigned) Option::OUT_WARNINGS;

    switch (typeId)
    {
    case FileTypes::Type::NODE_NODE:
    case FileTypes::Type::NODE_NODE_VALUE:
    case FileTypes::Type::BRACKETS:
    case FileTypes::Type::BRACKETS_VALUE:
    case FileTypes::Type::BRACKETS_FLAT:
    case FileTypes::Type::BRACKETS_FLAT_VALUE:
        {
            GraphReader graphReader(*graph);
            graphReader.setOptions(options);
            result = graphReader.readFile(f, typeId);
            lastError = graphReader.getLastError();
        }
        break;

    case FileTypes::Type::BRACKETS_SHORT_PATH_VALUE:
        {
            ShortPathReader shortPathReader(*shortPath);
            shortPathReader.setOptions(options);
            result = shortPathReader.readShortPath(f, typeId);
            lastError = shortPathReader.getLastError();
        }
        break;

    case FileTypes::Type::PROJECTIONS:
        {
            ProjectionsReader projectionsReader(*projections);
            projectionsReader.setOptions(options);
            result = projectionsReader.readProjections(f, typeId);
            lastError = projectionsReader.getLastError();
        }
        break;

    case FileTypes::Type::UNDEFINED:
        // no break
    default:
        if (outWarning)
        {
            const char* s;
            if (typeId == FileTypes::Type::UNDEFINED)
                s = "UNDEFINED";
            else
                s = FileTypes::typeName(typeId);

            std::clog << "[!] Warning: Graph can't read this file type ("
                      << s << "), " << "use other class implementations."
                      << std::endl;
        }
        lastError = Error::TYPE;
        result = false;
        break;
    }
    return result;
}




/**
 * @brief Graph::saveShortPaths - Write in file paths. Create if not exist.
 * @param fileName - file name for save paths
 * @param nodes - deque of nodes id for generate and save path into file
 * @param pathLimit - path weight limit
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if save successful
 */
bool Graph::saveShortPaths(const char* fileName, const NodeIdDeque* nodes,
                      float pathLimit, cuint options)
{
    ShortPathWriter writer(*shortPath);
    return writer.savePaths(fileName, nodes, pathLimit, options);
}



/**
 * @brief Graph::writeExistShortPaths - Save in file already generated
 * short paths for nodes from node list.
 * @param fileName - file name where save paths
 * @param nodes - deque list of node for save path
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if write successful
 */
bool Graph::writeExistShortPaths(const char* fileName, const NodeIdDeque* nodes,
                                 cuint options)
{
    ShortPathWriter writer(*shortPath);
    return writer.writeExistPaths(fileName, nodes, options);
}



/**
 * @brief Graph::createAllProjections creates projection
 * for each node in graph.
 */
void Graph::createAllProjections()
{
    startProcess(projections);
    projections->createAllProjections();
    completeProcess();
}



/**
 * @brief Graph::createProjection create one projection for nodeId node.
 * @param nodeId - node for which will create projection
 */
void Graph::createProjection(size_t nodeId)
{
    projections->createProjection(nodeId);
}



bool Graph::isProjectionExist(size_t nodeId) const
{
    return projections->isProjectionExist(nodeId);
}



size_t Graph::projectionsCount() const
{
    return projections->size();
}



/**
 * @brief Graph::saveProjections - save all created projections
 * in fileName file.
 * @param fileName - file to write projections
 * @param options - some options (PRINT_INDENTS)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool Graph::saveProjections(const char* fileName, cuint options)
{
    ProjectionsWriter writer(*projections);
    startProcess(&writer);
    bool result = writer.saveProjections(fileName, options);
    completeProcess();
    return result;
}



/**
 * @brief Graph::saveProjection - save created projection for
 * one node in fileName file.
 * @param fileName - file to write projections
 * @param rootNode - node id for create projection
 * @param options - some options (PRINT_INDENTS)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool Graph::saveProjection(const char* fileName, size_t rootNode,
                           cuint options)
{
    ProjectionsWriter writer(*projections);
    startProcess(&writer);
    bool result = writer.saveProjection(fileName, rootNode, options);
    completeProcess();
    return result;
}



const Projection* Graph::getProjection(size_t nodeId) const
{
    return projections->getProjection(nodeId);
}
