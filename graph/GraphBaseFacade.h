#ifndef GRAPHBASEFACADE_H
#define GRAPHBASEFACADE_H



#include <string>


#include "ReaderBase.h"
#include "Worker.h"
#include "GraphBase.h"
#include "WriterBase.h"
#include "GraphReader.h"



class GraphBase;


class GraphBaseFacade :
        virtual public ReaderBase,
        virtual public Worker
{
public:
    typedef const WriterBase::Option WOption;
    typedef GraphReader::Type Type;

    GraphBaseFacade();
    virtual ~GraphBaseFacade();

    unsigned getRadius() const;
    unsigned getDiameter() const;
    unsigned getGraphGirth() const;

    bool isGraphEmpty() const;
    unsigned nodeCount() const;
    unsigned edgeCount() const;

    virtual void clear();


    const InfoDeque* getAllInfo() const;
    const std::string& getFileName() const;

    // Generators
    void generateHypercube(unsigned dimention, float weight);
    void generateTorus(unsigned bigRadius, unsigned smallRadius, float weight);

    // Statistics
    UintMap* getNodeDegreeStatistic() const;
    UintMap* compactStatistic(UintMap* map, unsigned short limit) const;
    unsigned statisticParamSum(const UintMap* map) const;


    static bool isCanRead(const char* typeStr);
    static Type getType(const char* type);

    virtual bool readFile(const char* fileName);
    bool readFile(FILE* f, Type typeId);


    // Node finders
    const Node* findNode(unsigned nodeId) const;
    uint_vec* findNearNode(unsigned nodeId) const;

    // Graph writers
    virtual bool writeEdges(const char* fileName, cuint options =
                            (cuint) WOption::PRINT_VALUE );

    virtual bool writeBracketsFlat(const char* fileName, cuint options =
                                   (cuint) WOption::PRINT_VALUE );

    virtual bool writeBrackets(const char* fileName, cuint startNodeId,
                               cuint pathLimit = 0u,
                               cuint options = (cuint) WOption::PRINT_VALUE);

protected:
    GraphBase* graph;
    std::string fileName;

    unsigned radius;
    unsigned diameter;
    unsigned girth;


    virtual void updateFileName(const char* newName);
};

#endif // GRAPHBASEFACADE_H
