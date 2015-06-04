#include <iostream>
#include <deque>

#include "ShortPathReader.h"
#include "ShortPathElem.h"
#include "ShortPathRootElem.h"
#include "ShortPath.h"
#include "FileTypes.h"
#include "GraphBase.h"



/**
 * Types[] must be in same order with FileTypes::Type
 */
const char* const ShortPathReader::types[] = {
    "{BRACKETS_SHORT_PATH_VALUE}"
};



ShortPathReader::ShortPathReader(ShortPath& shortPath) : ReaderBase(),
    shortPath(&shortPath)
{ }



bool ShortPathReader::isCanRead(const char* type)
{
    Type t = FileTypes<Type>::typeId(type, types);
    return t == Type::UNDEFINED ? false : true;
}



/**
 * @brief ShortPathReader::readFile trying to open file to read.
 * Checking file type. Returning file pointer for next reading file
 * @param fileName file name to open
 * @return file pointer on success (file accessed to read and have proper type)
 * otherwise nullptr returned
 */
FILE* ShortPathReader::openFile(const char* fileName, Type& typeId)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        lastError = Error::READ;
        std::clog << "[!!!] Error: Can't read file (" << fileName << ")\n";
        return nullptr;
    }

    char typeStr[200];
    fgets(typeStr, 200, f);
    typeId = FileTypes<Type>::typeId(typeStr, types);
    if (typeId == Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!] Warning: File type is unknown " << typeStr
                  << "Try to use an another class implementation to read."
                  << std::endl;
        fclose(f);
        return nullptr;
    }
    lastError = Error::NONE;
    return f;
}



/**
 * @brief ShortPathReader::readShortPath - Read file contain graph
 * nodes short pahts. This method cleans the graph before reading.
 * After read graph will contain onty short paths edges.
 * @param fp - file pointer for read
 * @param options - some optinons (OUT_WARNINGS)
 * @return true if readed successful
 */
bool ShortPathReader::readShortPath(FILE* fp, Type typeId)
{
    if (!fp)
    {
        lastError = Error::READ;
        return false;
    }
    if (typeId != Type::BRACKETS_SHORT_PATH_VALUE)
    {
        lastError = Error::TYPE;
        return false;
    }
    // Cleanup structures before read
    shortPath->clear();
    GraphBase& graph = shortPath->getGraph();

    unsigned nodeFromNum, nodeToNum, startNode;
    float value = 0;
    int count;
    fpos_t position;
    unsigned outWarnings = options & (unsigned) Option::OUT_WARNINGS;
    unsigned char bracket;
    bool readError = false;
    unsigned indent;

    ShortPathElem* search, *parent, *elem;
    std::deque <unsigned> nodesStack;
    std::deque <ShortPathElem*> nodesElemStack;

    while (!feof(fp))
    {
        // If parents not exists so read parent from file
        if (!nodesStack.size())
        {
            count = fscanf(fp, "%u(", &nodeFromNum);
            if (count == 0)
            {
                readError = true;
                break;
            }
            else if (count == -1)
            {
                break;
            }

            startNode = nodeFromNum;
            indent = 1;

            ShortPathRootElem* rootElem = shortPath->initShortPath(nodeFromNum);
            parent = rootElem->getNodes();
            search = rootElem->getSearch();

            nodesStack.push_front(nodeFromNum);
            nodesElemStack.push_front(parent);
        }
        count = fscanf(fp, "%u[%g]", &nodeToNum, &value);

        // If readed add new node.
        if (count == 2)
        {
            // Exclude edge-loops
            if (nodeFromNum == nodeToNum)
            {
                if (outWarnings)
                {
                    std::clog << "[!] Warning: Loop found "
                              << nodeFromNum << ' ' << nodeToNum
                              << ' ' << value << std::endl;
                }
                continue;
            }
            elem = search->findElem(nodeToNum);

            if (elem != nullptr)
            {
                std::clog << "[!] Warning: node " << startNode << "(... "
                          << nodeFromNum << "( " << nodeToNum
                          << " ) already in path. Check path" << std::endl;
                continue;
            }
            // Add node hierarchy
            elem = parent->addNodeElem(nodeToNum, value, indent);
            // add node in search map
            search->addNodeElem(nodeToNum, elem);

            float weight = value - parent->getWeight();
            graph.addEdge(nodeFromNum, nodeToNum, weight);
        }
        else if (count == 0)
        {
            // If can't read node value here is bracket
            // If this is not bracket save pos to back one char
            // for read file from this incorrect symbol
            fgetpos(fp, &position);

            bracket = fgetc(fp);
            if (bracket == '(')
            {
                // So current node become parent
                nodesStack.push_front(nodeToNum);
                nodesElemStack.push_front(elem);
                nodeFromNum = nodeToNum;
                parent = elem;
                ++indent;
            }
            else if (bracket == ')')
            {
                // go up give old parent
                --indent;
                nodesStack.pop_front();
                nodesElemStack.pop_front();
                if (nodesStack.size())
                {
                    nodeFromNum = nodesStack.front();
                    parent = nodesElemStack.front();
                }
            }
            else
            {
                readError = true;
                fsetpos(fp, &position);
                break;
            }
        }
    }
    if (readError)
    {
        if (feof(fp))
        {
            std::clog << "[!!!] Error: Error while reading "
                         "at the end of file" << std::endl;
        }
        else
        {
            char buf[20] = "";
            buf[19] = '\0';
            fread(buf, sizeof(char), 19, fp);
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


