#include <bits/stdc++.h>
using namespace std;

struct Slice
{
	int size;
	char *data;
};

class kvstore
{

private:
	struct node
	{
		int cnt[52] = {0};
		node *ptr[52] = {NULL};
		char *val = NULL;
	};
	node *root = (node *)malloc(sizeof(node));

public:
	bool get(Slice &key, Slice &value)
	{
		node *cur = root;
		for (int i = 0; i < key.size; i++)
		{
			if (!cur)
				return false;
			if (key.data[i] <= 'Z' && cur->cnt[key.data[i] - 'A'])
			{
				cur = cur->ptr[key.data[i] - 'A'];
			}
			else if (key.data[i] > 'Z' && cur->cnt[key.data[i] - 'a' + 26])
			{
				cur = cur->ptr[key.data[i] - 'a' + 26];
			}
			else
				return false;
		}
		if (!cur || !cur->val)
			return false;
		value.size = strlen(cur->val);
		value.data = (char *)malloc((value.size + 1) * 8);
		strcpy(value.data, cur->val);
		return true;
	}

	int put(node *cur, Slice &key, Slice &value, int i)
	{
		if (!cur)
			return -1;
		if (i == key.size)
		{
			int ret = 0;
			if (!cur->val)
				ret = 1;
			cur->val = (char *)realloc(cur->val, (value.size + 1) * 8);
			if (!cur->val)
				return -1;
			strcpy(cur->val, value.data);
			return ret;
		}
		if (key.data[i] <= 'Z')
		{
			if (!cur->ptr[key.data[i] - 'A'])
			{
				cur->ptr[key.data[i] - 'A'] = (node *)malloc(sizeof(node));
				if (!cur->ptr[key.data[i] - 'A'])
					return -1;
			}
			int ret = put(cur->ptr[key.data[i] - 'A'], key, value, i + 1);
			if (ret == 1)
				cur->cnt[key.data[i] - 'A']++;
			if (!cur->cnt[key.data[i] - 'A'] && cur->ptr[key.data[i] - 'A'])
			{
				free(cur->ptr[key.data[i] - 'A']);
				cur->ptr[key.data[i] - 'A'] = NULL;
			}
			return ret;
		}
		else
		{
			if (!cur->ptr[key.data[i] - 'a' + 26])
			{
				cur->ptr[key.data[i] - 'a' + 26] = (node *)malloc(sizeof(node));
				if (!cur->ptr[key.data[i] - 'a' + 26])
					return -1;
			}
			int ret = put(cur->ptr[key.data[i] - 'a' + 26], key, value, i + 1);
			if (ret == 1)
				cur->cnt[key.data[i] - 'a' + 26]++;
			if (!cur->cnt[key.data[i] - 'a' + 26] && cur->ptr[key.data[i] - 'a' + 26])
			{
				free(cur->ptr[key.data[i] - 'a' + 26]);
				cur->ptr[key.data[i] - 'a' + 26] = NULL;
			}
			return ret;
		}
	}

	bool put(Slice &key, Slice &value)
	{
		if (put(root, key, value, 0) == -1)
			return false;
		return true;
	}

	bool del(node *cur, Slice &key, int i)
	{
		if (!cur)
			return false;
		if (i == key.size)
		{
			if (!cur->val)
				return false;
			free(cur->val);
			cur->val = NULL;
			return true;
		}
		if (key.data[i] <= 'Z' && cur->cnt[key.data[i] - 'A'])
		{
			if (!del(cur->ptr[key.data[i] - 'A'], key, i + 1))
				return false;
			cur->cnt[key.data[i] - 'A']--;
		}
		else if (key.data[i] > 'Z' && cur->cnt[key.data[i] - 'a' + 26])
		{
			if (!del(cur->ptr[key.data[i] - 'a' + 26], key, i + 1))
				return false;
			cur->cnt[key.data[i] - 'a' + 26]--;
		}
		else
			return false;
		return true;
	}

	bool del(Slice &key)
	{
		return del(root, key, 0);
	}

	bool get(int N, Slice &key, Slice &value)
	{
		char temp[65];
		int j = 0;
		node *cur = root;
		while (N != 1)
		{
			if (!cur)
				return false;
			int sum = 0;
			for (int i = 0; i < 52; i++)
			{
				sum += cur->cnt[i];
				if (sum >= N)
				{
					N -= sum - cur->cnt[i];
					temp[j++] = i < 26 ? 'A' + i : 'a' + i - 26;
					cur = cur->ptr[i];
					break;
				}
			}
			if (sum < N)
				return false;
		}
		if (!cur || !cur->val)
			return false;
		temp[j] = '\0';
		key.size = strlen(temp);
		key.data = (char *)malloc((key.size + 1) * 8);
		strcpy(key.data, temp);
		value.size = strlen(cur->val);
		value.data = (char *)malloc((value.size + 1) * 8);
		strcpy(value.data, cur->val);
		return true;
	}

	bool del(int N, node *cur)
	{
		if (!cur)
			return false;
		if (N == 1)
		{
			if (!cur->val)
				return false;
			free(cur->val);
			cur->val = NULL;
			return true;
		}
		int sum = 0;
		for (int i = 0; i < 52; i++)
		{
			sum += cur->cnt[i];
			if (sum >= N)
			{
				N -= sum - cur->cnt[i];
				if (!del(N, cur->ptr[i]))
					return false;
				cur->cnt[i]--;
				return true;
			}
		}
		if (sum < N)
			return false;
	}

	bool del(int N)
	{
		return del(N, root);
	}
};

Slice ms(String s)
{
	Slice k;
	k.size = s.length();
	k.data = (char *)malloc((k.size + 1) * 8);
	for (int i = 0; i < k.size; i++)
		k.data[i] = s[i];
	k.data[k.size] = '\0';
	return k;
}

Slice ms(char *s)
{
	Slice k;
	k.size = strlen(s);
	k.data = (char *)malloc((k.size + 1) * 8);
	strcpy(k.data, s);
	return k;
}

int main()
{
	kvstore obj;
	Slice key, value;
	key.size = strlen("HelloWorld");
	key.data = (char *)malloc((value.size + 1) * 8);
	strcpy(key.data, "HelloWorld");
	value.size = strlen("WhatTheHell");
	value.data = (char *)malloc((value.size + 1) * 8);
	strcpy(value.data, "WhatTheHell");
	cout << obj.put(key, value) << "\n";
}