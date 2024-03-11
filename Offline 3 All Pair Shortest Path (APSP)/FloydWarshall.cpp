#include <bits/stdc++.h>
using namespace std;

/* Floyd Warshall is executed by taking intermediate nodes.
 The running time of Floyd Warshall is O(V^3)*/

void floydWarshall(vector<vector<double>> &matrix, int vertices)
{
    for(int i=1 ; i<=vertices; i++)
    {
        for(int j=1; j<=vertices; j++)
        {
            for(int k = 1 ; k<=vertices; k++)
            {
                if(matrix[j][i] != DBL_MAX && matrix[i][k] != DBL_MAX &&
                        matrix[j][k] > matrix[j][i] + matrix[i][k])
                {
                    matrix[j][k] = matrix[j][i] + matrix[i][k];

                }
            }
        }

     for(int i = 1 ; i <= vertices ; i++)
    {
        for(int j = 1 ; j<=vertices ; j++)
        {
            if(matrix[i][j] == DBL_MAX)
                cout << "INF ";
            else
                cout << matrix[i][j] << " ";

        }
       
    }

    cout << endl << endl << endl;
        
    }

    cout << "Shortest Distance Matrix" << endl;

    for(int i = 1 ; i <= vertices ; i++)
    {
        for(int j = 1 ; j<=vertices ; j++)
        {
            if(matrix[i][j] == DBL_MAX)
                cout << "INF ";
            else
                cout << matrix[i][j] << " ";

        }
        cout << endl;
    }
}

int main()
{
    ifstream in;
    in.open("in.txt");

    FILE *fp;
    fp = freopen("floydWarshall.txt","w",stdout);

    int vertices,ed;
    int from,to;
    double weight;
    in >> vertices >> ed;

    vector<vector<double>> matrix(vertices+1, vector<double> (vertices+1,DBL_MAX));
    
    for(int i = 1 ; i<=vertices; i++)
    {
        matrix[i][i] = 0;
    }

    for(int i = 0 ; i<ed; i++)
    {
        in >> from >> to >> weight;
        matrix[from][to] = weight;
    }
    floydWarshall(matrix,vertices);
}

