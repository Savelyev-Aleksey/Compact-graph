
#include <cstring>

#include "FileTypes.h"


/**
 * Types[] must be in same order with FileTypes::Type
 */
const char* const FileTypes::types[] = {
    "{NODE_NODE}",
    "{NODE_NODE_VALUE}",
    "{BRACKETS_FLAT}",
    "{BRACKETS_FLAT_VALUE}",
    "{BRACKETS}",
    "{BRACKETS_VALUE}",
    "{BRACKETS_SHORT_PATH_VALUE}",
    "{PROJECTIONS}"
};



FileTypes::FileTypes()
{ }



FileTypes::~FileTypes()
{ }



/**
 * @brief FileTypes::typeId - return enum type by string file type
 * @param typeStr - string file type
 * @return file type id from enum of UNDEFINED id if found
 */
FileTypes::Type FileTypes::typeId(const char* typeStr)
{
    unsigned end = (unsigned) Type::UNDEFINED;
    for (unsigned i = 0; i < end; ++i)
    {
        if (!strncmp(typeStr, types[i], strlen(types[i])))
        {
            return (Type) i;
        }
    }
    return Type::UNDEFINED;
}


/**
 * @brief FileTypes::typeName - return file name string for enum type id
 * @param typeId - searched by enum file id
 * @return file type string pointer or nullptr if not found
 */
const char* FileTypes::typeName(Type typeId)
{
    if (typeId >= Type::UNDEFINED)
    {
        return nullptr;
    }
    return types[ (unsigned) typeId ];
}

