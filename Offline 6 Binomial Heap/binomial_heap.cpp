#include <bits/stdc++.h>
using namespace std;

class Node {
public:
  int key, degree;
  Node *parent, *child, *sibling;

  Node() {
    key = -1;
    parent = NULL;
    child = NULL;
    sibling = NULL;
    degree = 0;
  }

  Node(int key) {
    this->key = key;
    parent = NULL;
    child = NULL;
    sibling = NULL;
    degree = 0;
  }
};

class BinomialHeap {
public:
  Node *head;
  BinomialHeap() { head = NULL; }
  BinomialHeap(int x) { head = new Node(x); }
  BinomialHeap(Node *node) { head = node; }

  void joinNodes(Node *first, Node *second) {
    /*         second
          /   /
      first  xyz
  */
    first->parent = second;
    first->sibling = second->child;
    second->child = first;
    second->degree++;
  }

  void pushIntoTree(Node *node, vector<int> *vals, int l) {
    vals[l].push_back(node->key);
    if (node->child != NULL)
      pushIntoTree(node->child, vals, l + 1);
    if (node->sibling != NULL)
      pushIntoTree(node->sibling, vals, l);
  }

  Node *heapMerge(BinomialHeap *heap1, BinomialHeap *heap2) {
    Node *prev = NULL;
    Node *mergedHead = NULL;
    Node *x = heap1->head;
    Node *y = heap2->head;

    if (x->degree > y->degree) {
      mergedHead = y;
      y = y->sibling;

    } else {
      mergedHead = x;
      x = x->sibling;
    }

    prev = mergedHead;
    while (x != NULL || y != NULL) {
      /* B0--B0--B1--B2--B3--B3*/
      if (y == NULL || (x != NULL && x->degree <= y->degree)) {
        prev->sibling = x;
        prev = x;
        x = x->sibling;
      } else if (x == NULL || (y != NULL && x->degree > y->degree)) {
        prev->sibling = y;
        prev = y;
        y = y->sibling;
      }
    }
    return mergedHead;
  }

  void heapUnion(BinomialHeap *passedHeap) {
    if (passedHeap->head == NULL) {
      return;
    }
    if (head == NULL) {
      head = passedHeap->head;
      return;
    }

    head = heapMerge(this, passedHeap);
    Node *cur = head, *prev = NULL, *next = head->sibling;
    while (next != NULL) {
      if ((cur->degree != next->degree) ||
          (next->sibling != NULL && next->sibling->degree == cur->degree)) {
        /* B0--B1 OR B2--B2--B2, just right shifting*/
        prev = cur;
        cur = next;
      } else {
        /* B1(3) -- B1(2)*/
        if (cur->key > next->key) {
          if (prev == NULL)
            head = next; /* the smallest key will be the head.*/
          else
            prev->sibling = next;

          joinNodes(cur, next);
          cur = next;

        } else
          cur->sibling = next->sibling, joinNodes(next, cur);
      }

      next = cur->sibling;
    }
  }

  void insert(int num) {
    BinomialHeap *tmp = new BinomialHeap(num);
    /* B4 ---- B0*/
    heapUnion(tmp);
  }

  Node *extractMin() {
    int min = INT_MAX;
    Node *x = NULL, *x_prev = NULL;
    Node *min_node = NULL, *min_node_prev = NULL;

    for (x = head; x != NULL; x = x->sibling) {
      if (x->key < min) {
        min = x->key;
        min_node = x;
        min_node_prev = x_prev;
      }
      x_prev = x;
    }

    if (min_node_prev == NULL)
      head = min_node->sibling;
    else
      min_node_prev->sibling = min_node->sibling;

    Node *curr = min_node->child;
    Node *prev = NULL;
    Node *next = NULL;

    while (curr != NULL) {
      next = curr->sibling;
      curr->sibling = prev;
      prev = curr;
      curr = next;
    }

    BinomialHeap *tmp = new BinomialHeap(prev);
    heapUnion(tmp);
    return min_node;
  }

  int findMin() {
    int min = INT_MAX;
    Node *tmp;
    Node *x = head;

    if (head == NULL)
      return -1;

    for (x; x != NULL; x = x->sibling) {
      if (x->key < min)
        min = x->key;
    }

    return min;
  }

  void printHeap() {
    cout << "Printing Binomial Heap..." << endl;
    Node *x = head;

    cout << "Head : " << head->key << endl;

    for (x; x != NULL; x = x->sibling) {
      vector<int> *vals;
      vals = new vector<int>[x->degree];
      cout << "Binomial Tree, B" << x->degree << endl;
      if (x->child != NULL)
        pushIntoTree(x->child, vals, 0);
      cout << "Level 0 : " << x->key << " " << endl;
      for (int i = 0; i < x->degree; i++) {
        cout << "Level " << i + 1 << " : ";
        for (int j = 0; j < vals[i].size(); j++)
          cout << vals[i][j] << " ";
        cout << endl;
      }
    }
  }
};

int main() {

  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  BinomialHeap *bh = new BinomialHeap();
  string input;
  while (getline(cin, input)) {
    char c;
    {
      istringstream ins(input);
      ins >> c;

      if (c == 'P') {
        bh->printHeap();
      } else if (c == 'F') {
        int x = bh->findMin();
        cout << "Find-Min returned " << x << endl;

      } else if (c == 'E') {
        Node *tmp = bh->extractMin();
        cout << "Extract-Min returned " << tmp->key << endl;
      } else if (c == 'I') {
        int x;
        ins >> x;
        bh->insert(x);
      } else if (c == 'U') {
        int n;
        vector<int> union_vals;
        while (ins >> n) {
          union_vals.push_back(n);
        }

        BinomialHeap *heap = new BinomialHeap();
        for (int num : union_vals) {
          heap->insert(num);
        }
        bh->heapUnion(heap);
      }
    }
  }
}
