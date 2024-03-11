#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
vector<string> generatedStrings;

class node {
public:
  ll value;
  string key;
  node *prev, *next;

  node() {
    key = "";
    value = LONG_LONG_MIN;
    prev = NULL;
    next = NULL;
  }

  node(string str, ll val) {
    key = str;
    value = val;
    prev = NULL;
    next = NULL;
  }
};

class dnode {
public:
  string key;
  ll value;
  bool isFree;
  bool wasDeleted;

  dnode() {
    this->key = "";
    this->value = LONG_LONG_MIN;
    isFree = true;
    wasDeleted = false;
  }

  dnode(string key, ll value) {
    this->key = key;
    this->value = value;
    isFree = true;
    wasDeleted = false;
  }
};

bool isPrime(ll num) {
  for (ll i = 2; i <= num / 2; i++) {
    if (num % i == 0)
      return false;
    else
      continue;
  }

  return true;
}

ll findClosestPrime(ll num) {
  if (isPrime(num)) {
    return num;
  }
  ll upper = num + 1;
  while (true) {
    if (isPrime(upper)) {
      return upper;
    }
    upper++;
  }
}

void generateString(ll num) {
  int l = 5, u = 10;

  generatedStrings.clear();

  map<string, ll> mapping;

  for (ll i = 0; i < num; i++) {
    string str = "";
    int size = (rand() % (u - l + 1)) + l;
    for (ll it = 0; it < size; it++)
      str += ('a' + rand() % 26);

    if (mapping[str] == 0) {
      generatedStrings.push_back(str);
      mapping[str]++;
    }
  }
}

ll Hash1(string str, ll num) {
  const ll div = 1e9 + 7;
  int flag = 37, i = 1;
  ll h_val = 0;

  for (char c : str) {
    h_val = (h_val + (c - 'a' + 1) * i) % div;
    i = (i * flag) % div;
  }
  return (h_val % num + num) % num;
}

ll Hash2(string str, ll num) {
  ll h_val = 0;

  for (int i = 0; i < str.length(); i++)
    h_val = 31 * h_val + str[i];

  h_val = h_val % num;
  if (h_val < 0)
    h_val += num;

  return h_val;
}

double CheckHash1(vector<string> &arr, ll num) {
  ll flag = 0;
  set<ll> st;
  for (ll i = 0; i < arr.size(); i++) {
    st.insert(Hash1(arr[i], num));
    flag++;
  }
  return (double)st.size() / (double)flag;
}

double CheckHash2(vector<string> &arr, ll num) {
  ll flag = 0;
  set<ll> st;
  for (ll i = 0; i < arr.size(); i++) {
    st.insert(Hash2(arr[i], num));
    flag++;
  }
  return (double)st.size() / (double)flag;
}

void check(ll num) {
  generateString(100);
  cout << "Hash1: hash values are " << CheckHash1(generatedStrings, num) * 100
       << "\% unique." << endl;
  cout << "Hash2: hash values are " << CheckHash2(generatedStrings, num) * 100
       << "\% unique." << endl;
}

ll auxHash(string &str) {
  ll hashValue = Hash1(str, 71);
  return 71 - (hashValue % 71);
}

class chainHash {
public:
  ll size;
  node *head;
  int func;

  chainHash() {}

  chainHash(ll n, int f) {
    size = n;
    head = new node[n];
    func = f;
  }

  void reset() {
    delete[] head;
    size = 0;
  }

  ll insert(string str, ll val) {
    ll hs = -1;
    if (func == 1)
      hs = Hash1(str, size) % size;
    else if (func == 2)
      hs = Hash2(str, size) % size;

    node *tmp = new node(str, val);
    node *tmp2 = head + hs;
    ll collisionCount = 0;

    if (tmp2->next != NULL) {
      tmp->prev = tmp2;
      tmp->next = tmp2->next;
      tmp2->next->prev = tmp;
      tmp2->next = tmp;
      collisionCount = 1;
    }

    if (tmp2->next == NULL) {

      tmp->prev = tmp2;
      tmp2->next = tmp;
      collisionCount = 0;
    }

    return collisionCount;
  }

  ll remove(string str) {
    ll v = 0;
    ll hs = -1;
    if (func == 1)
      hs = Hash1(str, size) % size;
    else if (func == 2)
      hs = Hash2(str, size) % size;
    node *it = head + hs;
    while (it->next != NULL) {
      it = it->next;
      if (it->key == str) {
        v = it->value;
        it->prev->next = it->next;
        if (it->next != NULL)
          it->next->prev = it->prev;
        delete it;
        break;
      }
    }
    return v;
  }

  ll find(string str) {
    ll v = 0;
    ll hs = -1;
    if (func == 1)
      hs = Hash1(str, size) % size;
    else if (func == 2)
      hs = Hash2(str, size) % size;
    node *it = head + hs;
    ll flag = 0;
    while (it->next != NULL) {
      flag++;
      it = it->next;
      if (it->key == str) {
        v = it->value;
        break;
      }
    }
    return flag;
  }
};

class doubleHash {
public:
  ll size;
  dnode *head;
  int func;

  doubleHash() {}

  doubleHash(ll s, int f) {
    size = s;
    head = new dnode[s];
    func = f;
  }

  void clear() {
    size = 0;
    delete[] head;
  }

  ll insert(string str, ll val) {
    ll value = -1;
    ll hs1 = Hash1(str, size);
    ll hs2 = Hash2(str, size);
    ll aux_hs1 = auxHash(str);

    ll flag = 0, i = 0;
    ll it = -1;
    if (func == 1)
      it = (hs1 + i * aux_hs1) % size;
    else if (func == 2)
      it = (hs2 + i * aux_hs1) % size;
    while (flag < size) {
      if ((head + it)->isFree) {
        value = val;
        (head + it)->key = str;
        (head + it)->value = val;
        (head + it)->isFree = false;
        (head + it)->wasDeleted = false;
        break;
      }

      i = (i + aux_hs1) % size;
      it = (it + i) % size;
      flag++;
    }

    if (flag > 0 && flag != size)
      return 1;
    else
      return 0;
  }

  ll remove(string str) {
    ll value = -1;
    ll hs1 = Hash1(str, size);
    ll hs2 = Hash2(str, size);
    ll aux_hs1 = auxHash(str);

    ll flag = 0, i = 0;
    ll it = -1;
    if (func == 1)
      it = (hs1 + i * aux_hs1) % size;
    else if (func == 2)
      it = (hs2 + i * aux_hs1) % size;
    while (flag < size) {

      if ((head + it)->isFree == false && (head + it)->key == str) {
        value = (head + it)->value;
        (head + it)->isFree = true;
        (head + it)->wasDeleted = true;
        break;
      }

      i = (i + aux_hs1) % size;
      it = (it + i) % size;
      flag++;
    }
    return value;
  }

  ll search(string str) {
    ll value = -1;
    ll hs1 = Hash1(str, size);
    ll hs2 = Hash2(str, size);
    ll aux_hs1 = auxHash(str);

    ll flag = 1, i = 0;
    ll it = -1;
    if (func == 1)
      it = (hs1 + i * aux_hs1) % size;
    else if (func == 2)
      it = (hs2 + i * aux_hs1) % size;

    while (flag < size) {

      if ((head + it)->isFree == false && (head + it)->key == str) {
        value = (head + it)->value;
        break;
      }

      i = (i + aux_hs1) % size;

      it = (it + i) % size;
      flag++;
    }

    if (flag == size)
      return 0;
    else
      return flag;
  }
};

/* Custom hashing
CustomHash(k,i) = (Hash(k) + C1 * i * auxHash(k) + C2 * i^2) % N
*/

class customHash {
public:
  ll size;
  dnode *head;
  int func;

  customHash() {}

  customHash(ll s, int func) {
    size = s;
    head = new dnode[s];
    this->func = func;
  }

  void clear() {
    size = 0;
    delete[] head;
  }

  ll insert(string str, ll val) {
    ll value = -1;
    ll hs1 = Hash1(str, size);
    ll hs2 = Hash2(str, size);
    ll aux_hs1 = auxHash(str);

    ll flag = 0, i = 1;
    ll it = -1;
    if (func == 1)
      it = (hs1 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    else if (func == 2)
      it = (hs2 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    while (flag < size) {
      if ((head + it)->isFree) {
        value = val;
        (head + it)->key = str;
        (head + it)->value = val;
        (head + it)->isFree = false;
        (head + it)->wasDeleted = false;
        break;
      }

      i = (i + aux_hs1) % size;
      it = (it + i) % size;
      flag++;
    }

    if (flag > 0)
      return 1;
    else
      return 0;
  }

  ll remove(string str) {
    ll value = -1;
    ll hs1 = Hash1(str, size) % size;
    ll hs2 = Hash2(str, size) % size;
    ll aux_hs1 = auxHash(str);

    ll flag = 0, i = 1;
    ll it = -1;
    if (func == 1)
      it = (hs1 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    else if (func == 2)
      it = (hs2 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    while (flag < size) {

      if ((head + it)->isFree == false && (head + it)->key == str) {
        value = (head + it)->value;
        (head + it)->isFree = true;
        (head + it)->wasDeleted = true;
        break;
      }
      if (func == 1)
        i = (i + aux_hs1) % size;
      else if (func == 2)
        i = (i + aux_hs1) % size;
      it = (it + i) % size;
      flag++;
    }
    return value;
  }

  ll search(string str) {
    ll value = -1;
    ll hs1 = Hash1(str, size) % size;
    ll hs2 = Hash2(str, size) % size;
    ll aux_hs1 = auxHash(str);

    ll flag = 1, i = 1;
    ll it = -1;
    if (func == 1)
      it = (hs1 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    else if (func == 2)
      it = (hs2 + 3 * i * aux_hs1 + 5 * (i * i)) % size;
    while (flag < size) {

      if ((head + it)->isFree == false && (head + it)->key == str) {
        value = (head + it)->value;
        break;
      }

      i = (i + aux_hs1) % size;
      it = (it + i) % size;
      flag++;
    }
    if (flag == size)
      return 0;
    else
      return flag;
  }
};

int main() {

  srand(89);
  cout << "Please input your table size: ";
  int n;
  cin >> n;
  ll sz = findClosestPrime(n);

  cout << "--------------------------------------------------------------------"
          "------"
       << endl;
  cout << "Checking the hash function for 100 words and a hash table with size "
          "of 203."
       << endl;
  cout << "--------------------------------------------------------------------"
          "------"
       << endl
       << endl;

  check(203);

  cout << "--------------------------------------------------------------------"
          "------"
       << endl
       << endl;

  // check(n);

  // for (int i = 0; i < generatedStrings.size(); i++)
  //   cout << generatedStrings[i] << endl;
  //   ll in;
  //   cin >> in;
  //   in = findClosestPrime(in);
  //   n = in;
  //   cout << "n = " << n << endl;
  //   arr.resize(n);
  //   cout << "size = " << arr.size() << endl;

  // generateString(10000);

  ll noData = 10000;
  if (sz < 10000)
    noData = 5000;
  generateString(noData);

  double chainHashTotalProbs1 = 0, doubleHashTotalProbs1 = 0,
         customHashTotalProbs1 = 0, chainHashTotalProbs2 = 0,
         doubleHashTotalProbs2 = 0, customHashTotalProbs2 = 0;

  ll chainHashTotalCollisions1 = 0, doubleHashTotalCollisions1 = 0,
     customHashTotalCollisions1 = 0, chainHashTotalCollisions2 = 0,
     doubleHashTotalCollisions2 = 0, customHashTotalCollisions2 = 0;

  chainHash chainTable1(sz, 1), chainTable2(sz, 2);
  doubleHash doubleTable1(sz, 1), doubleTable2(sz, 2);
  customHash customTable1(sz, 1), customTable2(sz, 2);

  // cout << "GENERATED STRING SIZE: " << generatedStrings.size() << endl;

  ll i = 0;
  ll sz2 = 10000;

  for (string str : generatedStrings) {
    chainHashTotalCollisions1 += chainTable1.insert(str, i + 1);
    doubleHashTotalCollisions1 += doubleTable1.insert(str, i + 1);
    customHashTotalCollisions1 += customTable1.insert(str, i + 1);
    chainHashTotalCollisions2 += chainTable2.insert(str, i + 1);
    doubleHashTotalCollisions2 += doubleTable2.insert(str, i + 1);
    customHashTotalCollisions2 += customTable2.insert(str, i + 1);
    i++;
  }

  for (int i = 0; i < sz2; i++) {
    ll random = rand() % generatedStrings.size();

    chainHashTotalProbs1 += chainTable1.find(generatedStrings[random]);

    chainHashTotalProbs2 += chainTable2.find(generatedStrings[random]);

    doubleHashTotalProbs1 += doubleTable1.search(generatedStrings[random]);

    doubleHashTotalProbs2 += doubleTable2.search(generatedStrings[random]);

    customHashTotalProbs1 += customTable1.search(generatedStrings[random]);

    customHashTotalProbs2 += customTable2.search(generatedStrings[random]);
  }

  cout << "For N'= " << n << endl << endl;
  cout << "Hash Table Size = " << sz << endl << endl;
  cout << "Method: Separate chaining, Hash1" << endl;
  cout << "Collision Count: " << chainHashTotalCollisions1 << endl;
  cout << "Average probes: " << double(chainHashTotalProbs1 / sz2) << endl
       << endl;

  cout << "Method: Separate chaining, Hash2" << endl;
  cout << "Collision Count: " << chainHashTotalCollisions2 << endl;
  cout << "Average probes: " << double(chainHashTotalProbs2 / sz2) << endl
       << endl;

  cout << "Method: Double Hashing, Hash1" << endl;
  cout << "Collision Count: " << doubleHashTotalCollisions1 << endl;
  cout << "Average probes: " << double(doubleHashTotalProbs1 / sz2) << endl
       << endl;

  cout << "Method: Double Hashing, Hash2" << endl;
  cout << "Collision Count: " << doubleHashTotalCollisions2 << endl;
  cout << "Average probes: " << double(doubleHashTotalProbs2 / sz2) << endl
       << endl;

  cout << "Method: Custom Hashing, Hash1" << endl;
  cout << "Collision Count: " << customHashTotalCollisions1 << endl;
  cout << "Average probes: " << double(customHashTotalProbs1 / sz2) << endl
       << endl;

  cout << "Method: Custom Hashing, Hash2" << endl;
  cout << "Collision Count: " << customHashTotalCollisions2 << endl;
  cout << "Average probes: " << double(customHashTotalProbs2 / sz2) << endl
       << endl;
}
