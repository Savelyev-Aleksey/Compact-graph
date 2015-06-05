#include "FileProjectionsFacade.h"
#include "GraphBase.h"
#include "FileProjections.h"
#include "ProjectionsWriter.h"
#include "Projection.h"



FileProjectionsFacade::FileProjectionsFacade(GraphBase& graph) :
    ReaderBase(),
    Worker(),
    projections(new FileProjections(graph))
{}



FileProjectionsFacade::~FileProjectionsFacade()
{}



void FileProjectionsFacade::clear()
{
    projections->clear();
}



/**
 * @brief FileProjectionsFacade::createProjection
 * create one projection for nodeId node.
 * @param nodeId - node for which will create projection
 */
void FileProjectionsFacade::createProjection(unsigned nodeId)
{
    projections->createProjection(nodeId);
}



bool FileProjectionsFacade::isProjectionExist(unsigned nodeId) const
{
    return projections->isProjectionExist(nodeId);
}



unsigned FileProjectionsFacade::projectionsCount() const
{
    return projections->size();
}



const Projection* FileProjectionsFacade::getProjection(unsigned nodeId)
{
    return projections->getProjection(nodeId);
}



const Projection* FileProjectionsFacade::getLoadedProjection() const
{
    return projections->getLoadedProjection();
}



/**
 * @brief FileProjectionsFacade::createAllProjections creates projection
 * for each node in graph.
 */
void FileProjectionsFacade::createAllProjections()
{
    startProcess(projections);
    if (projections->getStatus() != FileProjections::Status::ALL)
        projections->createAllProjections();
    completeProcess();
}



/**
 * @brief FileProjectionsFacade::findShortPaths finds paths between
 * fromId and toId  nodes Functions returns all available
 * not intersected paths same length.
 * @param fromId - node id for seach paths.
 * @param toId - node id for seatch paths
 * @param reverse - by default path order from root elem to find elem
 * @return nullptr if no paths else vector of paths. path is vector of ids.
 */
ProjShortPaths* FileProjectionsFacade::
                findShortPaths(unsigned fromId, unsigned toId,bool reverse)
{
    startProcess(projections);
    auto res = projections->findShortPaths(fromId, toId, reverse);
    completeProcess();
    return res;
}



/**
 * @brief FileProjectionsFacade::saveProjections save all created projections
 * in fileName file.
 * @param fileName - file to write projections
 * @param options - some options (PRINT_INDENTS)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool FileProjectionsFacade::saveProjections(const char* fileName, cuint options)
{
    ProjectionsWriter writer(*projections);
    startProcess(&writer);
    bool result = writer.saveProjections(fileName, options);
    completeProcess();
    return result;
}



/**
 * @brief FileProjectionsFacade::saveProjection - save created projection for
 * one node in fileName file.
 * @param fileName - file to write projections
 * @param rootNode - node id for create projection
 * @param options - some options (PRINT_INDENTS)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool FileProjectionsFacade::saveProjection(const char* fileName,
                                           unsigned rootNode, cuint options)
{
    ProjectionsWriter writer(*projections);
    startProcess(&writer);
    bool result = writer.saveProjection(fileName, rootNode, options);
    completeProcess();
    return result;
}



void FileProjectionsFacade::updateGraphFileName(const std::string& newName)
{
    projections->updateGraphFileName(newName);
}



FileProjectionsFacade::Type FileProjectionsFacade::getType(const char* typeStr)
{
    return ProjectionsReader::getType(typeStr);
}



bool FileProjectionsFacade::isCanRead(const char *typeStr)
{
    return ProjectionsReader::isCanRead(typeStr);
}



bool FileProjectionsFacade::readFile(const char* fileName)
{
    bool result = false;

    ProjectionsReader projectionsReader(*projections);
    projectionsReader.setOptions(options);
    Type typeId;
    FILE* f = projectionsReader.openFile(fileName, typeId);
    if (f)
    {
        clear();
        result = projectionsReader.readProjections(f, typeId);
        fclose(f);
    }
    lastError = projectionsReader.getLastError();

    return result;
}



bool FileProjectionsFacade::readFile(FILE *f, Type typeId)
{
    ProjectionsReader projectionsReader(*projections);
    projectionsReader.setOptions(options);
    bool result = false;
    if (f)
    {
        clear();
        result = projectionsReader.readProjections(f, typeId);
        fclose(f);
        lastError = projectionsReader.getLastError();
    }
    return result;
}



bool FileProjectionsFacade::isProjectionsMemoryUsed() const
{
    return projections->isMemoryUsed();
}

