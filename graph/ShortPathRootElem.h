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
    ShortPathRootElem(unsigned nodeId);
    ~ShortPathRootElem();

    ShortPathElem* getNodes() const;
    ShortPathElem* getSearch() const;

    unsigned size() const;

    void setEccentricity(unsigned eccentricity);
    void updateEccentricity();
    unsigned getEccentricity() const;
    unsigned getNodeId() const;

    void clear();

private:
    ShortPathElem* nodes;
    ShortPathElem* search;
    unsigned eccentricity;
    unsigned nodeId;

};

#endif // SHORTPATHROOTELEM_H
