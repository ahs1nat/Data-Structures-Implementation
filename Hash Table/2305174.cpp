#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int INITIAL_SIZE = 13;
const double EXPAND_LF = 0.5;
const double SHRINK_LF = 0.25;

bool isPrime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int nextPrime(int n)
{
    while (!isPrime(n))
        n++;
    return n;
}

int prevPrime(int n)
{
    for (int i = n - 1; i >= 2; i--)
    {
        if (isPrime(i))
            return i;
    }
    return INITIAL_SIZE;
}

template <typename K>
string keyToString(const K &k)
{
    stringstream ss;
    ss << k;
    return ss.str();
}

template <typename K>
int Hash1(const K &key, int tableSize)
{
    string s = keyToString(key);
    int hash = 0;
    for (char c : s)
    {
        hash = (hash * 37 + c) % tableSize;
    }
    return hash;
}

template <typename K>
int Hash2(const K &key, int tableSize)
{
    string s = keyToString(key);
    int hash = 0;
    for (char c : s)
    {
        hash = (hash * 53 + c) % tableSize;
    }
    return hash;
}

template <typename K>
int auxHash(const K &key, int tableSize)
{
    string s = keyToString(key);
    int hash = 0;
    for (char c : s)
    {
        hash = (hash * 29 + c) % tableSize;
    }
    return hash==0?1:hash;
}

template <typename K, typename V>
class HashTable
{
protected:
    int M; // table size
    int N; // current number of elements
    int hashChoice;
    int nAtLastResize;
    int insertionSinceResize;
    int deletionSinceResize;
    ll collisions = 0;

    int getPrimaryHash(const K &key)
    {
        return (hashChoice == 1) ? Hash1(key, M) : Hash2(key, M);
    }

public:
    HashTable(int hType) : M(INITIAL_SIZE),
                           N(0),
                           hashChoice(hType),
                           nAtLastResize(0),
                           deletionSinceResize(0),
                           insertionSinceResize(0) {}

    virtual ~HashTable() {}

    virtual void insert(K key, V value) = 0;
    virtual bool search(K key, int &hits) = 0;
    virtual void remove(K key) = 0;
    virtual void rehash(int newM) = 0;

    double getLoadFactor() { return (double)N / M; }

    void checkResizing(bool isInsertion)
    {
        double lf = getLoadFactor();

        if (isInsertion)
        {
            insertionSinceResize++;
            if (lf > EXPAND_LF && insertionSinceResize >= nAtLastResize / 2)
            {
                rehash(nextPrime(2 * M));
                nAtLastResize = N;
                insertionSinceResize = 0;
                deletionSinceResize = 0;
            }
        }
        else
        {
            deletionSinceResize++;
            if (lf < SHRINK_LF && M > INITIAL_SIZE && deletionSinceResize >= nAtLastResize / 2)
            {
                rehash(prevPrime(M / 2));
                nAtLastResize = N;
                insertionSinceResize = 0;
                deletionSinceResize = 0;
            }
        }
    }
    ll getCollisions() const { return collisions; }
};

template <typename K, typename V>
class ChainingTable : public HashTable<K, V>
{
    vector<list<pair<K, V>>> table;

public:
    ChainingTable(int hType) : HashTable<K, V>(hType), table(INITIAL_SIZE) {}

    void insert(K key, V value) override
    {
        int idx = this->getPrimaryHash(key);
        for (auto &p : table[idx])
        {
            if (p.first == key)
                return;
        }

        if (!table[idx].empty())
            this->collisions++;

        table[idx].push_back({key, value});
        this->N++;
        this->checkResizing(true);
    }

    bool search(K key, int &hits) override
    {
        int idx = this->getPrimaryHash(key);
        hits = 0;
        for (auto &p : table[idx])
        {
            hits++;
            if (p.first == key)
                return true;
        }
        return false;
    }

    void remove(K key) override
    {
        int idx = this->getPrimaryHash(key);
        for (auto it = table[idx].begin(); it != table[idx].end(); it++)
        {
            if (it->first == key)
            {
                table[idx].erase(it);
                this->N--;
                this->checkResizing(false);
                return;
            }
        }
    }

    void rehash(int newM) override
    {
        auto oldTable = table;
        this->M = newM;
        table.assign(newM, list<pair<K, V>>());

        int x = this->N;
        this->N = 0;
        for(auto & bucket : oldTable)
        {
            for(auto &p:bucket)
            {
                insert(p.first, p.second);
            }
        }
        this->N = x;
    }

};

// Open Addressing base class
enum Status
{
    EMPTY,
    OCCUPIED,
    DELETED
};

template <typename K, typename V>
class ProbingTable : public HashTable<K, V>
{
protected:
    struct Entry
    {
        K key;
        V value;
        Status status = EMPTY;
    };
    vector<Entry> table;
    virtual int getProbe(const K &key, int i) = 0;

public:
    ProbingTable(int hType) : HashTable<K, V>(hType), table(INITIAL_SIZE) {}

    void insert(K key, V value) override
    {
        for (int i = 0; i < this->M; i++)
        {
            int idx = getProbe(key, i);

            if (table[idx].status == OCCUPIED)
            {
                if(table[idx].key==key)
                    return;
                else
                {
                    this->collisions++;
                }
            }

            if (table[idx].status != OCCUPIED)
            {
                table[idx] = {key, value, OCCUPIED};
                this->N++;
                this->checkResizing(true);
                return;
            }
        }
    }

    bool search(K key, int &hits) override
    {
        hits = 0;
        for (int i = 0; i < this->M; i++)
        {
            hits++;
            int idx = getProbe(key, i);
            if (table[idx].status == EMPTY)
                return false;
            if (table[idx].status == OCCUPIED && table[idx].key == key)
                return true;
        }
        return false;
    }

    void remove(K key) override
    {
        for (int i = 0; i < this->M; i++)
        {
            int idx = getProbe(key, i);
            if (table[idx].status == EMPTY)
                return;
            if (table[idx].status == OCCUPIED && table[idx].key == key)
            {
                table[idx].status = DELETED;
                this->N--;
                this->checkResizing(false);
                return;
            }
        }
    }

    void rehash(int newM) override
    {
        auto oldTable = table;
        this->M = newM;
        table.assign(newM, Entry());

        int x = this->N;
        this->N = 0;
        for (auto &e : oldTable)
        {
            if (e.status == OCCUPIED)
                insert(e.key, e.value);
        }
        this->N = x;
    }
};

//Double hashing
template <typename K, typename V>
class DoubleHash : public ProbingTable<K, V>
{
    int getProbe(const K &key, int i) override
    {
        int h1 = this->getPrimaryHash(key);
        int h2 = auxHash(key, this->M);
        return (h1 + (ll)i * h2) % this->M;
    }

public:
    DoubleHash(int hType) : ProbingTable<K, V>(hType) {}
};

//Custom Probing
template <typename K, typename V>
class CustomHash : public ProbingTable<K, V>
{
    const int C1 = 7, C2 = 11;
    int getProbe(const K &key, int i) override
    {
        int h1 = this->getPrimaryHash(key);
        int h2 = auxHash(key, this->M);
        return (h1 + (ll)C1 * i * h2 + (ll)C2 * i * i) % this->M;
    }

public:
    CustomHash(int hType) : ProbingTable<K, V>(hType) {}
};

int main()
{
    srand(time(0));

    vector<string> words;
    unordered_set<string> uniqueChecker;
    string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    while (words.size() < 10000)
    {
        string w = "";
        for (int i = 0; i < 10; i++)
            w += alphabet[rand() % alphabet.length()];
        if (uniqueChecker.find(w) == uniqueChecker.end())
        {
            uniqueChecker.insert(w);
            words.push_back(w);
        }
    }

    cout << fixed << setprecision(4);
    cout << "\n"
         << setfill('=') << setw(95) << "" << setfill(' ') << endl;
    cout << left << setw(20) << "Method"
         << setw(35) << "Hash 1"
         << setw(35) << "Hash 2" << endl;
    cout << left << setw(20) << ""
         << setw(18) << "Collisions" << setw(17) << "Avg Hits"
         << setw(18) << "Collisions" << setw(17) << "Avg Hits" << endl;
    cout << setfill('-') << setw(95) << "" << setfill(' ') << endl;


    string methods[] = {"Chaining Method", "Double Hashing", "Custom Probing"};

    for (string method : methods)
    {
        cout << left << setw(20) << method;

        for (int hType = 1; hType <= 2; hType++)
        {
            HashTable<string, int> *table = nullptr;
            if (method == "Chaining Method")
            {
                table = new ChainingTable<string, int>(hType);
            }
            else if (method == "Double Hashing")
            {
                table = new DoubleHash<string, int>(hType);
            }
            else if (method == "Custom Probing")
            {
                table = new CustomHash<string, int>(hType);
            }
            long long collisions = table->getCollisions();

            for (int i = 0; i < 10000; i++)
            {
                table->insert(words[i], i);
            }

            ll totalHits = 0;
            for (int i = 0; i < 1000; i++)
            {
                int hits = 0;
                string target = words[rand() % 10000];
                table->search(target, hits);
                totalHits += hits;
            }

            double avgHits = (double)totalHits / 1000.0;
            cout << left << setw(18) << table->getCollisions() << setw(17) << avgHits;

            delete table;
        }
        cout << endl;
    }

    cout << setfill('=') << setw(95) << "" << setfill(' ') << endl;

    return 0;
}