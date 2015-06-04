
#include <cstring>

#include "FileTypes.h"


FileTypes::FileTypes()
{ }



FileTypes::~FileTypes()
{ }



/**
 * @brief FileTypes::typeId - return enum type by string file type
 * @param typeStr - string file type
 * @return file type id from enum of UNDEFINED id if found
 */
template <typename T>
T FileTypes::typeId(const char* typeStr, const char** types)
{
    unsigned end = (unsigned) T::UNDEFINED;
    for (unsigned i = 0; i < end; ++i)
    {
        if (!strncmp(typeStr, types[i], strlen(types[i])))
        {
            return static_cast <T> (i);
        }
    }
    return T::UNDEFINED;
}


/**
 * @brief FileTypes::typeName - return file name string for enum type id
 * @param typeId - searched by enum file id
 * @return file type string pointer or nullptr if not found
 */
template <typename T>
const char* FileTypes::typeName(T typeId, const char** types)
{
    if (typeId >= T::UNDEFINED)
    {
        return nullptr;
    }
    return types[ (unsigned) typeId ];
}

