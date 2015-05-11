#include <cstddef>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>

#include "GraphBase.h"
#include "GraphReader.h"
#include "GraphWriter.h"
#include "GraphGenerator.h"
#include "ShortPath.h"

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

int main()
{
    std::ofstream logout("graph.log");
    std::clog.rdbuf(logout.rdbuf());

    std::clog << "Program started " << currentDateTime() << std::endl
              << std::endl;

//    GraphBase graph;
//    GraphReader reader(graph);
//    reader.readFile("graph.txt");
//    ShortPath path(graph);
//    path.generateShortPath(1);
//    NodeList list;
//    list.push_front(1);
//    unsigned options = ShortPath::Option::PRINT_INDENTS;
//    path.saveShortPaths("flat_2.txt", &list, 0, options);
//    GraphGenerator g(graph);
//    g.generateHypercube(4, 1);

//    writer.writeEdges("cube.txt");

    return 0;
}

