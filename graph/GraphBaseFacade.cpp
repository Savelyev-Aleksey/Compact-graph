#include <cmath>


#include "GraphBaseFacade.h"

#include "GraphWriter.h"
#include "GraphReader.h"
#include "GraphGenerator.h"



GraphBaseFacade::GraphBaseFacade() : ReaderBase(), Worker(),
    graph(new GraphBase),
    radius(0u),
    diameter(0u),
    girth(0u)
{}



GraphBaseFacade::~GraphBaseFacade()
{
    delete graph;
}



void GraphBaseFacade::clear()
{
    graph->clear();
    fileName.clear();
    diameter = 0u;
    radius = 0u;
}



unsigned GraphBaseFacade::getRadius() const
{
    return radius;
}



unsigned GraphBaseFacade::getDiameter() const
{
    return diameter;
}



unsigned GraphBaseFacade::getGraphGirth() const
{
    return girth;
}



const std::string& GraphBaseFacade::getFileName() const
{
    return fileName;
}



/**
 * @brief GraphBaseFacade::updateFileName updates graph saved file name.
 * @param newName new file name were saved graph
 */
void GraphBaseFacade::updateFileName(const char *newName)
{
    fileName = newName;
}



bool GraphBaseFacade::isGraphEmpty() const
{
    return graph->isEmpty();
}



unsigned GraphBaseFacade::nodeCount() const
{
    return graph->nodeCount();
}



unsigned GraphBaseFacade::edgeCount() const
{
    return graph->edgeCount();
}



const InfoDeque* GraphBaseFacade::getAllInfo() const
{
    return graph->getAllInfo();
}



/**
 * @brief GraphBaseFacade::generateHypercube facade for GraphGenerator function
 * @param dimention - Hypercube dimention
 * @param weight - edge weight
 */
void GraphBaseFacade::generateHypercube(unsigned dimention, float weight)
{
    clear();
    GraphGenerator generator(*graph);
    startProcess(&generator);
    generator.generateHypercube(dimention, weight);
    completeProcess();
}



/**
 * @brief GraphBaseFacade::generateTorus - Facade for GraphGenerator function
 * @param bigRadius
 * @param smallRadius
 * @param weight - edge weight
 */
void GraphBaseFacade::generateTorus(unsigned bigRadius, unsigned smallRadius,
                               float weight)
{
    clear();
    GraphGenerator generator(*graph);
    startProcess(&generator);
    generator.generateTorus(bigRadius, smallRadius, weight);
    completeProcess();
}



/**
 * @brief GraphBaseFacade::getNodeDegreeStatistic - graph degree statistic
 * @return map of Node degree and matched count.
 */
UintMap* GraphBaseFacade::getNodeDegreeStatistic() const
{
    return graph->getNodeDegreeStatistic();
}



/**
 * @brief GraphBaseFacade::compactStatistic shrink statistic for readable view.
 * It shrink large data to limit lines. It used in minimum three lines data.
 * If map size less limit copy map returned.
 * If map size or limit less than 3 copy map returned.
 * @param map - large map to shrink
 * @param limit - limit lines for map
 * @return copy of map shrinked to limit size
 */
UintMap* GraphBaseFacade::compactStatistic(UintMap *map, unsigned short limit)
const
{
    if (!map || limit < 3)
    {
        return nullptr;
    }
    unsigned mapSize = map->size();
    if (mapSize < 3 || mapSize <= limit)
    {
        return new UintMap(*map);
    }
    UintMap* compact = new UintMap();

    unsigned count = mapSize - 2;
    unsigned short lines = limit - 2;
    unsigned i = mapSize - 1;
    unsigned pos, current = i;
    UintMap::const_reverse_iterator it = map->rbegin(), end = map->rend();
    UintMap::iterator compactIt;
    compact->insert(*it);

    for (++it, --i; it != end; ++it, --i)
    {
        pos = static_cast<unsigned>( std::ceil( (double) i / count * lines ) );
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



unsigned GraphBaseFacade::statisticParamSum(const UintMap *map) const
{
    if (!map)
    {
        return 0u;
    }
    unsigned sum = 0u;
    for (const auto &it : *map)
    {
        sum += it.second;
    }
    return sum;
}



const Node* GraphBaseFacade::findNode(unsigned nodeId) const
{
    return graph->getNode(nodeId);
}



/**
 * @brief GraphBaseFacade::findNearNode - Find nodes, where near node.
 * For example if node not found, find near nodes.
 * Search range nodeId - 5 .. nodeId + 5.
 * @param nodeId - center of search range.
 * @return deque with node id:
 * first no greater nodeId - 5
 * last no less nodeId + 5
 */
uint_vec* GraphBaseFacade::findNearNode(unsigned nodeId) const
{
    NodeMap* list = graph->getNodeMap();
    if (!list->size())
    {
        return nullptr;
    }

    unsigned start = nodeId < 5 ? 0u : nodeId - 5;
    unsigned end = nodeId > UINT32_MAX - 5 ? UINT32_MAX : nodeId + 5;
    auto itLess = list->lower_bound(start);
    if (itLess == list->end())
        --itLess;
    auto itMore = list->upper_bound(end);

    uint_vec* nodes = new uint_vec;
    for (; itLess != itMore; ++itLess)
    {
        nodes->push_back(itLess->first);
    }
    return nodes;
}



/**
 * @brief GraphBaseFacade::readFile - read file contain nodes.
 * This is class wrappers functions for read all graph base
 * file types used in this project.
 * @param fileName - string contain file name
 * @return true if read successful
 */
bool GraphBaseFacade::readFile(const char* fileName)
{
    bool result = false;

    GraphReader graphReader(*graph);
    graphReader.setOptions(options);
    Type typeId;
    FILE* f = graphReader.openFile(fileName, typeId);
    if (f)
    {
        clear();
        graph->scanInfo(f);
        result = graphReader.readFile(f, typeId);
        fclose(f);
    }
    lastError = graphReader.getLastError();

    if (result)
    {
        updateFileName(fileName);
    }
    return result;
}



bool GraphBaseFacade::readFile(FILE *f, Type typeId)
{
    GraphReader graphReader(*graph);
    graphReader.setOptions(options);
    clear();
    graph->scanInfo(f);
    bool result = graphReader.readFile(f, typeId);
    lastError = graphReader.getLastError();
    return result;
}



/**
 * @brief GraphBaseFacade::writeEdges - Facade function.
 * More info in GraphWriter::writeEdges. Update file name for graph.
 * If graph was readed from file, old file name will be overwriten.
 * @param fileName - file to save graph
 * @param options - available options look in GraphWriter::writeEdges
 * @return - true if file accessed to write and graph written.
 */
bool GraphBaseFacade::writeEdges(const char* fileName, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool result = writer.writeEdges(fileName, options);
    completeProcess();

    if (result)
        updateFileName(fileName);

    return result;
}


/**
 * @brief GraphBaseFacade::writeBracketsFlat - Facade function.
 * More info in GraphWriter::writeBracketsFlat.
 * @param fileName - file to save graph
 * @param options - available options look in GraphWriter::writeBracketsFlat
 * @return - true if file accessed to write and graph written.
 */
bool GraphBaseFacade::writeBracketsFlat(const char* fileName, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool result = writer.writeBracketsFlat(fileName, options);
    completeProcess();

    if (result)
        updateFileName(fileName);

    return result;
}



/**
 * @brief GraphBaseFacade::writeBrackets - Facade function.
 * More info in GraphWriter::writeBrackets.
 * @param fileName - file to save graph.
 * @param startNodeId
 * @param pathLimit
 * @param options - available options look in GraphWriter::writeBrackets.
 * @return - true if file accessed to write and graph written.
 */
bool GraphBaseFacade::writeBrackets(const char* fileName, cuint startNodeId,
                          cuint pathLimit, cuint options)
{
    GraphWriter writer(*graph);
    startProcess(&writer);
    bool res = writer.writeBrackets(fileName, startNodeId, pathLimit, options);
    completeProcess();

    if (res)
        updateFileName(fileName);

    return res;
}



GraphBaseFacade::Type GraphBaseFacade::getType(const char* type)
{
   return GraphReader::getType(type);
}



bool GraphBaseFacade::isCanRead(const char* typeStr)
{
    return GraphReader::isCanRead(typeStr);
}
