#ifndef SHORTPATHELEM_H
#define SHORTPATHELEM_H

#include <cstddef>
#include <map>

class ShortPathElem;

typedef std::map <size_t, ShortPathElem*> PathList;
typedef std::pair<PathList::const_iterator,
                  PathList::const_iterator> PathListItPair;

class ShortPathElem
{
public:
    ShortPathElem(size_t nodeId, float weight, size_t indent);
    ~ShortPathElem();

    ShortPathElem* addNodeElem(size_t nodeNum, float weight, size_t indent);
    void addNodeElem(size_t nodeNum, ShortPathElem* pathElem);
    bool popNodeElem(size_t nodeNum);

    void setWeight(float newWeight);
    float getWeight() const;

    void setIndent(size_t newIndent);
    size_t getIndent() const;

    void setParent(ShortPathElem* newParent);
    ShortPathElem* getParent() const;

    ShortPathElem* findElem(size_t nodeNum) const;

    PathList* getPathList() const;

    size_t size() const;

protected:

    ShortPathElem* parent;
    PathList* list;
    size_t nodeId;
    float weight;
    size_t indent;

};

#endif // SHORTPATHELEM_H
