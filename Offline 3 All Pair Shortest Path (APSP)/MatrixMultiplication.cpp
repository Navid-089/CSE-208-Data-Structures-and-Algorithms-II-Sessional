#include <bits/stdc++.h>
using namespace std;

vector<vector<double>> multiply(vector<vector<double>> &helper,int vertices)
{
    vector<vector<double>> result(vertices+1,vector<double> (vertices+1,DBL_MAX));
    for(int i = 1; i<=vertices; i++)
    {
        for(int j = 1; j<=vertices; j++)
        {
            for(int k=1; k<=vertices ; k++)
            {
                result[i][j] = min(result[i][j], helper[i][k]+helper[k][j]);
            }
        }
    }

    return result;
}

void matrixMultiplication(vector<vector<double>> &matrix, int vertices)
{
    vector<vector<double>> helper(vertices+1,vector<double>(vertices+1));
    for(int i=1; i<=vertices; i++)
    {
        for(int j=1; j<=vertices; j++)
        {
            helper[i][j] = matrix[i][j];
        }
    }

    for(int flag = 1; flag <= vertices-1; flag = 2*flag)
    {
        helper = multiply(helper,vertices);
    }

    cout << "Shortest Distance Matrix" << endl;

    for(int i = 1 ; i <= vertices ; i++)
    {
        for(int j = 1 ; j<=vertices ; j++)
        {
            if(helper[i][j] == DBL_MAX)
                cout << "INF ";
            else
                cout << helper[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    ifstream in;
    in.open("in.txt");

    FILE *fp;
    fp = freopen("MatrixMultiplicationOutput.txt","w",stdout);

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

    matrixMultiplication(matrix,vertices);
}
