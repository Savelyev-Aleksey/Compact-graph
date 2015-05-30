#ifndef SHORTPATHELEM_H
#define SHORTPATHELEM_H

#include <cstddef>
#include <map>

class ShortPathElem;

typedef std::map <unsigned, ShortPathElem*> PathList;
typedef std::pair<PathList::const_iterator,
                  PathList::const_iterator> PathListItPair;

class ShortPathElem
{
public:
    ShortPathElem(unsigned nodeId, float weight, unsigned indent);
    ~ShortPathElem();

    ShortPathElem* addNodeElem(unsigned nodeNum, float weight, unsigned indent);
    void addNodeElem(unsigned nodeNum, ShortPathElem* pathElem);
    bool popNodeElem(unsigned nodeNum);

    void setWeight(float newWeight);
    float getWeight() const;

    void setIndent(unsigned newIndent);
    unsigned getIndent() const;

    void setParent(ShortPathElem* newParent);
    ShortPathElem* getParent() const;

    ShortPathElem* findElem(unsigned nodeNum) const;

    PathList* getPathList() const;

    unsigned size() const;

protected:

    ShortPathElem* parent;
    PathList* list;
    unsigned nodeId;
    float weight;
    unsigned indent;

};

#endif // SHORTPATHELEM_H
