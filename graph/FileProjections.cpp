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
    const unsigned memoryEdgeLimit = 2500;
}



FileProjections::FileProjections(GraphBase& graph) :
    Projections(graph),
    ReaderBase(),
    loadedProjection(nullptr),
    projectionStatus(Status::NONE)
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



Projection* FileProjections::getProjection(unsigned nodeId)
{
    lastError = Error::NONE;

    if (loadedProjection && loadedProjection->getId() == nodeId)
        return loadedProjection;

    if (isMemoryUsed())
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
    startProcess(&reader, 0u, pr->getEccentricity());

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
    completeProcess();
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

    if(isMemoryUsed())
    {
        Projections::createAllProjections();
        return;
    }

    // Prepare nodes
    NodeMap* nodeList = graph->getNodeMap();
    unsigned count = nodeList->size();

    if (!count)
        return;

    ProjectionsWriter writer(*this);
    setWorker(&writer, true);
    startProcess(0u, count - 1);

    unsigned oldSize = projectionsList->size();
    bool isWasEmpty = !oldSize;

    projectionsList->resize(count, nullptr);
    projectionsList->shrink_to_fit();

    auto oldStart = projectionsList->begin();
    auto oldEnd = projectionsList->begin() + oldSize;
    auto it = nodeList->begin();
    Projection* pr;

    std::string saveName(graphFileName);
    unsigned nameSize = saveName.size();

    unsigned currentId;
    unsigned pos = oldSize;

    for(unsigned i = 0u; i < count; ++i, ++it)
    {
        if (isInterrupted())
        {
            projectionStatus = Status::PARTIAL;
            // if interrupted on null filled projections, shrink list
            projectionsList->resize(pos);
            break;
        }

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
        if (pr->isInterrupted())
        {
            continue;
        }

        ProjectionsReader::projectionFileName(saveName, nameSize, currentId);

        bool result = writer.saveProjection(saveName.data(), pr);

        if (result)
        {
            pr->setFileExist(true);
        }
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
    if (!isInterrupted())
        projectionStatus = Status::ALL;
    completeProcess();
}



Projection* FileProjections::createProjection(unsigned nodeId)
{
    if (isMemoryUsed())
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
    Projection* pr = Projections::createProjection(nodeId);
    loadedProjection = pr;
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
    if(isMemoryUsed())
    {
        return Projections::findShortPaths(fromId, toId, reverse);
    }
    if (loadedProjection)
    {
        if (loadedProjection->getId() == fromId)
        {
            return loadedProjection->findShortPaths(toId, reverse);
        }
        else if(loadedProjection->getId() == toId)
        {
            return loadedProjection->findShortPaths(fromId, !reverse);
        }
        else
        {
            loadedProjection->clear();
            loadedProjection = nullptr;
        }
    }
    auto pr = getProjection(fromId);
    if (!pr)
    {
        return nullptr;
    }
    return pr->findShortPaths(toId, reverse);
}



bool FileProjections::isProjectionExist(unsigned nodeId) const
{
    if (isMemoryUsed())
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
    return graph->edgeCount() <= memoryEdgeLimit;
}



void FileProjections::readProjectionsInfo()
{
    // Prepare nodes
    NodeMap* nodeList = graph->getNodeMap();
    unsigned count = nodeList->size();

    if (!count)
        return;

    unsigned oldSize = projectionsList->size();
    bool isProjectionsWasEmpty = !oldSize;

    projectionsList->resize(count, nullptr);
    projectionsList->shrink_to_fit();

    auto oldStart = projectionsList->begin();
    auto oldEnd = projectionsList->begin() + oldSize;
    auto it = nodeList->begin();
    Projection* pr;

    ProjectionsReader reader(*this);
    std::string saveName(graphFileName);
    unsigned nameSize = saveName.size();
    bool someMissing = false, someReaded = false;

    startProcess(0u, count - 1);

    unsigned currentId;
    unsigned pos = oldSize;

    for(unsigned i = 0u; i < count; ++i, ++it)
    {
        if (isInterrupted())
        {
            projectionStatus = Status::PARTIAL;
            return;
        }

        updateProgress(i);

        currentId = it->first;
        pr = nullptr;
        // Projection exist - skip
        if (!isProjectionsWasEmpty)
        {
            auto e = std::lower_bound(oldStart, oldEnd, currentId,
                                      Projection::lessById);
            if (e != oldEnd && (*e)->getId() == currentId)
            {
                pr = *e;
            }
        }
        if (!pr)
        {
            pr = new Projection(currentId);
            (*projectionsList)[pos] = pr;
            ++pos;
        }

        ProjectionsReader::projectionFileName(saveName, nameSize, currentId);

        bool result = reader.readProjectionInfo(saveName.data(), pr);
        if (result)
        {
            someReaded = true;
        }
        else
        {
            someMissing = true;
            if (reader.getLastError() == ProjectionsReader::Error::TYPE)
            {
                lastError = Error::TYPE;
            }
        }
    }
    if (someReaded)
    {
        projectionStatus = someMissing ? Status::PARTIAL : Status::ALL;
    }
    else
    {
        projectionStatus = Status::EMPTY;
    }

    // check that list was not empty and some new projections added
    if (oldSize && oldSize != count)
    {
        auto end = projectionsList->end();
        std::sort(oldStart, end, Projection::less);
    }
    completeProcess();
}

