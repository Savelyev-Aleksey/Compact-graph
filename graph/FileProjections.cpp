#include <algorithm>


#include "FileProjections.h"
#include "GraphBase.h"
#include "ProjectionsReader.h"
#include "ProjectionsWriter.h"



namespace {
    /**
     * @brief memoryNodeLimit limit for graph projections before that
     * all projections are operated in memory, more nodes must store
     * all projections in files and operate by one projection.
     */
    const unsigned memoryEdgeLimit = 1000;
}



FileProjections::FileProjections(GraphBase& graph) :
    Projections(graph),
    ReaderBase(),
    loadedProjection(nullptr),
    projectionStatus(Status::NONE),
    useMemory(graph.edgeCount() <= memoryEdgeLimit)
{}



FileProjections::~FileProjections()
{}



FileProjections::Status FileProjections::getStatus() const
{
    return projectionStatus;
}



const std::string& FileProjections::getFileName() const
{
    return graphFileName;
}



void FileProjections::updateGraphFileName(const std::string& newName)
{
    graphFileName = newName;
    auto end = graphFileName.find_last_of('.');
    if (end == std::string::npos)
        end = graphFileName.size();
    graphFileName.resize(end);
    graphFileName.shrink_to_fit();
}



const Projection* FileProjections::getProjection(unsigned nodeId)
{
    lastError = Error::NONE;

    if (loadedProjection && loadedProjection->getId() == nodeId)
        return loadedProjection;

    if (useMemory)
    {
        loadedProjection = Projections::getProjection(nodeId);
        return loadedProjection;
    }

    auto pr = Projections::getProjection(nodeId);
    if (!pr)
        return nullptr;

    if (loadedProjection)
    {
        loadedProjection->clear();
        loadedProjection = pr;
    }
    if (loadProjection(pr))
    {
        loadedProjection = pr;
        return pr;
    }
    loadedProjection = nullptr;
    return nullptr;
}



bool FileProjections::loadProjection(Projection *pr)
{
    if (!pr)
        return false;

    unsigned id = pr->getId();
    ProjectionsReader reader(*this);
    std::string prName(graphFileName);
    prName += "_pr_";
    prName += std::to_string(id);
    prName +=  ".txt";
    ProjectionsReader::Type typeId;
    bool result = false;
    FILE* f = reader.openFile(prName.data(), typeId);
    if (f)
    {
        result = reader.readProjections(f, typeId);
        fclose(f);
    }
    lastError = reader.getLastError();
    return result;
}



const Projection* FileProjections::getLoadedProjection() const
{
    return loadedProjection;
}




void FileProjections::createAllProjections()
{
    if (graph->isEmpty())
    {
        return;
    }

    if(useMemory)
    {
        Projections::createAllProjections();
        return;
    }

    // Prepare nodes
    NodeMap* nodeList = graph->getNodeMap();
    unsigned count = nodeList->size();

    // all projections exist - skip
    if (projectionsList->size() == count)
        return;

    unsigned oldSize = projectionsList->size();
    bool isWasEmpty = !oldSize;

    projectionsList->resize(count, nullptr);
    projectionsList->shrink_to_fit();

    auto oldStart = projectionsList->begin();
    auto oldEnd = projectionsList->begin() + oldSize;
    auto it = nodeList->begin();
    Projection* pr;

    ProjectionsWriter writer(*this);
    std::string saveName(graphFileName);
    unsigned nameSize = saveName.size();

    startProcess(0, count - 1);

    unsigned currentId;
    unsigned pos = oldSize;

    for(unsigned i = 0; i < count; ++i, ++it)
    {
        if (isInterrupted())
            return;

        updateProgress(i);

        currentId = it->first;
        pr = nullptr;
        // Projection exist - skip
        if (!isWasEmpty)
        {
            auto e = std::lower_bound(oldStart, oldEnd, currentId,
                                      Projection::lessById);
            if (e != oldEnd && (*e)->getId() == currentId)
            {
                pr = *e;
                if (pr->fileExist())
                    continue;
            }
        }
        if (!pr)
        {
            pr = new Projection(currentId);
            (*projectionsList)[pos] = pr;
            ++pos;
        }
        pr->createProjection(*graph);

        saveName.resize(nameSize);
        saveName += "_pr_";
        saveName += std::to_string(currentId);
        saveName += ".txt";
        writer.saveProjection(saveName.data(), currentId);

        // stay loaded last projection
        if (count - i > 1)
        {
            pr->clear();
        }
        else
        {
            loadedProjection = pr;
        }
    }

    if (!isWasEmpty)
    {
        auto end = projectionsList->end();
        std::sort(oldStart, end, Projection::less);
    }
    completeProcess();
}



const Projection* FileProjections::createProjection(unsigned nodeId)
{
    if (useMemory)
    {
        return Projections::createProjection(nodeId);
    }

    if (loadedProjection)
    {
        if(loadedProjection->getId() != nodeId)
        {
            loadedProjection->clear();
            loadedProjection = nullptr;
        }
        else if (!loadedProjection->isEmpty())
            return loadedProjection;
    }
    const Projection* pr = Projections::createProjection(nodeId);
    loadedProjection = const_cast<Projection*>(pr);
    return pr;
}



/**
 * @brief FileProjections::findShortPaths finds paths between fromId and toId
 * nodes Functions returns all available not intersected paths same length.
 * @param fromId - node id for seach paths.
 * @param toId - node id for seatch paths
 * @param reverse - by default path order from root elem to find elem
 * @return nullptr if no paths else vector of paths. path is vector of ids.
 */
ProjShortPaths* FileProjections::findShortPaths(unsigned fromId, unsigned toId,
                                                bool reverse)
{
    if(useMemory)
    {
        return Projections::findShortPaths(fromId, toId, reverse);
    }
    if (loadedProjection)
    {
        if (loadedProjection->getId() == fromId)
        {
            return Projections::findShortPaths(fromId, toId, reverse);
        }
        else if(loadedProjection->getId() == toId)
        {
            return Projections::findShortPaths(toId, fromId, !reverse);
        }
        else
        {
            loadedProjection->clear();
            loadedProjection = nullptr;
        }
    }
    auto pr = Projections::getProjection(fromId);
    if (!pr)
    {
        return nullptr;
    }
    bool res = loadProjection(pr);
    if (!res)
    {
        return nullptr;
    }
    return Projections::findShortPaths(fromId, toId, reverse);
}



bool FileProjections::isProjectionExist(unsigned nodeId) const
{
    if (useMemory)
    {
        return Projections::isProjectionExist(nodeId);
    }
    Projection* pr = nullptr;
    if (loadedProjection && loadedProjection->getId() == nodeId)
        pr = loadedProjection;
    else
        pr = Projections::getProjection(nodeId);

    return !pr ? false : pr->fileExist();
}



bool FileProjections::isMemoryUsed() const
{
    return useMemory;
}
