#include "ShortPathFacade.h"

#include "ShortPathReader.h"
#include "ShortPathWriter.h"
#include "GraphBase.h"



ShortPathFacade::ShortPathFacade(GraphBase& graph) :
    ReaderBase(),
    Worker(),
    shortPath(new ShortPath(graph))
{}



ShortPathFacade::~ShortPathFacade()
{
    delete shortPath;
}



void ShortPathFacade::clear()
{
    shortPath->clear();
}



bool ShortPathFacade::isShortPathEmpty() const
{
    return shortPath->isEmpty();
}



bool ShortPathFacade::isPathExist(unsigned nodeId) const
{
    return shortPath->isPathExist(nodeId);
}



unsigned ShortPathFacade::shortPathsCount() const
{
    return shortPath->getShortPathCount();
}



void ShortPathFacade::createAllShortPaths(float pathLimit)
{
    startProcess(shortPath);
    shortPath->createAllShortPaths(pathLimit);
    completeProcess();
}



/**
 * @brief ShortPathFacade::saveShortPaths write in file paths.
 * Create if not exist.
 * @param fileName - file name for save paths
 * @param nodes - deque of nodes id for generate and save path into file
 * @param pathLimit - path weight limit
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if save successful
 */
bool ShortPathFacade::saveShortPaths(const char* fileName,
                                     const NodeIdDeque* nodes,
                                     float pathLimit, cuint options)
{
    ShortPathWriter writer(*shortPath);
    return writer.savePaths(fileName, nodes, pathLimit, options);
}



/**
 * @brief ShortPathFacade::writeExistShortPaths save in file already generated
 * short paths for nodes from node list.
 * @param fileName - file name where save paths
 * @param nodes - deque list of node for save path
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if write successful
 */
bool ShortPathFacade::writeExistShortPaths(const char* fileName,
                                      const NodeIdDeque* nodes, cuint options)
{
    ShortPathWriter writer(*shortPath);
    return writer.writeExistPaths(fileName, nodes, options);
}



bool ShortPathFacade::readFile(const char* fileName)
{
    bool result = false;

    ShortPathReader shortPathReader(*shortPath);
    shortPathReader.setOptions(options);
    Type typeId;
    FILE* f = shortPathReader.openFile(fileName, typeId);
    if (f)
    {
        clear();
        result = shortPathReader.readShortPath(f, typeId);
        fclose(f);
    }
    lastError = shortPathReader.getLastError();

    return result;
}



bool ShortPathFacade::readFile(FILE *f, Type typeId)
{
    ShortPathReader shortPathReader(*shortPath);
    shortPathReader.setOptions(options);
    bool result = false;
    if (f)
    {
        clear();
        result = shortPathReader.readShortPath(f, typeId);
        fclose(f);
    }
    lastError = shortPathReader.getLastError();

    return result;
}

