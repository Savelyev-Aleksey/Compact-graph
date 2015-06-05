#ifndef FILEPROJECTIONS_H
#define FILEPROJECTIONS_H


#include <string>


#include "ReaderBase.h"
#include "Projections.h"


class GraphBase;


class FileProjections : public Projections, public ReaderBase
{
public:
    FileProjections(GraphBase& graph);
    virtual ~FileProjections();

    friend class ProjectionsReader;

    enum class Status : unsigned
    {
        NONE = 0u,// projection info not readed
        EMPTY,    // projection files not exist at all
        PARTIAL,  // projection files exists, but not for all nodes
        ALL       // all projection files exists
    };

    Status getStatus() const;
    bool isMemoryUsed() const;

    const std::string& getFileName() const;

    bool isProjectionExist(unsigned nodeId) const;
    const Projection* getLoadedProjection() const;
    virtual const Projection* getProjection(unsigned nodeId);
    virtual const Projection* createProjection(unsigned nodeId);
    virtual void createAllProjections();

    virtual ProjShortPaths* findShortPaths(unsigned fromId, unsigned toId,
                                           bool reverse = false);

    void updateGraphFileName(const std::string& newName);

protected:
    std::string graphFileName;
    Projection* loadedProjection;
    Status projectionStatus;
    bool useMemory;


    bool loadProjection(Projection* pr);
};

#endif // FILEPROJECTIONS_H
