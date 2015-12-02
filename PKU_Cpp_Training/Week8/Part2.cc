#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>

using namespace std;


#define SIZE_BLAH       (512)
#define CMD_NEW         "new"
#define CMD_ADD         "add"
#define CMD_MERGE       "merge"
#define CMD_UNIQUE      "unique"
#define CMD_OUT         "out"


// I represents as index and N represents as numerics.
template <typename I, typename N>
class List
{
  private:
    map<I, map<N, N> > array_list_;

  public:
    List()
      : array_list_()
    {}

    ~List()
    {}

    void New(I);
    void Add(I, N);
    void Merge(I, I);
    void Unique(I);
    void Output(I);
    //friend ostream& operator<<<> (ostream&, const List&);
};


template <typename I, typename N>
void List<I, N>::New(I idx)
{
    this->array_list_.insert(pair<I, map<N, N> >(idx, map<N, N>()));
}

template <typename I, typename N>
void List<I, N>::Add(I idx, N num)
{
    map<N, N>& list = this->array_list_.find(idx)->second;
    list.insert(pair<N, N>(num, 1));
}

template <typename I, typename N>
void List<I, N>::Merge(I id_src, I id_tge)
{
    if (id_src == id_tge)
        return;

    map<N, N>& list_src = this->array_list_.find(id_src)->second;
    map<N, N>& list_tge = this->array_list_.find(id_tge)->second;

    typename map<N, N>::iterator iter_tge = list_tge.begin();
    while (iter_tge != list_tge.end()) {
        N num = iter_tge->first;
        N freq = iter_tge->second;

        typename map<N, N>::iterator iter_src = list_src.find(num);
        if (iter_src != list_src.end())
            iter_src->second += freq;
        else
            list_src.insert(pair<N, N>(num, freq));

        ++iter_tge;
    }

    list_tge.clear();
}

template <typename I, typename N>
void List<I, N>::Unique(I idx)
{
    map<N, N>& list = this->array_list_.find(idx)->second;
    typename map<N, N>::iterator iter = list.begin();
    while (iter != list.end()) {
        iter->second = 1;
        ++iter;
    }
}

template <typename I, typename N>
void List<I, N>::Output(I idx)
{
    map<N, N>& list = this->array_list_.find(idx)->second;
    typename map<N, N>::const_iterator iter = list.begin();
    stringstream stream;

    while (iter != list.end()) {
        N num = iter->first;
        N freq = iter->second;
        for (N i = 0 ; i < freq ; ++i)
            stream << num << ' ';
        ++iter;
    }

    cout << stream.str() << endl;
}


int main()
{
    int loop;
    string str_in;
    char buf[SIZE_BLAH];
    List<int, int> list_obj;

    cin >> loop;
    getline(cin, str_in);

    for (int i = 0 ; i < loop ; ++i) {
        string str_in;
        getline(cin, str_in);

        int len_str = str_in.length();
        strncpy(buf, str_in.c_str(), len_str);
        buf[len_str] = 0;
        char* token = strtok(buf, " ");

        if (strcmp(token, CMD_NEW) == 0) {
            token = strtok(NULL, " ");
            int id_list = atoi(token);
            list_obj.New(id_list);
            continue;
        }
        if (strcmp(token, CMD_ADD) == 0) {
            token = strtok(NULL, " ");
            int id_list = atoi(token);
            token = strtok(NULL, " ");
            int num = atoi(token);
            list_obj.Add(id_list, num);
            continue;
        }
        if (strcmp(token, CMD_MERGE) == 0) {
            token = strtok(NULL, " ");
            int id_fst = atoi(token);
            token = strtok(NULL, " ");
            int id_snd = atoi(token);
            list_obj.Merge(id_fst, id_snd);
            continue;
        }
        if (strcmp(token, CMD_UNIQUE) == 0) {
            token = strtok(NULL, " ");
            int id_list = atoi(token);
            list_obj.Unique(id_list);
            continue;
        }
        if (strcmp(token, CMD_OUT) == 0) {
            token = strtok(NULL, " ");
            int id_list = atoi(token);
            list_obj.Output(id_list);
        }
    }

    return 0;
}