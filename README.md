## Compact graph

This is simple program for generate periodical graph structures,
for print information about graph: diameter, radius, eccentricity.
And some statistic about nodes degree and eccentricity.

This is c++ program used Qt for gui
For compile program add c++11 option


This is simple program can do: 
*   Read text file contain graph in following formats:
    > * \{NODE_NODE\[_VALUE\]\} with basic weight value or with different weight values
    > * \{BRACKETS_FLAT\[_VALUE\]\} graph in brackets notation without nesting with basic weight or different weights
    > * \{BRACKETS\[_VALUE\]\} graph in brackets notation with nesting with basic weight or different weights
*   Save in files graph with same as read formats
*   Save in files generated short paths of graph with path weight
*   Generate periodical graph structures: at this moment only hypercubes and torus
    > * Print graph statistic information: 
    > * Graph node, edge, generated short path count
    > * Graph diameter and radius
    > * Nodes indents statistic
    > * Nodes eccentricity sratistic
