
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstring>

#include <deque>

#include "GraphBase.h"
#include "GraphReader.h"



GraphReader::GraphReader(GraphBase& graph) : ReaderBase(),
    graph(&graph)
{ }



GraphReader::~GraphReader()
{ }



/**
 * @brief GraphReader::readFile - read file contain nodes. If file
 * not recognised error will logged.
 * This function used if class used standalone. It can read not all file types.
 * For other types used another classes.
 * For all types use class wrapper with overloaded readFile function.
 * @param fileName - string contain file name
 * @return true if read successful
 */
bool GraphReader::readFile(const char* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        lastError = Error::READ;
        std::clog << "[!!!] Critical: Can't read file (" << fileName << ")\n";
        return false;
    }

    char typeStr[200];
    fgets(typeStr, 200, f);
    FileTypes::Type typeId = FileTypes::typeId(typeStr);
    if (typeId == FileTypes::Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!!!] Critical: File type is unknown " << typeStr
                  << "in file (" << fileName << ")" << std::endl;
        fclose(f);
        return false;
    }
    graph->scanInfo(f);
    bool result = readFile(f, typeId);
    fclose(f);

    if (result)
    {
        lastError = Error::NONE;
        std::clog << "File (" << fileName << ") readed" << std::endl;
    }
    else
    {
        std::clog << "Error in file (" << fileName << ")" << std::endl;
    }

    graph->logStatus();

    return result;
}



/**
 * @brief GraphReader::readFile - read file contain nodes.
 * @param f - FILE handle
 * @param typeId - file type id
 * @return true if file readed successful
 */
bool GraphReader::readFile(FILE *f, FileTypes::Type typeId)
{
    bool result;

    bool outWarning = options & (unsigned) Option::OUT_WARNINGS;

    switch (typeId)
    {
    case FileTypes::Type::NODE_NODE:
        result = readEdges(f, false);
        break;
    case FileTypes::Type::NODE_NODE_VALUE:
        result = readEdges(f, true);
        break;
    case FileTypes::Type::BRACKETS:
        // no break
    case FileTypes::Type::BRACKETS_FLAT:
        result = readBrackets(f, false);
        break;
    case FileTypes::Type::BRACKETS_VALUE:
        // no break
    case FileTypes::Type::BRACKETS_FLAT_VALUE:
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
    unsigned nodeFromNum, nodeToNum, readLine = 0;
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
            char buf[20];
            buf[19] = '\0';
            fread(buf, sizeof(char), 19, f);
            std::clog << "[!!!] Critical: Error while reading from file on: "
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
            std::clog << "[!!!] Critical: Error while reading "
                         "at the end of file" << std::endl;
        }
        else
        {
            char buf[20] = "";
            buf[19] = '\0';
            fread(buf, sizeof(char), 19, f);
            std::clog << "[!!!] Critical: Error while reading from file near: "
                      << buf << std::endl;
        }
        lastError = Error::SYNTAX;
        return false;
    }
    if (nodesStack.size())
    {
        std::clog << "[!!!] Critical: Error while reading at the end of file. "
                     "File not full, nesting are wrong." << std::endl;
        lastError = Error::SYNTAX;
        return false;
    }
    return true;
}


