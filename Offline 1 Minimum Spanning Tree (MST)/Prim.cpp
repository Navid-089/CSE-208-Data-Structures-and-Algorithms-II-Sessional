#include <bits/stdc++.h>
#include <fstream>
using namespace std;
int root = -1;

struct ComparePairs {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;                     // Compare based on the second parameter (weight)
    }
};

void prim(int vertices, vector <pair<int,int>> adj[], vector <bool> &mst, vector<int> &parents, vector <int> &keys)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, ComparePairs> minheap;     // STL for minheap

    keys[0] = 0 ;               // taking the root node
    parents[0] = -1;            // as there's no edge, setting its parent as -1 (if there are n vertex in a graph, the MST will have (n-1) vertices that's why
    pair<int,int> temp= {0,0};   // pushing the first pair with key or weight 0
    minheap. push(temp);             // pushing it to the minheap

    while(!minheap.empty())
    {
        int u = minheap.top().first;        // pop the pair or edge that's on the top with lowest weight
        mst[u] = true;                      // include it to set of MST
        minheap.pop();


        for(auto i : adj[u])
        {
            int v = i.first;
            int w_temp = i.second;

            if(mst[v] == false && w_temp < keys[v])
            {
                parents[v]= u;
                keys[v] = w_temp;
                minheap.push(make_pair(v,keys[v]));
            }
        }
    }

    int total_weight = 0;

    for(int i = 0 ; i < vertices ; i++)
        total_weight += keys[i];

    cout << "Total weight : " << total_weight << endl;
    cout << "Root node : " << root << endl;

    if(vertices != 0)
    {
        for(int i = 1; i<vertices; i++)                 // (n-1) edges in the MST
            cout << parents[i] << " " << i << endl;
    }
}

void addEdge(vector<pair<int,int>> adj[], int u, int v, int weight)
{
    pair<int,int> temp = make_pair(u,weight);
    adj[v].push_back(temp);
    temp = make_pair(v,weight);
    adj[u].push_back(temp);
}

int main()
{
    FILE *fp;
    fp = freopen("prim.txt","w",stdout);

    ifstream in;
    in.open("in.txt");

    int vertices = 0, edges = 0 ;
    in >> vertices >> edges;

    vector <pair<int,int>> adj[vertices];
    vector <bool> MST(vertices,false);
    vector<int> keys(vertices,INT_MAX);
    vector <int> parents(vertices,-1);

    for(int i = 0 ; i<edges; i++)
    {
        int u,v,weight;
        in >> u >> v >> weight;
        if(i==0) root = u;
        addEdge(adj,u,v,weight);
    }
    prim(vertices,adj,MST,parents,keys);
}



/* Running time of Kruskal -> O(ElogE). Mainly, to sort the edges. */
/* Running time of Prim -> O(ElogV). The inner for loop gets executed 2E times and it takes O(logV) to decrease key.*/

