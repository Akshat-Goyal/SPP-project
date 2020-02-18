#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "kvStore.cpp"
using namespace std;

char *random_key(int stringLength)
{
    char *key = (char *)malloc(sizeof(char) * (stringLength + 1));
    string letters = "";
    for (char i = 'a'; i <= 'z'; i++)
        letters += i;
    for (char i = 'A'; i <= 'Z'; i++)
        letters += i;
    for (int i = 0; i < stringLength; i++)
        key[i] = letters[rand() % 52];
    key[stringLength] = '\0';
    return key;
}

char *random_value(int stringLength)
{
    char *value = (char *)malloc(sizeof(char) * (stringLength + 1));
    string letters = "";
    for (char i = 'a'; i <= 'z'; i++)
        letters += i;
    for (char i = 'A'; i <= 'Z'; i++)
        letters += i;
    for (int i = 0; i < stringLength; i++)
        value[i] = letters[rand() % 52];
    value[stringLength] = '\0';
    return value;
}
long CLOCKS_PER_SECOND = 1000000;
kvstore kv;
map<char *, char *> db;
int db_size = 0;
int num = 0;

// void *myThreadFun(void *vargp) {
//     int transactions = 0;
//     clock_t start = clock();
//     int time = 10;
//     clock_t tt = clock();
//     while ((float(tt - start) / CLOCKS_PER_SECOND) <= time) {
//         for (int i = 0; i < 10000; i++) {
//             transactions += 1;
//             int x = rand() % 5;
//             if (x == 0) {
//                 string k = random_key(10);
//                 bool ans = kv.get(k);
//             } else if (x == 1) {
//                 int k = rand() % 64 + 1;
//                 int v = rand() % 256 + 1;
//                 string key = random_key(k);
//                 string value = random_value(v);
//                 bool ans = kv.put(key, value);
//                 db_size++;
//             } else if (x == 2) {
//                 int temp = db_size;
//                 if (temp == 0)
//                     continue;
//                 int rem = rand() % temp;
//                 pair<string, string> check = kv.get(rem);
//                 bool check2 = kv.del(check.first);
//                 db_size--;
//             } else if (x == 3) {
//                 int temp = db_size;
//                 if (temp == 0)
//                     continue;
//                 int rem = rand() % temp;
//                 pair<string, string> check = kv.get(rem);
//             } else if (x == 4) {
//                 int temp = db_size;
//                 if (temp == 0)
//                     continue;
//                 int rem = rand() % temp;
//                 bool check = kv.del(rem);
//                 db_size--;
//             }
//         }
//         tt = clock();
//     }
//     cout << transactions / time << endl;
//     return NULL;
// }

string cs(char *s)
{
    string k;
    int n = strlen(s);
    for (int i = 0; i < n; i++)
        k += s[i];
    return k;
}

Slice ms(char *s)
{
    Slice k;
    k.size = strlen(s);
    k.data = s;
    // k.data = (char *)malloc((k.size + 1) * 8);
    // strcpy(k.data, s);
    return k;
}

map<string, char *> mp;

int main()
{
    for (int i = 0; i < 1000; i++)
    {
        int k = rand() % 10 + 1;
        int v = rand() % 256 + 1;
        // cout << k << " " << v << "\n";
        char *key = random_key(k);
        char *value = random_value(v);
        // cout << key << "\n";
        // cout << value << "\n";
        // cout << key << " " << strlen(key) << "\n";
        // cout << value << " " << strlen(value) << "\n";
        // cout << key << "\n";
        int fl = 1;
        for (auto it : db)
        {
            if (strcmp(it.first, key) == 0)
            {
                fl = 0;
                break;
            }
        }
        if (!fl)
            continue;
        db.insert(make_pair(key, value));
        mp.insert(make_pair(cs(key), key));
        Slice kk = ms(key);
        Slice vv = ms(value);
        if (!kv.put(kk, vv))
        {
            cout << "Error in Put\n";
            return 0;
        }
        db_size++;
    }
    int incorrect = 0;
    cout << db_size << "\n";
    int cnt = 1;
    Slice key, value;
    cout << kv.del(db_size) << "\n";
    cout << kv.get(db_size, key, value) << "\n";

    // for (auto it : mp)
    // {
    //     Slice key, value;
    //     if (!kv.get(cnt, key, value))
    //     {
    //         cout << "error in getN\n";
    //         // cout << cnt << "\n";
    //         // return 0;
    //     }
    //     if (strcmp(it.second, key.data) || strcmp(db[it.second], value.data))
    //     {
    //         cout << "Wrong \n";
    //         cout << it.second << "\n";
    //         cout << key.data << "\n";
    //         cout << db[it.second] << "\n";
    //         cout << value.data << "\n";
    //         cout << cnt << "\n";
    //         // return 0;
    //     }
    //     cnt++;
    // }
    // cout << cnt << "\n";
    cout << "Correct\n";
    // for (int i = 0; i < 10; i++) {
    //     // int x = rand() % 5;
    //     // if (x == 0) {
    //     auto itr = db.begin();
    //     string key = *(itr.first);
    //     // map<string, string>::iterator itr = db.find(k);
    //     if ((ans == false && itr != db.end()) ||
    //         (ans == true && itr == db.end()))
    //         incorrect = true;
    //     //     } else if (x == 1) {
    //         int k = rand() % 64 + 1;
    //         int v = rand() % 256 + 1;
    //         string key = random_key(k);
    //         string value = random_value(v);
    //         db.insert(pair<string, string>(key, value));
    //         bool check1 = kv.get(key);
    //         bool ans = kv.put(key, value);
    //         bool check2 = kv.get(key);
    //         db_size++;
    //         if (check2 == false || check1 != ans)
    //             incorrect = true;
    //     } else if (x == 2) {
    //         int max_size = db.size();
    //         int rem = rand() % max_size;
    //         map<string, string>::iterator itr = db.begin();
    //         for (int i = 0; i < rem; i++)
    //             itr++;
    //         string key = itr->first;
    //         bool check = kv.del(key);
    //         db_size--;
    //         db.erase(itr);
    //         bool check2 = kv.get(key);
    //         if (check2 == true)
    //             incorrect = true;
    //     } else if (x == 3) {
    //         int max_size = db.size();
    //         int rem = rand() % max_size;
    //         pair<string, string> check = kv.get(rem);
    //         map<string, string>::iterator itr = db.begin();
    //         for (int i = 0; i < rem; i++)
    //             itr++;
    //         if (check.first != itr->first || check.second != itr->second)
    //             incorrect = true;
    //     } else if (x == 4) {
    //         int max_size = db.size();
    //         int rem = rand() % max_size;
    //         map<string, string>::iterator itr = db.begin();
    //         for (int i = 0; i < rem; i++)
    //             itr++;
    //         string key = itr->first;
    //         bool check = kv.del(rem);
    //         db.erase(itr);
    //         db_size--;
    //         bool check2 = kv.get(key);
    //         if (check2 == true)
    //             incorrect = true;
    //     }
    // }
    // if (incorrect == true) {
    //     cout << 0 << endl;
    //     return 0;
    // }
    // int threads = 4;

    // pthread_t tid[threads];
    // for (int i = 0; i < threads; i++) {
    //     tid[i] = i;
    //     pthread_create(&tid[i], NULL, myThreadFun, (void *)&tid[i]);
    // }
    // for (int i = 0; i < threads; i++)
    //     pthread_join(tid[i], NULL);
    return 0;
}