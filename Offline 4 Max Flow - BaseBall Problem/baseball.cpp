#include <bits/stdc++.h>
using namespace std;

#define V 1000

vector <int> wins(V);            // array for storing wins
vector <int> losses(V);           // array for storing losses
vector <int> remaining(V);      // array for storing remaining matches
vector <int> adjGraph[V*V];        // adjGraph where every node stores a vector of adjacent nodes
vector <int> parent(V,-1);      // for backtracking
int flowCapacity [V][V];            // adjacency matrix

void addEdge(int u,int v)
{
    adjGraph[u].push_back(v);
    adjGraph[v].push_back(u);
}

int BFS(int source, int sink)
{
    for(int i = 0 ; i<V; i++)
        parent[i] = -1;
    queue<pair<int,int>> q;
    q.push(make_pair(source,INT_MAX));
    parent[source] = -2;

    while(!q.empty())
    {
        int from = q.front().first;
        int val = q.front().second;
        q.pop();
        for(auto to: adjGraph[from])
        {
            if(parent[to] == -1 && flowCapacity[from][to] !=0)
            {
                parent[to] = from;
                val = min(val,flowCapacity[from][to]);
                if(to == sink) return val;
                q.push(make_pair(to,val));
            }
        }
    }

    return 0;
}

int edmond_karp(int source, int sink)
{
    int n_f, flow = 0, flag, flag_before;

    while(n_f = BFS(source,sink))               // finds the flow in every way from source to sink
    {
        flag = sink;
        flow += n_f;
        while(flag != source)
        {
            flag_before = parent[flag];
            flowCapacity[flag_before][flag] -= n_f;
            flowCapacity[flag][flag_before] += n_f; // reverse edge
            flag = flag_before;                     // iterating backwards until source is found
        }

    }
    return flow;   // returns max flow for a source and sink
}

int main()
{
//
//    ifstream cin;
//    cin.open("in.txt");
//
//    FILE *fp;
//    fp = freopen("output.txt","w",stdout);

    int num;
    cin >> num;
    int games[num+1][num+1];
    string teamNames[num+1];
    for(int i = 1; i <= num; i++)
    {
        cin >> teamNames[i];
        cin >> wins[i] >> losses[i] >> remaining[i];

        for( int j = 1; j<= num; j++) cin >> games[i][j];
    }

    for(int i = 0 ; i<=num; i++)
        games[i][i] = 0;

    int source = 0, sink =  num + ((num-1)*(num-2))/2 + 2;


    for(int k = 1 ; k<= num ; k++)
    {
//        minCutArray.clear();
        for(int i = 0; i<V; i++)
        {
            adjGraph[i].clear();
            for(int j = 0 ; j<V; j++) flowCapacity [i][j] = 0;
        }

        int flag = 1;
        int nodes = ((num-1)*(num-2))/2;
        for(int i =  1; i<=num ; i++)
        {
            if(i == k ) continue;
            for(int j=i+1; j<=num; j++)
            {
                if(j==k) continue;
                addEdge(source,flag);
                flowCapacity[source][flag] = games[i][j];
                addEdge(flag,nodes+i);
                flowCapacity[flag][nodes+i] = 10000;
                addEdge(flag,nodes+j);
                flowCapacity[flag][nodes+j] = 10000;
                flag++;
            }
        }
        bool Eliminated = true;
        int f = 0;
        for(int i=1; i<=num; i++)
        {
            if(i==k) continue;
            addEdge(nodes+i,sink);
            flowCapacity[nodes+i][sink] = (wins[k]+remaining[k] - wins[i]);
            if(flowCapacity[nodes+i][sink] < 0) Eliminated = false, f=-1;
        }
        if(Eliminated == false)
        {
            cout << teamNames[k] << " is eliminated. " << endl;
            cout << "They can win at most " << wins[k] <<"+" << remaining[k] << " matches." << endl;
            f=-1;

        }
        else
        {
            int tmp = edmond_karp(source,sink);
//            cout << "MAXFLOW: " << tmp << endl;

            for(int i = 1; i<=nodes; i++)
            {
                if(flowCapacity[source][i] != 0)
                {
                    cout << teamNames[k] << " is eliminated. " << endl;
                    cout << "They can win at most " << wins[k] <<"+" << remaining[k] << " matches." << endl;
                    f=-1;
                    break;
                }
            }
        }

        if(f==-1)
        {
            BFS(source,sink);
//            cout << "SINK" << parent[sink] << endl;
//
//            for(int i = 0 ; i< V ; i++)
//                cout << parent[i] << " ";
//            cout << endl;
            vector <int> reachable(num);
            int rflag = 0;
            for(int it = 1; it<=num; it++)
            {
                if(it==k) continue;
//                cout << "NUM:" << num << endl;
                int final = nodes+it;
//                cout << "PAR:" << teamNames[it] << " " << parent[nodes+it] << endl;
                while(parent[final] > 0)
                {
//                    cout << parent[final] << " ";

                        final = parent[final];

                }

//                cout << "hello" << endl << endl;

//                cout << "PAR:" << parent[final] << endl;

                if(parent[final] == -2)
                {
                    reachable.push_back(it);
                    rflag++;
                }


            }

//


//            for(int i = 0 ; i<reachable.size(); i++)
//                cout << teamNames[reachable[i]] << "&";
//            cout << endl;



//            for(int i = 0 ; i<reachable.size(); i++)
//                cout << teamNames[reachable[i]] << " ";
//            cout << endl;
//
//                for(int m = 0 ; m<reachable.size(); m++)
//                    cout << reachable[m] << "&" << endl;


//

//            if(!rflag)
//            {
//                cout << "RFLAG : " << rflag << endl;
////
//                int w_sum = 0;
//                for(int i = 0; i<rflag-2; i++)
//                {
//                    cout << teamNames[reachable[i]] << " and ";
//
//                }
//
//                for(int i = 0 ; i<rflag; i++)
//                {
//                    int found = reachable[i];
//                    w_sum+=wins[found];
//                }
//                cout << teamNames[reachable[rflag-1]] << "have won a total of " << w_sum << " ." << endl;
//                cout << " meow" << endl;
//                cout << "So, on average, each of the teams wins " << w_sum << "/" << rflag << " = " << w_sum/rflag << " games." << endl;
////
//            }

        }

    }

    return 0;
}



