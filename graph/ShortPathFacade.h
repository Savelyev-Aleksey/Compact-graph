#ifndef SHORTPATHFACADE_H
#define SHORTPATHFACADE_H


#include "types.h"
#include "ReaderBase.h"
#include "Worker.h"
#include "ShortPath.h"
#include "ShortPathReader.h"
#include "ShortPathWriter.h"


class ShortPath;
class GraphBase;



class ShortPathFacade :
        virtual public ReaderBase,
        virtual public Worker
{
public:
    ShortPathFacade(GraphBase& graph);
    virtual ~ShortPathFacade();

    typedef ShortPathWriter::Option WOption;

    typedef ShortPathReader::Type Type;

    virtual void clear();

    static bool isCanRead(const char* typeStr);
    static Type getType(const char* typeStr);
    virtual bool readFile(const char* fileName);
    bool readFile(FILE* f, Type typeId);

    bool isShortPathEmpty() const;
    bool isPathExist(unsigned nodeId) const;
    unsigned shortPathsCount() const;

    virtual void createAllShortPaths(float pathLimit = 0);

    // ShortPath writers
    bool saveShortPaths(const char* fileName, const NodeIdDeque* nodes =nullptr,
         float pathLimit = 0, cuint options = (cuint) WOption::NONE);

    bool writeExistShortPaths(const char* fileName, const NodeIdDeque* nodes,
                              cuint options = (cuint) WOption::NONE);

protected:
    ShortPath* shortPath;
};

#endif // SHORTPATHFACADE_H
