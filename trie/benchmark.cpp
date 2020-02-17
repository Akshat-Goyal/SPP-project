#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "kvStore.cpp"
using namespace std;
#define BILLION (int)1E9

// struct Slice {
//     int size;
//     char *data;
// };

string random_key(int stringLength, string &key) {
    // string key = "";
    key = "";
    string letters = "";

    for (char i = 'a'; i <= 'z'; i++)
        letters += i;
    for (char i = 'A'; i <= 'Z'; i++)
        letters += i;

    for (int i = 0; i < stringLength; i++) {
        int x = rand() % letters.size();
        key += letters[x];
    }
    cout << key << endl;
    return key;
}

string random_value(int stringLength) {
    string value = "";
    string letters = "";

    for (int i = 0; i <= 255; i++)
        letters += char(i);

    for (int i = 0; i < stringLength; i++) {
        value += letters[rand() % letters.size()];
    }

    return value;
}

int main() {
    kvstore obj;
    double ans = 0;
    string letters = "", letters2 = "";
    for (char i = 'a'; i <= 'z'; i++)
        letters += i;
    for (char i = 'A'; i <= 'Z'; i++)
        letters += i;

    for (int i = 0; i <= 255; i++)
        letters2 += char(i);

    int val1 = rand() % 26;
    int val2 = rand() % 255;

    for (int i = 0; i < 10000; i++) {
        int k = rand() % 64 + 1;
        int v = rand() % 256 + 1;
        string key = "", value = "2";

        for (int i = 0; i < val1; i++) {
            int x = rand() % letters.size();
            key += letters[x];
        }
        key += '\0';
        for (int i = 0; i < val2; i++) {
            value += letters[rand() % letters.size()];
        }

        value += '\0';
        // cout << key << "    " << value << endl;
        Slice kk, vv;
        kk.size = val1;
        vv.size = val2;
        kk.data = (char *)malloc(sizeof(char) * val1 + 8);
        vv.data = (char *)malloc(sizeof(char) * val2 + 8);
        for (int j = 0; j < val1; j++) {
            kk.data[j] = key[j];
        }
        for (int j = 0; j < val2; j++) {
            vv.data[j] = value[j];
        }
        // kk.data = &key;
        // vv.data = &value;
        struct timespec start, stop;
        double accum;

        clock_gettime(CLOCK_MONOTONIC, &start);
        obj.get(kk, vv);
        clock_gettime(CLOCK_MONOTONIC, &stop);
        accum = (stop.tv_sec - start.tv_sec) +
                (stop.tv_nsec - start.tv_nsec) / BILLION;
        ans += accum;
    }
    printf("time : %lf\n\n", ans);
    return 0;
}
