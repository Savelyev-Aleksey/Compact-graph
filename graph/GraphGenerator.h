#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <cstddef>


#include "Worker.h"


class GraphBase;


class GraphGenerator : public Worker
{
public:
    GraphGenerator(GraphBase& graph);
    ~GraphGenerator();

    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

private:
    GraphBase* graph;

    unsigned number_capacity(unsigned number);
};

#endif // GRAPHGENERATOR_H
