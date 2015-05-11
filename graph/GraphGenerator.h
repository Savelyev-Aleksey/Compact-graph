#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <cstddef>


class GraphBase;


class GraphGenerator
{
public:
    GraphGenerator(GraphBase& graph);
    ~GraphGenerator();

    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

private:
    GraphBase* graph;
};

#endif // GRAPHGENERATOR_H
