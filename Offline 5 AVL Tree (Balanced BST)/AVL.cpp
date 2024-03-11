#include <bits/stdc++.h>
using namespace std;


class node
{
public:
    double key;
    node *left;
    node *right;
    int height;

    node(double key)
    {
        this->key = key;
        height = 1;
        left = NULL;
        right = NULL;
    }

    bool isLeaf()
    {
        return (left == NULL && right == NULL);
    }
};

class AVL
{
private:
    node *root ;
    int numOfNodes;

    int height(node *head)
    {
        if(head == NULL) return 0;
        else
            return head->height;
    }

    node *leftRotation(node *head)
    {
        node *tmp = head->right;
        node *tmp2 = tmp->left;
        tmp->left = head;
        head->right = tmp2;

        head-> height = 1+max(height(head->left),height(head->right));
        tmp-> height = 1+max(height(tmp->left),height(tmp->right));
        return tmp;
    }

    node *rightRotation(node *head)
    {
        node *tmp = head->left;
        node *tmp2 = tmp->right;
        tmp->right = head;
        head->left = tmp2;
        head-> height = 1+max(height(head->left),height(head->right));
        tmp-> height = 1+max(height(tmp->left),height(tmp->right));
        return tmp;
    }


    node *insertHelper(node *head, double key)
    {

        if(head == NULL)
        {
            node *tmp = new node(key);
            numOfNodes++;

            return tmp;
        }

        if(key < head->key)
            head->left = insertHelper(head->left,key);
        else if(key > head->key)
            head->right = insertHelper(head->right,key);

        head->height = 1+max(height(head->left),height(head->right));


        int balance_factor = height(head->left) - height(head->right);

        if(balance_factor > 1)
        {
            if(key < head->left->key)
                return rightRotation(head);
            else
            {
                head->left = leftRotation(head->left);
                return rightRotation(head);
            }
        }
        else if(balance_factor < -1)
        {
            if(key > head->right->key)
                return leftRotation(head);
            else
            {
                head->right = rightRotation(head->right);
                return leftRotation(head);
            }
        }
        return head;
    }

    node *removeHelper(node *head, double key)
    {

        if(head == NULL) return NULL;
        if(key < head->key) head->left = removeHelper(head->left, key);
        else if(key > head->key) head->right = removeHelper(head->right, key);
        else
        {
            node *tmp_right = head->right ;
            if(head->right == NULL)
            {
                node *tmp_left = head->left;
                delete head;
                head = tmp_left;
            }
            else if(head->left == NULL)
            {
                delete head;
                head = tmp_right;
            }
            else
            {
                while(tmp_right->left !=NULL) tmp_right = tmp_right -> left;
                head->key = tmp_right->key;
                head->right = removeHelper(head->right,tmp_right->key);
            }

        }

        if(head == NULL) return head;
        head->height = 1 + max(height(head->left), height(head->right));


        int balance_factor = height(head->left) - height(head->right);

        if(balance_factor > 1)
        {
            if(height(head->left) >= height(head->right))
                return rightRotation(head);
            else
            {
                head->left = leftRotation(head->left);
                return rightRotation(head);
            }
        }
        else if(balance_factor < -1)
        {
            if(height(head->right) >= height(head->left))
                return leftRotation(head);
            else
            {
                head->right = rightRotation(head->right);
                return leftRotation(head);
            }
        }

        return head;
    }

    node *searchHelper(node *head, double key)
    {
        if(head == NULL) return NULL;
        if(key == head->key) return head;
        else if(key > head->key) return searchHelper(head->right, key);
        else if(key < head->key) return searchHelper(head->left,key);
    }

    void inOrderTraversalHelper(node *head,std::ofstream& out1)
    {
        if(head == NULL) return;
        inOrderTraversalHelper(head->left,out1);
        out1 << head->key << " ";
        inOrderTraversalHelper(head->right,out1);
    }

    void printHelper(node *head,std::ofstream& out1)
    {
        if(head == NULL) return;

        out1 << head->key;
        if( head -> isLeaf() == false)
        {
            out1<< "(";
            printHelper(head->left,out1);
            out1 << "),(";
            printHelper(head->right,out1);
            out1 << ")";
        }
    }


public:


    AVL()
    {
        root = NULL;
        numOfNodes = 0;
    }

    void insert(double key,std::ofstream& out1)
    {

        root=insertHelper(root,key);
        print(out1);

    }
    void remove(double key,std::ofstream& out1)
    {
        root=removeHelper(root,key);
        print(out1);
    }
    void search(double key,std::ofstream& out1)
    {
        node *tmp = searchHelper(root,key);
        if(tmp == NULL) out1 << "Not found" << endl;
        else out1 << "Found" << endl;
    }
    void traverse(std::ofstream& out1)
    {
        inOrderTraversalHelper(root,out1);
        out1 << endl;
    }

    void print(std::ofstream& out1)
    {
        printHelper(root,out1);
        out1 << endl;
    }
};


int main()
{
    AVL av1;
    ifstream cin;
    cin.open("in.txt");
    std::ofstream out1("out_avl.txt");
    std::ofstream out2("report_avl.txt");

    out2 << "Operation time(ms)" << endl;

    std::chrono::duration<double, std::milli> insertion_time(0);
    std::chrono::duration<double, std::milli> deletion_time(0);
    std::chrono::duration<double, std::milli> search_time(0);
    std::chrono::duration<double, std::milli> trav_time(0);


    int insertion_count = 0, deletion_count = 0, search_count = 0, trav_count = 0;

    char c;
    double key;

    while(cin >> c)
    {
        if(c == 'I')
        {
            cin >> key;
            auto start = std::chrono::high_resolution_clock::now();
            av1.insert(key,out1);

            auto end = std::chrono::high_resolution_clock::now();

            insertion_time += (end-start);
            insertion_count++;
        }

        else if(c == 'F')
        {
            cin >> key;
            auto start = std::chrono::high_resolution_clock::now();
            av1.search(key,out1);
            auto end = std::chrono::high_resolution_clock::now();
            search_time += (end-start);
            search_count++;
        }
        else if(c == 'T')
        {

            auto start = std::chrono::high_resolution_clock::now();
            av1.traverse(out1);
            auto end = std::chrono::high_resolution_clock::now();
            trav_time += (end-start);
            trav_count++;

        }

        else if(c == 'D')
        {
            cin >> key;
            auto start = std::chrono::high_resolution_clock::now();
            av1.remove(key,out1);
            auto end = std::chrono::high_resolution_clock::now();
            deletion_time += (end-start);
            deletion_count++;
        }

    }

    out2 << "insert " << insertion_time.count() / insertion_count << endl;
    out2 << "delete " << deletion_time.count() / deletion_count << endl;
    out2 << "search " << search_time.count() / search_count << endl;
    out2 << "trav " << trav_time.count() / trav_count << endl;
    out2 << "total " << insertion_time.count() / insertion_count + deletion_time.count() / deletion_count + search_time.count() / search_count + trav_time.count() / trav_count << endl;


}
