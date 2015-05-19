#ifndef GRAPHWORKER_H
#define GRAPHWORKER_H

#include <functional>


#include "graph/Graph.h"


class QWidget;


/**
 * @brief The GraphWorker class GUI wrapper of main graph class.
 * This class represents progressbar on slow tasks.
 */
class GraphWorker : public Graph
{
public:
    GraphWorker(QWidget* parent);
    virtual ~GraphWorker();


    // Graph writers
    virtual bool writeEdges(const char* fileName, cuint options =
                    WriterBase::PRINT_VALUE );

    virtual bool writeBracketsFlat(const char* fileName, cuint options =
                            WriterBase::PRINT_VALUE );

    virtual bool writeBrackets(const char* fileName, const size_t startNodeId,
                       const size_t pathLimit = 0,
                       cuint options = WriterBase::PRINT_VALUE);


public:
    // Short path
    virtual void generateAllShortPaths();

protected:

    QWidget* parent;

    void progressDialog(size_t size, const QString& title,
                        std::function<void()> progressFn);

};

#endif // GRAPHWORKER_H
