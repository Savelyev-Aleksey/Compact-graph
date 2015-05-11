#ifndef SHORTPATHROOTELEM_H
#define SHORTPATHROOTELEM_H

#include <cstddef>

class ShortPathElem;



/**
 * @brief The ShortPathRootElem class
 * @param nodes - using for contain short path nodes by hierarchy.
 * Each node contain list of nodes with shortest paths.
 * @param search - using for fast access to every node in hierarchy.
 * Each pointer in map is node parent which contain searching node.
 * It using for easy update hierarchy.
 */
class ShortPathRootElem
{
public:
    ShortPathRootElem(size_t nodeId);
    ~ShortPathRootElem();

    ShortPathElem* getNodes() const;
    ShortPathElem* getSearch() const;

    void setEccentricity(size_t eccentricity);
    void updateEccentricity();
    size_t getEccentricity() const;
    size_t getNodeId() const;

    void clear();

private:
    ShortPathElem* nodes;
    ShortPathElem* search;
    size_t eccentricity;
    size_t nodeId;

};

#endif // SHORTPATHROOTELEM_H
