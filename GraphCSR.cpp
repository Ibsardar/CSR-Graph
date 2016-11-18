/// GraphCSR.cpp
/// By: Ibrahim Sardar
///
/// LOG
/// ->input to CSR done.
///     Error: Pegasus will not compile line 131, even though my compiler does
///     Update: Nevermind, it works. You have to separate the "<",">" from "<<" to "< <"
/// ->undirected done.
/// ->directed done.
/// ->dfs done.
/// ->bfs done.
/// ->Prim's done.
///     Warning: usually will not work with weakly connected graphs
/// ->Articulation Points done.
///     Warning: usually will not work with weakly connected graphs
/// ->Dijkstra's done.
///
/// note: messiness result of time pressure
///

#include "GraphCSR.h"

#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <iostream>
using std::getline; using std::cout; using std::endl;
#include <fstream>
using std::ifstream;
#include <algorithm>
using std::min;



//constructor
GraphCSR::GraphCSR(){
    //...
}

//destructor
GraphCSR::~GraphCSR(){
    //remove all values from vectors
    row_ptr.clear(); col_ind.clear(); val.clear();
}

// converts string to int
int GraphCSR::strToInt(string str){
    stringstream ss( str );
    int i;
    ss >> i;
    return i;
}

// converts string to double
double GraphCSR::strToDub(string str){
    stringstream ss( str );
    double d;
    ss >> d;
    return d;
}

// returns row of an index of col_ind
// note: 'index' lowest value is 1
int GraphCSR::getRow(int index){
    // > i will be the row #
    // > Row_ptr[i] will be the # of
    //   non-0s +1 before the i'th row
    for(int i=0;i<row_ptr.size();i++){
        if(row_ptr[i] == index)
            return i+1;
        if(row_ptr[i] > index)
            return i;
    }
    return -1; //should never happen!
}

// returns index of x if x is found in the list
// if not, returns -1
int GraphCSR::containsVertex( vector<int>& vec, int x ){
    for(int i=0;i<vec.size();i++){
        if(vec[i] == x)
            return i;
    }
    return -1;
}

//returns a list of adjacent vertices to input vertex
//....WOULD HAVE BEEN SO MUCH EASIER IF I MADE THIS IN THE BEGINNING -____-
vector<int> GraphCSR::getAdjs(int vertex){
    vector<int> adjs;
    for(int i=0;i<col_ind.size();i++){
        if(vertex == col_ind[i])
            adjs.push_back( getRow(i+1) );
    }
    return adjs;
}

//returns val at certain col and row
int GraphCSR::getVal(int col, int row){
    for(int i=0;i<col_ind.size();i++){
        if(col==col_ind[i] && row==getRow(i+1))
            return val[i];
    }
    return 0;
}

void GraphCSR::readGraph(char* inputFileName){
    //open file for later operations
    ifstream file; file.open(inputFileName);
    if (!file.is_open())
        cout << ">>> error opening file..." << endl;

    //init 1st line data
    string line, token;
    vector<int> vals; //stores values from 1st line
    getline(file, line); stringstream ss(line);
    while( getline(ss,token,' ') ){
        vals.push_back( strToInt(token) );
    }
    //is graph directed?
    directed = vals[2];
    //warning if it is
    if(directed == true)
        cout<<"\nWARNING: Input graph is directed; some algorithms may not reach every node if weakly connected."<<endl;

    // resize matrix (fills it with 0s)
    vector< vector<double> > matrix; // I have NO IDEA why Pegasus won't compile this!
    for(int woah=0;woah<vals[0];woah++){
        vector<double> vec;cout<<"\nWARN2";
        vec.resize(vals[0]);
        matrix.push_back( vec );
    }

    // input to adjacency matrix
    // (init vars)
    line = ""; token = "";
    int lineCounter=0;
    bool tokenTracker = false;
    string lastToken;

    //loop thru lines:
    while(getline(file, line)){
        ss.str(""); ss.clear();
        ss.str(line);

        //loop thru current line:
        while( getline(ss,token,' ') ){

            if (tokenTracker==false){
                tokenTracker = true;
                lastToken = token;
            }else if (tokenTracker==true){
                tokenTracker = false;
                matrix[strToInt(lastToken)-1][lineCounter] = strToDub(token);
            }

        }//end of line

        lineCounter++;
    }//end of file

    // check matrix
    cout<< "\nMatrix:"<<endl;
    for(int i=0;i<matrix.size();i++){
        for(int j=0;j<matrix[i].size();j++)
            cout<< matrix[i][j] <<" ";
        cout<<endl;
    }

    //convert matrix to CSR
    int  rowTracker = 0;
    bool rowChecker; //handles end of row action
    for (int i=0;i<matrix.size();i++){
        rowChecker = false;
        for(int j=0;j<matrix[i].size();j++){
            if(matrix[i][j] != 0){
                rowTracker++;
                col_ind.push_back(j+1);
                val.push_back(matrix[i][j]);
                if(rowChecker == false){
                    row_ptr.push_back(rowTracker);
                    rowChecker = true;
                }
            }
        }
        if(rowChecker == false)
            row_ptr.push_back(rowTracker);
    }
    row_ptr.push_back(rowTracker+1);

    //check CSR
    cout<<"\nCSR:"<<endl;
    cout<<"row_ptr:";
    for(int i=0;i<row_ptr.size();i++)
        cout<<" "<<row_ptr[i];
    cout<<endl;
    cout<<"col_ind:";
    for(int i=0;i<col_ind.size();i++)
        cout<<" "<<col_ind[i];
    cout<<endl;
    cout<<"val:    ";
    for(int i=0;i<val.size();i++)
        cout<<" "<<val[i];
    cout<<endl;

    //close file
    file.close();
}

void GraphCSR::dfs(int vertex){

    //list of visited vertices
    //start off with input being visited
    vector<int> visited;
    visited.push_back(vertex);
    // col indices = starting nodes
    // row indices = ending nodes
    // recursively prints vertices
    // -using depth first
    cout<<"\ndfs: ";
    dfsRecur(visited, vertex);
    cout<<endl;

}

// recursive depth 1st search algo
void GraphCSR::dfsRecur(vector<int>& visited, int vertex){
    //reduces the calls to 'getRow'
    int row;
    //print current vertex
    cout<<" "<<vertex;
    //checks all edges...
    for(int i=0;i<col_ind.size();i++){
        //...from current vertex...
        if(col_ind[i] == vertex){
            row = getRow(i+1); //(+1 for adjustment: 0..n-1 to 1..n)
            //...that's not visited yet
            if ( containsVertex(visited, row) < 0 ){
                visited.push_back(row);
                dfsRecur( visited,row ); //recursion
            }
        }
    }

}

void GraphCSR::bfs(int vertex){

    //list of visited vertices
    //start off with input being visited
    vector<int> visited;
    visited.push_back(vertex);
    //queue that will take care of the breadth
    //init queue with input
    queue<int> vQueue;
    vQueue.push(vertex);
    //reduces the calls to 'getRow'
    int row;
    // col indices = starting nodes
    // row indices = ending nodes
    // recursively prints vertices
    // -using breadth first
    cout<<"\nbfs: ";
    // --- breadth 1st algo:

    while( !vQueue.empty() ){
        vertex = vQueue.front();
        vQueue.pop();
        cout<<" "<<vertex;

        //checks all edges...
        for(int i=0;i<col_ind.size();i++){
            row = getRow(i+1); //(+1 for adjustment)
            //...from current vertex...
            if(col_ind[i] == vertex){
                //...that's not visited yet
                if ( containsVertex(visited, row) < 0 ){
                    visited.push_back(row);
                    vQueue.push( row );
                }
            }
        }//all edges checked

    }//queue is now empty

    // ---
    cout<<endl;

}

void GraphCSR::prim(int vertex){

    //list of visited vertices
    //start off with input being visited
    vector<int> visited;
    visited.push_back(vertex);
    //finds a minimum spanning tree
    //returns total edge cost of spanning tree
    //prints order of visited nodes
    //recursive
    cout<<"\nPrim's: ";
    double cost = primRecur(visited, vertex);
    cout<<endl<<"Prim's Cost: "<<cost<<endl;

}

double GraphCSR::primRecur(vector<int>& visited, int vertex){

    cout<<" "<<vertex;

    //temp variable for cost
    double cost = 999999999999;
    //keeps track of lowest cost vertex
    int vTmp;
    //reduces the calls to 'getRow'
    int row = -2;
    //base case: visited size == row_ptr size - 1
    if (visited.size() < row_ptr.size()-1){
        //checks all edges...
        for(int i=0;i<col_ind.size();i++){
            //...from visited vertices...
            if( containsVertex(visited, col_ind[i]) >= 0 ){
                row = getRow(i+1); //(+1 for adjustment)
                //...to vertices not visited yet...
                if ( containsVertex(visited, row) < 0 ){
                    //...to find lowest cost edge...
                    if(cost > val[i]){
                        cost = val[i];
                        vTmp = row;
                    }
                }
            }
        }//all edges checked
        if (row != -2) {
            visited.push_back(vTmp);
            return cost + primRecur(visited, vTmp);
        }
    }//base case

    return 0;
}

void GraphCSR::assignLow(int vertex){

    //counter
    int counter=1;
    //list of visited vertices
    vector<int> visited;
    //num, low, and parent same size as # of vertices
    //(filling up with 0s)
    vector<int> num    (row_ptr.size()-1, 0);
    vector<int> low    (row_ptr.size()-1, 0);
    vector<int> parent (row_ptr.size()-1, 0);

    //assign nums to each node by dfs
    cout<<"\n(assignLow)"<<endl;

    assignNumRecur( num,parent,visited,counter,vertex );
    cout<<"num: "; for(int i=0;i<num.size();i++){cout<<" "<<num[i];} cout<<endl;

    //check if source has more than 1 child
    int rootChildrenCount=0;
    for(int i=0;i<parent.size();i++){
        if(parent[i] == 1)
            rootChildrenCount++;
    }

    vector<int> arts;
    assignLowRecur( num,low,parent,arts,rootChildrenCount,vertex );
    cout<<"low: "; for(int i=0;i<low.size();i++){cout<<" "<<low[i];} cout<<endl;

    //prints articulation points, if any
    cout<<"Articulation Points: ";
    for(int i=0;i<arts.size();i++)
        cout<<" "<<arts[i];
    cout<<endl;
}

void GraphCSR::assignNumRecur(vector<int>& num,
                              vector<int>& parent, vector<int>& visited,
                              int& counter, int vertex){
    //current vertex is now visited
    visited.push_back(vertex);
    //increment num, counter
    num[vertex-1] = counter;
    counter++;
    //reduces the calls to 'getRow'
    int row;
    //checks all edges...
    for(int i=0;i<col_ind.size();i++){
        //...from current vertex...
        if(col_ind[i] == vertex){
            row = getRow(i+1); //(+1 for adjustment: 0..n-1 to 1..n)
            //...that's not visited yet
            if ( containsVertex(visited, row) < 0 ){
                //for backtracking a path later
                parent[row-1] = vertex;
                //recursion
                assignNumRecur( num,parent,visited,counter,row );
            }
        }
    }
}

void GraphCSR::assignLowRecur(vector<int>& num, vector<int>& low,
                              vector<int>& parent, vector<int>& arts,
                              int& rootSubCount, int vertex){

    //Rule 1:
    low[vertex-1] = num[vertex-1];
    //reduces the calls to 'getRow'
    int row;
    //checks all edges...
    for(int i=0;i<col_ind.size();i++){
        //...from current vertex...
        if(col_ind[i] == vertex){
            row = getRow(i+1); //(+1 for adjustment)
            if ( num[row-1] > num[vertex-1] ){
                //forward edge
                //recursion
                assignLowRecur( num,low,parent,arts,rootSubCount,row );

                if(containsVertex(arts, vertex) < 0){
                    if( low[row-1] >= num[vertex-1] ){
                        //if vertex is root
                        if(num[vertex-1] == 1){
                            //only add if root has >1 children
                            if (rootSubCount > 1)
                                arts.push_back(vertex);
                        }else{
                            //if not... add.
                            arts.push_back(vertex);
                        }
                    }//may have added an art point
                }//else don't add art point again...

                //Rule 3:
                low[vertex-1] = min( low[vertex-1], low[row-1] );
            //...but if it is visited:
            }else if( parent[vertex-1] != row ){
                //back edge
                //Rule 2:
                low[vertex-1] = min( low[vertex-1], num[row-1] );
            }
        }
    }//all edges checked

}

void GraphCSR::SSP(int source){
    //Dijkstra to find shortest path
    //note: slightly improved some techniques used in Prim's here
    //note: col-1 == index in visited, costs
    //      row-1 == index in visited, costs

    //list of visited nodes
    //init with source as visited
    vector<int> visited;
    visited.push_back(source);
    //*** index of list indicate which vertex-1 ***
    //list of costs from source to vertex 'i'
    //init with source cost as 0
    //the rest of the nodes acting as cost of infinity
    vector<int> cost(row_ptr.size()-1, 999999999);
    cost[source-1] = 0;
    //list of parents of nodes
    //init with parents as 0 (no parent)
    vector<int> parents(row_ptr.size()-1, 0);
    //temp "tracking" nodes
    int next = source;
    int nextParent;
    int col, row, val;

    //calculates all costs from source
    //while all nodes have not been visited
    while(visited.size() != row_ptr.size()-1){

        //new method ======|
        for(int i=0;i<visited.size();i++){
            vector<int> adjs = getAdjs(visited[i]);
            for(int j=0;j<adjs.size();j++){
                col = visited[i];
                row = adjs[j];
                val = getVal(col, row);

                //if not yet visited
                if ( containsVertex(visited, row) < 0 ){
                    //=== Cost Management ===
                    //if cost improves
                    if( (cost[col-1]+val) <= cost[row-1] ){
                        //set cost
                        cost[row-1] = cost[col-1] + val;
                        //to store lowest costing adj node
                        //note: next == source is the initial case
                        if( cost[next-1] > cost[row-1]  ||  next == source){
                            next = row;
                            nextParent = col;
                        }
                    }
                    //end Cost Management ===
                }
            }
        }
        //=================|

        //mark lowest costing adj node as visited
        visited.push_back(next);
        //mark next's parent as parent of lowest costing adj node
        parents[next-1] = nextParent;
        //"reset" next
        next = source;

    }//all nodes visited

    //check some stuff
    cout<<"\ncheck cost:";
    for(int i=0;i<cost.size();i++)
        cout<<" "<<cost[i];
    cout<<"\ncheck visited:";
    for(int i=0;i<visited.size();i++)
        cout<<" "<<visited[i];
    cout<<"\ncheck parents:";
    for(int i=0;i<parents.size();i++)
        cout<<" "<<parents[i];
    cout<<endl;

    //print ssp from every node
    cout<<"\n(Dijkstra)\nAll SSPs from source:"<<endl;
    for(int i=0;i<visited.size();i++){
        //only print path if destination isn't source
        if (visited[i] != source){

            int dest = visited[i];
            int curr = dest;
            int cos  = 0;
            int error = 0;
            vector<int> results; //for reversing

            cout<<source<<" "<<dest<<": 1";
            while(curr != source){
                results.push_back(curr);
                cos  = cos + cost[curr-1];
                curr = parents[curr-1];
                error++; if(error>=row_ptr.size()) {cout<<" >>>ERROR"<<endl; break;}
            }
            //reversing order of output
            for(int i=results.size()-1;i>=0;i--)
                cout<<" -> "<<results[i];
            cout<<": "<<cos<<endl;
        }
    }


}

/*

    PSEUDO ALGORITHM - Dijkstra

     // Find shortest path for all vertices
     for (int count = 0; count < V-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = minDistance(dist, sptSet);

       // Mark the picked vertex as processed
       sptSet[u] = true;

       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < V; v++)

         // Update dist[v] only if is not in sptSet, there is an edge from
         // u to v, and total weight of path from src to  v through u is
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }

     // print the constructed distance array
     printSolution(dist, V);

*/








