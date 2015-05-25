#ifndef FILETYPES_H
#define FILETYPES_H


class FileTypes
{
public:
    enum Type
    {
        // Type id must be in same order with types[]
        NODE_NODE,
        NODE_NODE_VALUE,
        BRACKETS_FLAT,
        BRACKETS_FLAT_VALUE,
        BRACKETS,
        BRACKETS_VALUE,
        BRACKETS_SHORT_PATH_VALUE,
        PROJECTIONS,
        // Always must be last - used in loop
        UNDEFINED
    };

    FileTypes();
    ~FileTypes();

    static Type typeId(const char* typeStr);
    static const char* typeName(Type typeId);
private:
    static const char* const types[];
};

#endif // FILETYPES_H
