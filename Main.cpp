/// Main.cpp
/// By: Ibrahim Sardar

#include "GraphCSR.h"
//#include "GraphCSR.cpp"  // <-- for Code::Blocks

#include <iostream>
#include <sstream>
#include <string>

int main(int c, char* a[]){

    //input vertex
    std::stringstream ss(a[2]); //a[3] in CB
    int i; ss >> i;
    std::cout<<"vertex:\n"<<i<<std::endl;

    GraphCSR* g = new GraphCSR();
    g->readGraph(a[1]);//a[2] in CB
    g->bfs(i);
    g->dfs(i);
    g->prim(i);
    g->assignLow(i);
    g->SSP(i);
    delete g;

    return 0;
}//end main
