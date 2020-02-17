#include <bits/stdc++.h>
using namespace std;
#define BILLION 1E9

struct Slice{
	uint8_t size;
	char *data;
};
#define C 52
struct node{
	int cnt[C] = {0};
	node *ptr[C] = {NULL};
	char *val = NULL;
};
node *root = (node*) malloc(sizeof(node));
extern void put(Slice &key, Slice &value){
	node *cur = root;
	for(int i = 0; i < key.size; i++){
		if(key.data[i] <= 'Z'){
			cur->cnt[key.data[i] - 'A']++;
			if(!cur->ptr[key.data[i] - 'A']) cur->ptr[key.data[i] - 'A'] = (node*) malloc(sizeof(node));
			cur = cur->ptr[key.data[i] - 'A'];	
		}
		else{
			cur->cnt[key.data[i] - 'a' + 26]++;
			if(!cur->ptr[key.data[i] - 'a' + 26]) cur->ptr[key.data[i] - 'a' + 26] = (node*) malloc(sizeof(node));
			cur = cur->ptr[key.data[i] - 'a' + 26];	
		}
	}
	cur->val = (char*) realloc(cur->val, (value.size + 2) * 8);
	strcpy(cur->val, value.data);
}
extern char* get(Slice &key){
	node *cur = root;
	for(int i = 0; i < key.size; i++){
		if(key.data[i] <= 'Z' && cur->cnt[key.data[i] - 'A']){
			cur = cur->ptr[key.data[i] - 'A'];	
		}
		else if(key.data[i] > 'Z' && cur->cnt[key.data[i] - 'a' + 26]){ 
			cur = cur->ptr[key.data[i] - 'a' + 26];	
		}
		else{
			cur = NULL;
			break;
		}
	}
	if(!cur) return NULL;
	return cur->val;
}

int main(void) {
    int n = 5;
    double ans = 0;

    for (int i = 0; i < n; i++) {
        char key[65], value[257];
        Slice Key, Value;
        for (int j = 0; j < 64; j++) {
            int p;
            int flag = rand() % 2;
            if (flag == 1) {
                p = 65 + rand() % 26;
            } else {
                p = 97 + rand() % 26;
            }
            key[j] = (char)p;
        }
        for (int j = 0; j < 256; j++) {
            int  p = 65 + rand() % 26;
            value[j] = (char)p;
        }
        struct timespec start, stop;
        double accum;
        Key.size = 63;
        Key.data = (char*) malloc(65 * 8); 
        strcpy(Key.data, key);
        Value.size = 255;
        Value.data = (char*) malloc(257 * 8); 
        strcpy(Value.data, value);
        clock_gettime(CLOCK_MONOTONIC, &start);
        cout << Value.data << "\n";
        put(Key, Value);
        cout << get(Key) << "\n\n";
        clock_gettime(CLOCK_MONOTONIC, &stop);

        accum = (stop.tv_sec - start.tv_sec) +
                (stop.tv_nsec - start.tv_nsec) / BILLION;
        ans += accum;
    }

    printf("time : %lf\n\n", ans);
}