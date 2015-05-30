#include <iostream>
#include <deque>

#include "ShortPathReader.h"
#include "ShortPathElem.h"
#include "ShortPathRootElem.h"
#include "ShortPath.h"
#include "FileTypes.h"
#include "GraphBase.h"



ShortPathReader::ShortPathReader(ShortPath& shortPath) : ReaderBase(),
    shortPath(&shortPath)
{ }



/**
 * @brief ShortPathReader::readFile - read file contain short paths nodes.
 * If file not recognised error will logged.
 * This function used if class used standalone.
 * It can read not all file types. For other types used another classes.
 * For all types use class wrapper with overloaded readFile function.
 * @param fileName - string contain file name
 * @return true if read successful
 */
bool ShortPathReader::readFile(const char* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
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
                  << std::endl;
        fclose(f);
        return false;
    }

    bool outWarnings = options & (unsigned) Option::OUT_WARNINGS;
    bool result;

    if (typeId == FileTypes::Type::BRACKETS_SHORT_PATH_VALUE)
    {
        result = readShortPath(f, typeId);
    }
    else if (outWarnings)
    {
        lastError = Error::TYPE;
        std::clog << "[!] Warning: ShortPathReader can't read this file type, "
                  << "use other class implementations" << std::endl;
    }

    fclose(f);

    if (result)
    {
        lastError = Error::NONE;
        std::clog << "File (" << fileName << ") readed" << std::endl;
    }

    return result;
}



/**
 * @brief ShortPathReader::readShortPath - Read file contain graph
 * nodes short pahts.
 * @param fp - file pointer for read
 * @param options - some optinons (OUT_WARNINGS)
 * @return true if readed successful
 */
bool ShortPathReader::readShortPath(FILE* fp, FileTypes::Type typeId)
{
    if (!fp)
    {
        lastError = Error::READ;
        return false;
    }
    if (typeId != FileTypes::Type::BRACKETS_SHORT_PATH_VALUE)
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
            std::clog << "[!!!] Critical: Error while reading "
                         "at the end of file" << std::endl;
        }
        else
        {
            char buf[20] = "";
            buf[19] = '\0';
            fread(buf, sizeof(char), 19, fp);
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


