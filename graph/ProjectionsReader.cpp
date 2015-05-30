#include <cstddef>
#include <iostream>



#include "ProjectionsReader.h"

#include "ProjectionElem.h"
#include "Projection.h"
#include "Projections.h"
#include "GraphBase.h"



ProjectionsReader::ProjectionsReader(Projections& projections) :
    ReaderBase(),
    projections(&projections)
{}



/**
 * @brief ProjectionsReader::readFile - read file contain nodes projections.
 * If file not recognised error will logged.
 * This function used if class used standalone.
 * It can read not all file types. For other types used another classes.
 * For all types use class wrapper with overloaded readFile function.
 * @param fileName - string contain file name
 * @return true if read successful
 */
bool ProjectionsReader::readFile(const char *fileName)
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

    if (typeId == FileTypes::Type::PROJECTIONS)
    {
        result = readProjections(f, typeId);
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
 * @brief ProjectionsReader::readProjections - Read file contain graph nodes
 * projections.
 * @param fp - file pointer for read
 * @param options - some optinons (OUT_WARNINGS)
 * @return true if readed successful
 */
bool ProjectionsReader::readProjections(FILE *fp, FileTypes::Type typeId)
{
    if (!fp)
    {
        lastError = Error::READ;
        return false;
    }
    if (typeId != FileTypes::Type::PROJECTIONS)
    {
        lastError = Error::TYPE;
        return false;
    }
    // Cleanup structures before read
    projections->clear();

    GraphBase& graph = projections->getGraph();

    float weight = 0;
    const char* weightStr = graph.getInfo("WEIGHT");
    if (!weightStr || sscanf(weightStr, "%f", &weight) != 1)
    {
        weight = 0;
    }

    unsigned nodeFromNum, nodeToNum;

    int count;
    fpos_t position;
    unsigned outWarnings = options & (unsigned) Option::OUT_WARNINGS;
    unsigned char bracket;
    bool readError = false;
    unsigned indent;

    Projection* projection = nullptr;
    ProjectionElem *parent, *elem;
    ProjectionLevelList* levelList;
    ProjectionLevelElem* level;
    std::deque <unsigned> nodesStack;
    std::deque <ProjectionElem*> nodesElemStack;

    ProjectionsList* projectionsList = projections->projectionsList;

//    ProjectionElemMap origianlNodes;

    while (!feof(fp))
    {
        // If parents not exists so read parent from file
        if (!nodesStack.size())
        {
            count = fscanf(fp, "%zu(", &nodeFromNum);
            if (!count)
            {
                readError = true;
                break;
            }
            indent = 1;

            // if on last loop will be readed
            if (projection)
            {
                projection->updateAfterRead();
            }

            parent = new ProjectionElem(nodeFromNum);
            levelList = new ProjectionLevelList;
            projection = new Projection(nodeFromNum);
            //projection->setProjection(parent, levelList);
            projection->rootNode = parent;
            projection->levelList = levelList;
            projectionsList->insert({nodeFromNum, projection});

            // This is root level with respectived node
            level = new ProjectionLevelElem;
            level->insert({nodeFromNum, parent});
            levelList->push_back(level);
            // This is current level for add child elements
            level = new ProjectionLevelElem;
            levelList->push_back(level);

            nodesStack.push_front(nodeFromNum);
            nodesElemStack.push_front(parent);
        }
        count = fscanf(fp, "%zu", &nodeToNum);

        // If readed add new node.
        if (count == 1)
        {
            // Exclude edge-loops
            if (nodeFromNum == nodeToNum)
            {
                if (outWarnings)
                {
                    std::clog << "[!] Warning: Loop found "
                              << nodeFromNum << ' ' << nodeToNum
                              << ' ' << weight << std::endl;
                }
                continue;
            }

            // Add node
            elem = parent->addElem(nodeToNum);
            // Add node in current projection level
            levelList->at(indent)->insert({nodeToNum, elem});

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

                if (indent > levelList->size() - 1)
                    levelList->push_back(new ProjectionLevelElem);
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
        char buf[20] = "";
        buf[19] = '\0';
        fread(buf, sizeof(char), 19, fp);
        std::clog << "[!!!] Critical: Error while reading from file near: "
                  << buf << std::endl;
        lastError = Error::SYNTAX;
        return false;
    }

    // for last loop to be shure that data updated
    if (projection && !projection->getEccentricity())
    {
        projection->updateAfterRead();
    }
    return true;
}

