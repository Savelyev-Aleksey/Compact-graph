#ifndef PROJECTIONSREADER_H
#define PROJECTIONSREADER_H


#include <cstdio>
#include <string>

#include "FileTypes.h"
#include "ReaderBase.h"

class GraphBase;
class Projection;
class FileProjections;


class ProjectionsReader : public ReaderBase
{
public:
    ProjectionsReader(FileProjections& projections);

    static const char* const types[];

    enum class Type : unsigned
    {
        // Type id must be in same order with types[]
        PROJECTIONS,
        // Always must be last - used in loop
        UNDEFINED
    };

    static bool isCanRead(const char* typeStr);
    static Type getType(const char *type);

    FILE* openFile(const char* fileName, Type& typeId);
    bool readProjections(FILE* fp, Type typeId);
    bool readProjectionsInfo(const std::string& fileName);

protected:
    FileProjections* projections;

    unsigned eccesntricity;
    unsigned shortestLoop;

    bool readProjectionInfo(const char* fileName, Projection* pr);

    void readInfo(FILE* f);
    void setInfo(const char* str);
    void setInfo(const std::string& prop, std::string& value);
};

#endif // PROJECTIONSREADER_H
