#ifndef GRAPHWORKER_H
#define GRAPHWORKER_H

#include <functional>


#include "graph/Graph.h"


class QWidget;
class QString;


/**
 * @brief The GraphWorker class GUI wrapper of main graph class.
 * This class represents progressbar on slow tasks.
 */
class GraphWorker : public Graph
{
public:
    GraphWorker(QWidget* parent);
    virtual ~GraphWorker();

    const Node* findNode(unsigned nodeId, QString& info) const;


    // Graph writers
    virtual bool writeEdges(const char* fileName, cuint options = (cuint)
                    WriterBase::Option::PRINT_VALUE );

    virtual bool writeBracketsFlat(const char* fileName, cuint options = (cuint)
                            WriterBase::Option::PRINT_VALUE );

    virtual bool writeBrackets(const char* fileName, cuint startNodeId,
                       cuint pathLimit = 0,
                       cuint options = (cuint) WriterBase::Option::PRINT_VALUE);


    // Projections writers
    virtual bool saveProjections(const char* fileName,
                                 cuint options = (cuint) Option::NONE);
    virtual bool saveProjection(const char* fileName, cuint rootNode,
                                cuint options = (cuint) Option::NONE);

    virtual void createAllProjections();

public:
    // Short path
    virtual void createAllShortPaths();


protected:

    QWidget* parent;

    void progressDialog(unsigned size, const QString& title,
                        std::function<void()> progressFn);

};

#endif // GRAPHWORKER_H
