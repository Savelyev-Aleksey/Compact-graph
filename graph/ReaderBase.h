#ifndef READERBASE_H
#define READERBASE_H


class ReaderBase
{
public:
    enum class Option : unsigned
    {
        NONE         = 0x00,
        OUT_WARNINGS = 0x01
    };
    enum class Error : unsigned
    {
        NONE = 0,
        TYPE,
        READ,
        SYNTAX,
    };


    ReaderBase();
    void setOptions(unsigned options);
    Error getLastError() const;

protected:
    unsigned options;
    Error lastError;
};

#endif // READERBASE_H
