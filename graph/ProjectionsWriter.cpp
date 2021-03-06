#include <cstdio>


#include "ProjectionsWriter.h"
#include "FileProjections.h"
#include "Projections.h"
#include "Projection.h"
#include "ProjectionElem.h"
#include "FileTypes.h"
#include "GraphBase.h"
#include "ProjectionsReader.h"



ProjectionsWriter::ProjectionsWriter(FileProjections& projections) :
    WriterBase(),
    projections(&projections)
{}



ProjectionsWriter::~ProjectionsWriter()
{}



/**
 * @brief ProjectionsWriter::saveProjections - save all created projections
 * in fileName file.
 * @param fileName - file to write projections
 * @param options - some options (PRINT_INDENTS PRINT_INFO)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool ProjectionsWriter::saveProjections(const char *fileName, cuint options)
{
    const ProjectionsList* list = projections->getList();
    if (!list->size())
    {
        return false;
    }

    FILE* f = fopen(fileName, "w");
    if (f == nullptr)
    {
        return false;
    }

    ProjectionsReader::Type typeId = ProjectionsReader::Type::PROJECTIONS;

    const char* fileType = FileTypes::typeName<ProjectionsReader::Type>
                                      (typeId,ProjectionsReader::types);
    fputs(fileType, f);
    fputc('\n', f);

    bool printInfo = options & (unsigned) Option::PRINT_INFO;

    if (printInfo)
    {
        projections->getGraph().printInfo(f);
    }

    bool result = true;
    unsigned pos = 0u;

    startProcess(pos, list->size() - 1);

    for (const auto &projection : *list)
    {
        if (isInterrupted())
        {
            result = false;
            break;
        }
        updateProgress(++pos);

        result = writeProjection(f, projection, options, false);
        if (!result)
            break;
    }
    fclose(f);
    completeProcess();
    return result;
}



/**
 * @brief ProjectionsWriter::saveProjection - save created projection for
 * one node in fileName file.
 * @param fileName - file to write projections
 * @param pr - created non empty projection
 * @param options - some options (PRINT_INDENTS PRINT_INFO)
 * @return true if writing was successful, false if write error file can't be
 * open on write or projections are empty.
 */
bool ProjectionsWriter::saveProjection(const char *fileName,
                                       const Projection* pr,
                                       cuint options)
{
    if (!pr || !pr->levelCount())
    {
        return false;
    }

    FILE* f = fopen(fileName, "w");
    if (f == nullptr)
    {
        return false;
    }

    ProjectionsReader::Type typeId = ProjectionsReader::Type::PROJECTIONS;

    const char* fileType = FileTypes::typeName<ProjectionsReader::Type>
                                      (typeId, ProjectionsReader::types);
    fputs(fileType, f);
    fputc('\n', f);

    bool printInfo = options & (unsigned) Option::PRINT_INFO;

    if (printInfo)
    {
        projections->getGraph().printInfo(f);
    }
    writeParameters(f, pr);

    unsigned size = pr->getRootNode()->listCount();
    if (size)
        --size;

    startProcess(0u, size);
    bool result = writeProjection(f, pr, options, true);

    completeProcess();

    fclose(f);
    return result;
}



/**
 * @brief ProjectionsWriter::writeProjection - Write in open file f some
 * projection, started from projection root node. It's can be writen in
 * compact mode - inline, or multiline with indents for more readable view.
 * @param f - open file to write projection
 * @param projection - projection to write in file
 * @param options - some options (PRINT_INDENTS)
 * @return true - if projection not empty and writing was successful
 */
bool ProjectionsWriter::writeProjection(FILE *f, const Projection *projection,
                                        cuint options, bool isProgress)
{
    const auto rootNode = projection->getRootNode();
    if (rootNode->isEmpty())
    {
        return false;
    }

    unsigned nodeId, count = 0u;
    bool isFirst = true;
    std::deque <ProjectionElemListItPair> pathStack;
    unsigned currentIndent = 1;

    bool printIndents = options & (unsigned) Option::PRINT_INDENTS;

    const ProjectionElemList* pathList = rootNode->getList();
    const ProjectionElem* pathElem;


    pathStack.push_front({ pathList->begin(), pathList->end() });

    fprintf(f, "%u(", rootNode->getId() );
    if (printIndents)
    {
        fputc('\n', f);
    }
    while (pathStack.size())
    {
        auto pathIt  = pathStack.front().first;
        auto pathEnd = pathStack.front().second;
        if (pathIt == pathEnd)
        {
            pathStack.pop_front();
            --currentIndent;
            if (isProgress && currentIndent == 1)
            {
                updateProgress(++count);
            }
            if (printIndents)
            {
                for(unsigned i = 0u; i < currentIndent; ++i)
                {
                    fputc('\t', f);
                }
                fputs(")\n", f);
            }
            else
            {
                fputc(')', f);
            }
            continue;
        }
        pathElem = *pathIt;
        nodeId = pathElem->getId();
        pathList = pathElem->getList();

        if (printIndents)
        {
            for(unsigned i = 0u; i < currentIndent; ++i)
            {
                fputc('\t', f);
            }
        }
        else if (!isFirst)
        {
            fputc(' ', f);
        }
        else {
            isFirst = false;
        }

        fprintf(f, "%u", nodeId);

        ++(pathStack.front().first);

        if (pathList && pathList->size())
        {
            fputc('(', f);
            // Move to next elem

            // go to next child
            pathStack.push_front({ pathList->begin(), pathList->end() });
            isFirst = true;
            ++currentIndent;
        }
        if (printIndents)
        {
            fputc('\n', f);
        }
    }
    if (!printIndents)
    {
        fputc('\n', f);
    }

    return true;
}



void ProjectionsWriter::writeParameters(FILE *f, const Projection* pr)
{
    fprintf(f, "{ECCENTRICITY=%u}\n", pr->getEccentricity());
    fprintf(f, "{SHORTEST_LOOP=%u}\n", pr->getShortestLoop());
}



bool ProjectionsWriter::saveShortPaths(const char *fileName,
                                       const ProjShortPaths *paths)
{
    if (!paths || !paths->size())
        return false;

    FILE* f = fopen(fileName, "w");
    if (f == nullptr)
    {
        return false;
    }

    for (auto &v : *paths)
    {
        for (auto &el : *v)
        {
            fprintf(f, "%u ", el);
        }
        fputs("\n#\n", f);
    }
    fclose(f);
    return true;
}

