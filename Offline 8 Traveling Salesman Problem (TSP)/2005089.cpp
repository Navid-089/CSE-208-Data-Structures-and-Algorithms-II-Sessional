#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

struct ComparePairs {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    }
};

class Graph {
public:
    int V;
    vector<vector<int>> adj;

    Graph() 
    {
        V = 0 ;

    }

    Graph(int V) {
        this->V = V;
        adj.resize(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v, int w) {
        adj[u][v] = w;
        adj[v][u] = w;
    }

    vector <vector<int>>  create_Random_Graph(int V) {
        this->V = V;
        adj.resize(V, vector<int>(V, 0));

      for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            int w = rand() % 151 + 50;
            addEdge(i, j, w);
        }
    }
    return adj;
    }


   vector<int> findTourForOptimal(vector<vector<int>> &graph, vector<vector<int>> &soln)
    {
        int curr = 0 ;
        int flag = 1 ;
        vector<int> path;
        path.push_back(curr);

        while(flag != ((1<<V)-1))
        {
            int next = -1;
            for(int i = 0 ; i< V ; i++)
            {
                // cout << flag << endl;
                if((flag & (1 << i)) == 0)
                {
                    if(next == -1 || soln[flag][curr] == graph[curr][i] + soln[flag | (1 << i)][i])
                        next = i;
                }
            }
           
            flag = flag | (1 << next);
            curr = next;
            path.push_back(next);
            
         }
         path.push_back(0);

         return path;

    }

    int minTSP(vector<vector<int>> &graph, vector<vector<int>> &soln, int curr, int flag)
    {
        if(flag == ((1<<V)-1)) return graph[curr][0];
        if(soln[flag][curr] != -1) return soln[flag][curr];
       

        
        int minimumCost = INF;
        // cout << " HI " << endl;
        for(int i = 0 ; i<V; i++)
        {
            if((flag & (1<<i)) == 0)
            {
                int tmp = graph[curr][i] + minTSP(graph, soln, i, (flag | (1 << i))) ;
                // cout << "TMP " << tmp << endl;
                if(tmp < minimumCost)  minimumCost=tmp;
            }
        }

        soln[flag][curr] = minimumCost;

        return minimumCost;
    }

    vector <int> exact_TSP(vector<vector<int>> &graph)
    {
        vector <vector<int>> soln(1<<V, vector<int> (V,0));
        for(int i = 0 ; i< soln.size() ; i++)
            for(int j = 0  ; j<soln[i].size() ; j++)
                soln[i][j]= -1;

        //  for(int i = 0 ; i< soln.size() ; i++)
        //  {
        //     for(int j = 0  ; j<soln[0].size() ; j++)
        //         cout << soln[i][j] << " ";
        //     cout << endl;
         //  }

        int cost = minTSP(graph, soln, 0, 1); 
        cout << "Optimal length found from minTSP: " << cost << endl;
        return findTourForOptimal(graph,soln);
    }

    vector <int> MST(vector<vector<int>> graph)
    {
        int size = graph.size();
        
        vector<int> parent(size,-1);
        vector<bool> visited(size, false);
        vector<int> values(size, INF);
        

        priority_queue<pair<int, int>, vector<pair<int, int>>, ComparePairs> minheap;

        values[0] = 0 ;
        pair<int,int> temp = {0,0};
        minheap.push(temp);

        while(!minheap.empty())
        {
            int u = minheap.top().first;
            visited[u] = true;
            minheap.pop();

            for(int v = 0 ; v<V ; v++)
            {
                if(graph[u][v] < values[v] && !visited[v])
                {
                    values[v] = graph[u][v];
                    parent[v] = u;
                    minheap.push(make_pair(v,values[v]));
                }
            }

        }

        return parent;

    }

    void m_tour(vector<int> &parents, vector <int> &path, int n)
    {
        path.push_back(n);

        for(int i = 0 ; i<parents.size() ; i++)
            if(parents[i] == n) m_tour(parents, path, i);
    }

    vector <int> Metric_Approximation_TSP(vector<vector<int>> &graph)
    {
        vector<int> par = MST(graph);
        vector <int> p;
        m_tour(par,p,0);
        p.push_back(0);
        return p;
    }




    int calculate_Tour_Length(vector<int> path)
    {
        int sum  = 0;
        for(int i = 0 ; i< path.size() - 1;i++)
            sum = sum + adj[path[i]][path[i+1]];
        return sum;
    }
};


int main()
{
    srand(2005089);

    int n;
    cin >> n;
    Graph gr(20);
  
   
    vector<vector<int>> res;

    for(int i = 0 ; i<n; i++)
    {
      
        res = gr.create_Random_Graph(20);
        // for(int i = 0 ; i<20 ; i++)
        // {for(int j = 0 ; j<20 ; j++)
        //         cout << gr.adj[i][j] << " " ;
        //     cout << endl;

        // }
            
        cout << "\nTEST CASE " << i+1 << endl;
        int a_cost = gr.calculate_Tour_Length(gr.Metric_Approximation_TSP(res));
        cout << "Approximation Length: " << a_cost << endl;
        //  cout << a_cost << endl;
        int o_cost = gr.calculate_Tour_Length(gr.exact_TSP(res));
        cout << "Optimal Length after calculating the tour: " << o_cost << endl;
        // cout << gr.V << endl;
        res.clear();
        cout << "Ratio = " << (double) a_cost / o_cost << endl;
    }
    
}












