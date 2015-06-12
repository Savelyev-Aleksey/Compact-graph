
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstring>

#include <deque>

#include "GraphBase.h"
#include "GraphReader.h"
#include "FileTypes.h"


/**
 * Types[] must be in same order with FileTypes::Type
 */
const char* const GraphReader::types[] = {
    "{NODE_NODE}",
    "{NODE_NODE_VALUE}",
    "{BRACKETS_FLAT}",
    "{BRACKETS_FLAT_VALUE}",
    "{BRACKETS}",
    "{BRACKETS_VALUE}"
};



GraphReader::GraphReader(GraphBase& graph) : ReaderBase(),
    graph(&graph)
{ }



GraphReader::~GraphReader()
{ }



bool GraphReader::isCanRead(const char* type)
{
    Type t = FileTypes::typeId<Type>(type, types);
    return t == Type::UNDEFINED ? false : true;
}



GraphReader::Type GraphReader::getType(const char *type)
{
    return FileTypes::typeId<Type>(type, types);
}



/**
 * @brief GraphReader::readFile trying to open file to read.
 * Checking file type. Returning file pointer for next reading file
 * @param fileName - file name to open
 * @param typeId - will return type if of file
 * @return file pointer on success (file accessed to read and have proper type)
 * otherwise nullptr returned
 */
FILE* GraphReader::openFile(const char* fileName, Type &typeId)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        lastError = Error::READ;
        std::clog << "[!!!] Error: Can't read file (" << fileName << ")\n";
        return nullptr;
    }

    char typeStr[200u];
    fgets(typeStr, 200u, f);
    typeId = FileTypes::typeId<Type>(typeStr, types);
    if (typeId == Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!] Warning: File type is unknown " << typeStr
                  << "Try to use an another class for read."
                  << std::endl;
        fclose(f);
        return nullptr;
    }
    lastError = Error::NONE;
    return f;
}



/**
 * @brief GraphReader::readFile - read file contain nodes.
 * @param f - FILE handle
 * @param typeId - file type id
 * @return true if file readed successful
 */
bool GraphReader::readFile(FILE *f, Type typeId)
{
    bool result;

    bool outWarning = options & (unsigned) Option::OUT_WARNINGS;

    switch (typeId)
    {
    case Type::NODE_NODE:
        result = readEdges(f, false);
        break;
    case Type::NODE_NODE_VALUE:
        result = readEdges(f, true);
        break;
    case Type::BRACKETS: // no break
    case Type::BRACKETS_FLAT:
        result = readBrackets(f, false);
        break;
    case Type::BRACKETS_VALUE: // no break
    case Type::BRACKETS_FLAT_VALUE:
        result = readBrackets(f, true);
        break;

    default:
        if (outWarning)
        {
            std::clog << "[!] Warning: GraphReader can't read this file type, "
                      << "use other class implementations" << std::endl;
        }
        lastError = Error::TYPE;
        result = false;
        break;
    }
    return result;
}



/**
 * @brief GraphReader::readNodeNodeValue read file format by
 * Node Node Value
 * @param f - reading file's pointer
 * @param readValue - read value if file contain value (n n v) else (n n)
 * @return true if read successful
 */
bool GraphReader::readEdges(FILE* f, bool readValue)
{
    unsigned nodeFromNum, nodeToNum, readLine = 0u;
    float value;
    int count;
    bool isEdgeAdded;
    unsigned outWarning = options & (unsigned) Option::OUT_WARNINGS;

    float defaultWeight = 0;

    if (!readValue)
    {
        const char* weightStr = graph->getInfo("WEIGHT");
        if (!weightStr || sscanf(weightStr, "%f", &defaultWeight) != 1)
        {
            defaultWeight = 0;
        }
    }

    while (!feof(f))
    {
        ++readLine;
        if (readValue)
        {
            count = fscanf(f, "%u %u %f\n", &nodeFromNum, &nodeToNum, &value);
        }
        else
        {
            count = fscanf(f, "%u %u\n", &nodeFromNum, &nodeToNum);
            value = defaultWeight;
        }

        if (count == 0)
        {
            char buf[20u];
            buf[19u] = '\0';
            fread(buf, sizeof(char), 19u, f);
            std::clog << "[!!!] Error while reading from file on: "
                      << buf << std::endl;
            lastError = Error::SYNTAX;
            return false;
        }
        else if (count == -1)
        {
            continue;
        }

        // Exclude edge-loops
        if (nodeFromNum == nodeToNum)
        {
            if (outWarning)
            {
                std::clog << "[!] Warning: Loop find on line(" << readLine
                          << ") " << nodeFromNum << ' ' << nodeToNum
                          << ' ' << value << std::endl;
            }
            continue;
        }
        isEdgeAdded = graph->addEdge(nodeFromNum, nodeToNum, value);

        if (!isEdgeAdded && outWarning)
        {
            std::clog << "[!] Warning: The edge dublicate line(" << readLine
                      << ") " << nodeFromNum << ' ' << nodeToNum
                      << ' ' << value << std::endl;

        }
    }
    return true;
}



/**
 * @brief GraphReader::readBrackets - read file format by
 * Node(Node[V](Node[V]...), Node[V]...) - flat style, nodes can be nested
 * @param f - reading file's pointer
 * @param readValue - true if file contain values Node[V] else Node
 * @return true if read successful
 */
bool GraphReader::readBrackets(FILE* f, bool readValue)
{
    float defaultWeight = 0;

    if (!readValue)
    {
        const char* weightStr = graph->getInfo("WEIGHT");
        if (!weightStr || sscanf(weightStr, "%f", &defaultWeight) != 1)
        {
            defaultWeight = 0;
        }
    }

    unsigned nodeFromNum, nodeToNum;
    float value = 0;
    int count;
    fpos_t position;
    unsigned outWarning = options & (unsigned) Option::OUT_WARNINGS;
    unsigned char bracket;
    bool readError = false;

    std::deque <unsigned> nodesStack;

    while (!feof(f))
    {
        // If parents not exists so read parent from file
        if (!nodesStack.size())
        {
            count = fscanf(f, "%u(", &nodeFromNum);
            if (count == -1)
            {
                continue;
            }
            else if (count == 0)
            {
                readError = true;
                break;
            }
            nodesStack.push_front(nodeFromNum);
        }
        if (readValue)
        {
            count = fscanf(f, "%u[%g]", &nodeToNum, &value);
        }
        else
        {
            count = fscanf(f, "%u", &nodeToNum);
            value = defaultWeight;
        }

        // If readed add new node.
        if (count > 0)
        {
            // Exclude edge-loops
            if (nodeFromNum == nodeToNum)
            {
                if (outWarning)
                {
                    std::clog << "[!] Warning: Loop found "
                              << nodeFromNum << ' ' << nodeToNum
                              << ' ' << value << std::endl;
                }
                continue;
            }
            graph->addEdge(nodeFromNum, nodeToNum, value);
        }
        else if (count == 0)
        {
            // If can't read node value here is bracket
            fgetpos(f, &position);

            bracket = fgetc(f);
            if (bracket == '(')
            {
                // So current node become parent
                nodesStack.push_front(nodeToNum);
                nodeFromNum = nodeToNum;
            }
            else if (bracket == ')')
            {
                // go up give old parent
                nodesStack.pop_front();
                if (nodesStack.size())
                {
                    nodeFromNum = nodesStack.front();
                }
            }
            else
            {
                readError = true;
                fsetpos(f, &position);
                break;
            }
        }
    }
    if (readError)
    {
        if (feof(f))
        {
            std::clog << "[!!!] Error while reading "
                         "at the end of file" << std::endl;
        }
        else
        {
            char buf[20u] = "";
            buf[19u] = '\0';
            fread(buf, sizeof(char), 19u, f);
            std::clog << "[!!!] Error while reading from file near: "
                      << buf << std::endl;
        }
        lastError = Error::SYNTAX;
        return false;
    }
    if (nodesStack.size())
    {
        std::clog << "[!!!] Error while reading at the end of file. "
                     "File not full, nesting are wrong." << std::endl;
        lastError = Error::SYNTAX;
        return false;
    }
    return true;
}


