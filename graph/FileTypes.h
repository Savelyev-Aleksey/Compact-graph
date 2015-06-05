#ifndef FILETYPES_H
#define FILETYPES_H


template< typename Type >
class FileTypes
{
public:

    FileTypes();
    ~FileTypes();

    static Type typeId(const char* typeStr, const char* const types[]);
    static const char* typeName(Type typeId, const char* const types[]);
};

#endif // FILETYPES_H
