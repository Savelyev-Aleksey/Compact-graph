
#include <cstring>

#include "FileTypes.h"

template< typename Type >
FileTypes<Type>::FileTypes()
{ }


template< typename Type >
FileTypes<Type>::~FileTypes()
{ }



/**
 * @brief FileTypes::typeId - return enum type by string file type
 * @param typeStr - string file type
 * @return file type id from enum of UNDEFINED id if found
 */
template< typename Type >
Type FileTypes<Type>::typeId(const char* typeStr, const char* const types[])
{
    unsigned end = (unsigned) Type::UNDEFINED;
    for (unsigned i = 0; i < end; ++i)
    {
        if (!strncmp(typeStr, types[i], strlen(types[i])))
        {
            return static_cast <Type> (i);
        }
    }
    return Type::UNDEFINED;
}


/**
 * @brief FileTypes::typeName - return file name string for enum type id
 * @param typeId - searched by enum file id
 * @return file type string pointer or nullptr if not found
 */
template< typename Type >
const char* FileTypes<Type>::typeName(Type typeId, const char* const types[])
{
    if (typeId >= Type::UNDEFINED)
    {
        return nullptr;
    }
    return types[ (unsigned) typeId ];
}

