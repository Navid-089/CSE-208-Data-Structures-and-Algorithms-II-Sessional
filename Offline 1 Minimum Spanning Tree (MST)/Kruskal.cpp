#include <bits/stdc++.h>
using namespace std;

class edge
{
public:
    int u;
    int v;
    int weight;

    edge(int u,int v, int weight)
    {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }
};

int find(int i, vector <int> &parent)
{
    if(i == parent[i]) return i;
    return parent[i] = find(parent[i], parent);

}


void unionOfI(int u, int v, vector <int> &parent, vector <int> &rank)
{
    u = find(u,parent);
    v = find(v,parent);
    if(rank[u] < rank[v]) parent[u] = v;
    else if(rank[v] < rank[u]) parent[v] = u;
    else
    {
        parent[v] = u;
        rank[u]++;
    }
}




void kruskal(vector <edge> &edges, vector <int> &parent, vector <int> &rank, int vertices)
{
    for(int i = 0 ; i<vertices ; i++)
        parent[i] = i;

    int total_weight = 0 ;
    vector<pair<int,int>> mst;

    for(auto i: edges)
    {
        if(find(i.u,parent) != find(i.v,parent))
        {
            total_weight += i.weight;
            mst.push_back(make_pair(i.u,i.v));
            unionOfI(i.u,i.v,parent,rank);
        }
    }

    cout << "Total weight : " << total_weight << endl;
    for(auto i : mst)
        cout << i.first << " " << i.second << endl;
}

bool inc_compare(edge a, edge b)
{
    return a.weight < b.weight;
}

int main()
{
    ifstream in;
    in.open("in.txt");

    FILE *fp;
    fp = freopen("kruskal.txt","w",stdout);

    int vertices = 0, ed = 0 ;
    in >> vertices >> ed;
    vector <edge> edges;
    vector <int> parent(vertices);
    vector <int> rank(vertices,0);

    for(int i = 0; i<ed; i++)
    {
        int u,v,w;
        in >> u >> v >> w;
        edges.push_back(edge(u,v,w));
    }

    sort(edges.begin(),edges.end(),inc_compare);
    kruskal(edges,parent,rank,vertices);

}


