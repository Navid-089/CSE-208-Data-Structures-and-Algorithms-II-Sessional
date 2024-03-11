/*  
    Running time of Prim's -> O(ElogV)
    Running time of Kruskal's -> O(ElogE)
    Running time of Dijkstra's -> O(ElogV)
    Running time of Bellman-Ford -> O(VE) // if a complete graph, it will take O(V^3)
    Running time of Floyd-Warshall -> O(V^3)
*/



#include <bits/stdc++.h>
using namespace std;

class edge
{
public:
    int u;
    int v;
    double weight;

    edge(int u, int v, int weight)
    {
        this->u = u ;
        this->v = v;
        this->weight = weight;
    }
};


struct ComparePairs
{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b)
    {
        return a.second > b.second;                     // Compare based on the second parameter (weight)
    }
};

void shortestPathDijkstra(vector <pair<int,double>> adj[], int vertices, int source, int dest)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, ComparePairs> minheap;

    vector <double> val(vertices,DBL_MAX);
    vector <int> parent(vertices,-1);
    vector<int> path;
    val[source] = 0;
    parent[source] = -1;
    pair<int,double> temp = make_pair(source,0.0);
    minheap.push(temp);

    while(!minheap.empty())
    {
        int dist,from;
        // dist = minheap.top().second;
        from = minheap.top().first;
        minheap.pop();

        for(auto i : adj[from])
        {
            int to, to_val;
            to = i.first;
            to_val = i.second;

            if(val[to] > val[from] + to_val)
            {
                val[to] = val[from] + to_val;
                minheap.push(make_pair(to,val[to]));
                parent[to] = from;
            }
        }
    }
    cout << "Dijkstra Algorithm : " << endl << val[dest] << endl;
    int i = dest;
    while(i!=source)
    {
        path.push_back(i);
        i = parent[i];
    }
    path.push_back(source);
    for(int i = path.size() - 1; i>0 ; i--)
        cout << path[i] << "->";
    cout << path[0] << endl;
}

void bellman(vector <edge> edges,int vertices,int source,int dest)
{
    vector <int> parent(vertices,-1);
    vector <double> value(vertices,DBL_MAX);
    vector <int> path;
    bool hasNegCycle;
    value[source] = 0;
    parent[source] = -1;
    for(int i = 0 ; i< vertices-1; i++)
    {
        for(auto ed : edges)
        {
            if(value[ed.v] > value[ed.u] + ed.weight)
            {
                value[ed.v] = value[ed.u] + ed.weight;
                parent[ed.v] = ed.u;
            }
        }
    }

    cout << "Bellman-Ford algorithm: " << endl;

    for(auto ed : edges)
    {
        if(value[ed.v] > value[ed.u] + ed.weight)
        {
            cout << "Negative weight cycle present" << endl;
            hasNegCycle = true;
            break;
        }

    }

    if(hasNegCycle == false)
    {
        cout << value[dest] << endl;
        int i = dest;
        while(i!=source)
        {
            path.push_back(i);
            i = parent[i];
        }
        path.push_back(source);
        for(int i = path.size() - 1; i>0 ; i--)
            cout << path[i] << "->";
        cout << path[0] << endl;

    }
}

void addEdge(vector <pair<int,double>> adj[], int u, int v, double weight)
{
    pair <int,double> temp = make_pair(v,weight);
    adj[u].push_back(temp);
}

int main()
{
    ifstream in;
    in.open("in.txt");

    FILE *fp;
    fp = freopen("output.txt","w",stdout);

    int vertices = 0,ed = 0, weight_dijkstra = 0;
    int source,dest;
    in >> vertices >> ed;
    vector <pair<int,double>> adj[vertices];
    vector <edge> edges;
    for(int i = 0; i<ed; i++)
    {
        int u,v;
        double weight;
        in >> u >> v >> weight;
        weight_dijkstra = abs(weight);
        addEdge(adj,u,v,weight_dijkstra);
        edges.push_back(edge(u,v,weight));
    }

    in >> source >> dest;
    shortestPathDijkstra(adj,vertices,source,dest);
    bellman(edges,vertices,source,dest);
}
