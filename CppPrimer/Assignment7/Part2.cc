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


void handle_new(map<int, map<int, int> >* p_map_all, int id_map)
{
    p_map_all->insert(pair<int, map<int, int> >(id_map, map<int, int>()));
}

void handle_add(map<int, map<int, int> >* p_map_all, int id_map, int num)
{
    map<int, int>& map_sge = p_map_all->find(id_map)->second;
    map_sge.insert(pair<int, int>(num, 1));
}

void handle_merge(map<int, map<int, int> >* p_map_all, int id_fst, int id_snd)
{
    if (id_fst == id_snd)
        return;

    map<int, int>& map_fst = p_map_all->find(id_fst)->second;
    map<int, int>& map_snd = p_map_all->find(id_snd)->second;

    map<int, int>::iterator iter_snd = map_snd.begin();
    while (iter_snd != map_snd.end()) {
        int num = iter_snd->first;
        int count = iter_snd->second;
        map<int, int>::iterator iter_fst = map_fst.find(num);
        if (iter_fst != map_fst.end())
            iter_fst->second += count;
        else
            map_fst.insert(pair<int, int>(num, count));
        ++iter_snd;
    }

    map_snd.clear();
}

void handle_unique(map<int, map<int, int> >* p_map_all, int id_map)
{
    map<int, int>& map_sge = p_map_all->find(id_map)->second;
    map<int, int>::iterator iter = map_sge.begin();
    while (iter != map_sge.end()) {
        iter->second = 1;
        ++iter;
    }
}

void handle_out(const map<int, map<int, int> >& map_all, int id_map)
{
    const map<int, int>& map_sge = map_all.find(id_map)->second;
    map<int, int>::const_iterator iter = map_sge.begin();
    stringstream stream;

    while (iter != map_sge.end()) {
        int num = iter->first;
        int count = iter->second;
        for (int i = 0 ; i < count ; ++i)
            stream << num << ' ';
        ++iter;
    }

    string str = stream.str();
    cout << str << endl;
}


int main()
{
    int loop;
    string str_in;
    char buf[SIZE_BLAH];
    map<int, map<int, int> > map_all;

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
            int id_map = atoi(token);
            handle_new(&map_all, id_map);
            continue;
        }
        if (strcmp(token, CMD_ADD) == 0) {
            token = strtok(NULL, " ");
            int id_map = atoi(token);
            token = strtok(NULL, " ");
            int num = atoi(token);
            handle_add(&map_all, id_map, num);
            continue;
        }
        if (strcmp(token, CMD_MERGE) == 0) {
            token = strtok(NULL, " ");
            int id_fst = atoi(token);
            token = strtok(NULL, " ");
            int id_snd = atoi(token);
            handle_merge(&map_all, id_fst, id_snd);
            continue;
        }
        if (strcmp(token, CMD_UNIQUE) == 0) {
            token = strtok(NULL, " ");
            int id_map = atoi(token);
            handle_unique(&map_all, id_map);
            continue;
        }
        if (strcmp(token, CMD_OUT) == 0) {
            token = strtok(NULL, " ");
            int id_map = atoi(token);
            handle_out(map_all, id_map);
        }
    }

    map<int, map<int, int> >::iterator iter = map_all.begin();
    while (iter != map_all.end()) {
        iter->second.clear();
        ++iter;
    }
    map_all.clear();

    return 0;
}