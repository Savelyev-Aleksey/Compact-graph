#ifndef FILETYPES_H
#define FILETYPES_H


template <typename T>
class FileTypes
{
public:

    FileTypes();
    ~FileTypes();

    static T typeId(const char* typeStr, const char** types);
    static const char* typeName(T typeId, const char** types);
};

#endif // FILETYPES_H
