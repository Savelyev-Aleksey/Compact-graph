
#include "ReaderBase.h"



ReaderBase::ReaderBase() :
    options(Option::OUT_WARNINGS),
    lastError(Error::NONE)
{ }



/**
 * @brief ReaderBase::setOptions - set options for reading
 * @param options - set options
 */
void ReaderBase::setOptions(unsigned options)
{
    this->options = options;
}



ReaderBase::Error ReaderBase::getLastError() const
{
    return lastError;
}

