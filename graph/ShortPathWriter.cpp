
#include "ShortPathWriter.h"
#include "FileTypes.h"
#include "GraphBase.h"
#include "ShortPathElem.h"
#include "ShortPathRootElem.h"



ShortPathWriter::ShortPathWriter(ShortPath& shortPath) :
    WriterBase(),
    shortPath(&shortPath)
{ }



/**
 * @brief ShortPathWriter::savePaths - Write in file paths. Create if not exist.
 * @param fileName - file name for save paths
 * @param nodes - deque of nodes id for generate and save path into file
 * @param pathLimit - path weight limit
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if save successful
 */
bool ShortPathWriter::savePaths(const char* fileName, const NodeIdDeque* nodes,
                                float pathLimit, unsigned options)
{
    if (!nodes)
    {
        GraphBase* graph = &shortPath->getGraph();
        NodeMap* map = graph->getNodeMap();
        NodeIdDeque list;
        for (auto it = map->begin(); it != map->end(); ++it)
        {
            list.push_back(it->first);
            shortPath->generateShortPath(it->first, pathLimit);
        }
        return writeExistPaths(fileName, &list, options);
    }
    else
    {
        for (size_t i = 0; i < nodes->size(); ++i)
        {
            shortPath->generateShortPath(nodes->at(i), pathLimit);
        }
        return writeExistPaths(fileName, nodes, options);
    }
}



/**
 * @brief ShortPathWriter::writeExistPaths - Save in file already generated
 * short paths for nodes from node list.
 * @param fileName - file name where save paths
 * @param nodes - deque list of node for save path
 * @param options - set some options for function (PRINT_INDENTS, PRINT_INFO)
 * @return true if write successful
 */
bool ShortPathWriter::writeExistPaths(const char* fileName,
                                     const NodeIdDeque* nodes, unsigned options)
{
    if (!nodes)
    {
        return false;
    }
    RootPathList *shortPathNodes = shortPath->getShortPathNodes();
    if (!shortPathNodes->size())
    {
        return false;
    }

    FILE* f = fopen(fileName, "w");
    if (f == nullptr)
    {
        return false;
    }

    FileTypes::Type typeId = FileTypes::BRACKETS_SHORT_PATH_VALUE;

    const char* fileType = FileTypes::typeName(typeId);
    fputs(fileType, f);
    fputc('\n', f);

    for (auto it = nodes->begin(); it != nodes->end(); ++it)
    {
        auto parentIt = shortPathNodes->find(*it);
        if (parentIt == shortPathNodes->end())
        {
            continue;
        }
        writeShortPath(f, parentIt, options);
    }
    fclose(f);
    return true;
}



/**
 * @brief ShortPathWriter::writeShortPath - Save exist short path in open file
 * @param fp - open file pointer
 * @param pathIt - Iterator on exist short path
 * @param options - some options (PRINT_INDENTS)
 */
void ShortPathWriter::writeShortPath(FILE* fp, RootPathList::iterator pathIt,
                                     unsigned options)
{
    size_t parentId = pathIt->first;
    ShortPathElem* pathElem = pathIt->second->getNodes();
    PathList* pathList = pathElem->getPathList();
    if (pathList == nullptr)
    {
        return;
    }

    size_t nodeId;
    float weight;
    bool isFirst = true;
    std::deque <PathListItPair> pathStack;
    size_t currentIndent = 1;

    bool printIndents = options & Option::PRINT_INDENTS;


    pathStack.push_front( PathListItPair(pathList->begin(), pathList->end()) );

    fprintf(fp, "%zu(", parentId);
    if (printIndents)
    {
        fputc('\n', fp);
    }
    while (pathStack.size())
    {
        auto pathIt  = pathStack.front().first;
        auto pathEnd = pathStack.front().second;
        if (pathIt == pathEnd)
        {
            pathStack.pop_front();
            --currentIndent;
            if (printIndents)
            {
                for(unsigned i = 0; i < currentIndent; ++i)
                {
                    fputc('\t', fp);
                }
                fputs(")\n", fp);
            }
            else
            {
                fputc(')', fp);
            }
            continue;
        }
        nodeId = pathIt->first;
        pathElem = pathIt->second;
        pathList = pathElem->getPathList();

        if (printIndents)
        {
            for(unsigned i = 0; i < currentIndent; ++i)
            {
                fputc('\t', fp);
            }
        }
        else if (!isFirst)
        {
            fputc(' ', fp);
        }
        else {
            isFirst = false;
        }
        weight = pathElem->getWeight();

        fprintf(fp, "%zu[%g]", nodeId, weight);

        ++(pathStack.front().first);

        if (pathList && pathList->size())
        {
            fputc('(', fp);
            // Move to next elem

            // go to next child
            pathStack.push_front( PathListItPair(pathList->begin(),
                                                 pathList->end()) );
            isFirst = true;
            ++currentIndent;
        }
        if (printIndents)
        {
            fputc('\n', fp);
        }
    }
}



