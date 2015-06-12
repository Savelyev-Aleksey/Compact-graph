#include <cstddef>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>


#include "ProjectionsReader.h"

#include "ProjectionElem.h"
#include "Projection.h"
#include "FileProjections.h"
#include "GraphBase.h"



/**
 * Types[] must be in same order with FileTypes::Type
 */
const char* const ProjectionsReader::types[] = {
    "{PROJECTIONS}"
};



ProjectionsReader::ProjectionsReader(FileProjections& projections) :
    ReaderBase(),
    Worker(),
    projections(&projections),
    eccesntricity(0u),
    shortestLoop(0u)
{}



ProjectionsReader::Type ProjectionsReader::getType(const char *type)
{
    return FileTypes::typeId<Type>(type, types);
}



bool ProjectionsReader::isCanRead(const char* type)
{
    Type t = FileTypes::typeId<Type>(type, types);
    return t == Type::UNDEFINED ? false : true;
}



/**
 * @brief ProjectionsReader::projectionFileName function to get file name
 * for saved projection.
 * @param prName - current file name
 * @param length - file name prefix length (without extension and suffix)
 * @param id - projection id
 */
void ProjectionsReader::projectionFileName(std::string &prName, unsigned length,
                                           unsigned id)
{
    prName.resize(length);
    prName += "_pr_";
    prName += std::to_string(id);
    prName += ".txt";
}



bool ProjectionsReader::readProjectionInfo(const char *fileName, Projection* pr)
{
    FILE* f = fopen(fileName, "r");
    // If file not exist skip error to log.
    // Maby graph is new and projections not created yet.
    if (f == nullptr)
    {
        pr->isFileExist = false;
        lastError = Error::READ;
        return false;
    }

    char typeStr[200u];
    fgets(typeStr, 200u, f);

    Type typeId = FileTypes::typeId<Type>(typeStr, types);

    if (typeId == Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!!!] Error: File type is unknown " << typeStr
                  << std::endl;
        fclose(f);
        return false;
    }
    readInfo(f);
    fclose(f);

    pr->eccesntricity = eccesntricity;
    pr->shortestLoop = shortestLoop;
    pr->isFileExist = true;

    return true;
}



/**
 * @brief ProjectionsReader::readFile - read file contain nodes projections.
 * If file not recognised error will logged.
 * This function used if class used standalone.
 * It can read not all file types. For other types used another classes.
 * For all types use class wrapper with overloaded readFile function.
 * @param fileName - string contain file name
 * @return true if read successful
 */
FILE* ProjectionsReader::openFile(const char *fileName, Type& typeId)
{
    FILE* f = fopen(fileName, "r");
    if (f == nullptr)
    {
        std::clog << "[!!!] Error: Can't read file (" << fileName << ")\n";
        lastError = Error::READ;
        return nullptr;
    }

    char typeStr[200u];
    fgets(typeStr, 200u, f);

    typeId = FileTypes::typeId<Type>(typeStr, types);

    if (typeId == Type::UNDEFINED)
    {
        lastError = Error::TYPE;
        std::clog << "[!] Warning: File type is unknown " << typeStr << ". "
                  << "Use other class implementations." << std::endl;
        fclose(f);
        return nullptr;
    }
    return f;
}



/**
 * @brief ProjectionsReader::readProjections - Read file contain graph nodes
 * projections.
 * @param fp - file pointer for read
 * @param options - some optinons (OUT_WARNINGS)
 * @return true if readed successful
 */
bool ProjectionsReader::readProjections(FILE *fp, Type typeId)
{
    if (!fp)
    {
        lastError = Error::READ;
        return false;
    }
    if (typeId != Type::PROJECTIONS)
    {
        lastError = Error::TYPE;
        return false;
    }

    readInfo(fp);

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
    bool isPrWasEmpty = !projectionsList->size();

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
                continue;
            indent = 1;

            // if on last loop projection was readed
            if (projection)
            {
                projection->updateAfterRead();
            }

            if (!isPrWasEmpty)
            {
                projection=projections->Projections::getProjection(nodeFromNum);
                if (projection)
                {
                    // Projection not empty
                    if (projection->levelCount())
                        continue;
                }
            }

            if (isPrWasEmpty || !projection)
            {
                projection = new Projection(nodeFromNum);
                projectionsList->push_back(projection);
            }
            // This is root level with respectived node
            parent = new ProjectionElem(nodeFromNum);
            parent->setLevel(0u);
            levelList = new ProjectionLevelList;
            projection->rootNode = parent;
            projection->levelList = levelList;
            // add zero level or respectived node
            level = new ProjectionLevelElem;
            level->push_back(parent);
            levelList->push_back(level);

            // This is current level for add child elements
            level = new ProjectionLevelElem;
            levelList->push_back(level);

            nodesStack.push_front(nodeFromNum);
            nodesElemStack.push_front(parent);
        }
        count = fscanf(fp, "%u", &nodeToNum);

        if (isInterrupted())
        {
            projection->clear();
            return false;
        }

        // If readed add new node.
        if (count == 1)
        {
            // Exclude edge-loops
            if (nodeFromNum == nodeToNum)
            {
                if (outWarnings)
                {
                    std::clog << "[!] Warning: Loop found "
                              << nodeFromNum << ' ' << nodeToNum << std::endl;
                }
                continue;
            }

            // Add node
            elem = parent->addElem(nodeToNum);
            elem->setLevel(indent);
            // Add node in current projection level
            levelList->at(indent)->push_back(elem);
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
        char buf[20u] = "";
        buf[19u] = '\0';
        fread(buf, sizeof(char), 19u, fp);
        std::clog << "[!!!] Error while reading from file near: "
                  << buf << std::endl;
        lastError = Error::SYNTAX;
        return false;
    }

    projection->updateAfterRead();

    return true;
}



/**
 * @brief ProjectionsReader::scanInfo - read parameters from projection.
 * This parameters was calculated then projection was created.
 * Which parameters loaded - look at addInfo function
 * @param f - open projection file
 */
void ProjectionsReader::readInfo(FILE *f)
{
    char buf[200u], bracket;
    fpos_t position;
    while (true)
    {
        fgetpos(f, &position);
        bracket = fgetc(f);
        if (bracket != '{')
        {
            fsetpos(f, &position);
            return;
        }
        fgets(buf, 200u, f);
        setInfo(buf);
    }
}



/**
 * @brief ProjectionsReader::addInfo - perfom readed parameters to trimmed
 * upper case strings and add info in projection
 * @param str
 */
void ProjectionsReader::setInfo(const char *str)
{
    if (!str)
    {
        return;
    }
    const char* pos = strchr(str, '=');
    if (pos == nullptr)
    {
        return;
    }

    std::string name(str, pos - str);
    std::string value(pos + 1);

    // upper case
    std::transform(name.begin(), name.end(), name.begin(),
                   (int (*)(int))std::toupper);
    // trim
    name.erase(0u, name.find_first_not_of(" \t") );
    name.erase(name.find_last_not_of(" \t") +1, name.size() );

    // upper case
    std::transform(value.begin(), value.end(), value.begin(),
                   (int (*)(int))std::toupper);
    // trim
    value.erase(0u, value.find_first_not_of(" \r\n\t") );
    value.erase(value.find_last_not_of(" \r\n\t}") + 1, value.size() );

    setInfo(name, value);
}



void ProjectionsReader::setInfo(const std::string& prop, std::string& value)
{
    if (prop.compare("ECCENTRICITY") == 0)
    {
        eccesntricity = static_cast <unsigned> ( std::stoul(value) );
    }
    else if (prop.compare("SHORTEST_LOOP") == 0)
    {
        shortestLoop = static_cast <unsigned> ( std::stoul(value) );
    }
}

