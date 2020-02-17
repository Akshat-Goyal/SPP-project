#include<bits/stdc++.h>
using namespace std;

struct Slice{
	uint8_t size;
	char *data;
};
struct node{
	int cnt[52] = {0};
	node *ptr[52] = {NULL};
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


int main(){
	Slice Key, Value;
	Key.size = 63;
	Key.data = (char*) malloc(65 * 8); 
	strcpy(Key.data, "Hell");
	Value.size = 255;
	Value.data = (char*) malloc(257 * 8); 
	strcpy(Value.data, "Welcome to Hell");
	put(Key, Value);
	cout << get(Key) << "\n";
}