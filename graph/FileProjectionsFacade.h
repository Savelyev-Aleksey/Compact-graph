#ifndef FILEPROJECTIONSFACADE_H
#define FILEPROJECTIONSFACADE_H



#include "types.h"
#include "ReaderBase.h"
#include "Worker.h"
#include "WriterBase.h"
#include "ProjectionsReader.h"
#include "Projection.h"



class GraphBase;
class FileProjections;
class Projection;


class FileProjectionsFacade :
        virtual public ReaderBase,
        virtual public Worker
{
public:
    typedef const WriterBase::Option WOption;
    typedef ProjectionsReader::Type Type;

    FileProjectionsFacade(GraphBase& graph);
    virtual ~FileProjectionsFacade();

    virtual void clear();

    static bool isCanRead(const char* typeStr);
    static Type getType(const char* typeStr);

    virtual bool readFile(const char* fileName);
    bool readFile(FILE* f, Type typeId);



    bool isProjectionExist(unsigned nodeId) const;
    unsigned projectionsCount() const;

    const Projection* getLoadedProjection() const;
    const Projection* getProjection(unsigned nodeId);
    void createProjection(unsigned nodeId);
    virtual void createAllProjections();


    virtual ProjShortPaths* findShortPaths(unsigned fromId, unsigned toId,
                                           bool reverse = false);


    // Projections writers
    virtual bool saveProjections(const char* fileName,
                                 cuint options = (cuint) WOption::NONE);
    virtual bool saveProjection(const char* fileName, unsigned rootNode,
                                cuint options = (cuint) WOption::NONE);

protected:
    FileProjections* projections;

    void updateGraphFileName(const std::string& newName);
};

#endif // FILEPROJECTIONSFACADE_H
