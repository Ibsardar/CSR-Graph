/// GraphCSR.h
/// By: Ibrahim Sardar

#ifndef GRAPHCSR_H_EXISTS
#define GRAPHCSR_H_EXISTS

#include <vector>
using std::vector;
#include <string>
using std::string;



class GraphCSR{

    private:
        bool directed;
        vector<int> row_ptr;
        vector<int> col_ind;
        vector<double> val;
        int strToInt(string); //helper function
        double strToDub(string); //helper function
        int getRow(int); //helper function
        int containsVertex(vector<int>&, int); //helper function
        void dfsRecur(vector<int>&, int); //recursive function
        double primRecur(vector<int>&, int); //recursive function
        void assignNumRecur(vector<int>&, vector<int>&, vector<int>&,
                            int&, int); //recursive function
        void assignLowRecur(vector<int>&, vector<int>&, vector<int>&,
                            vector<int>&, int&, int); //recursive function
        vector<int> getAdjs(int); //helper function
        int getVal(int, int); //helper function

    public:
        GraphCSR();
        ~GraphCSR();
        void readGraph(char*);
        void dfs(int);
        void bfs(int);
        void prim(int);
        void assignLow(int);
        void SSP(int);

};//end class



#endif



