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
		value.data = cur->val;
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
			cur->val = (char *)realloc(cur->val, (value.size + 1) * sizeof(char));
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

	bool get(node *cur, char *s, int &len, int &N, Slice &value)
	{
		if (!cur)
			return 0;
		if (cur->val)
		{
			N--;
			if (!N)
			{
				if (!cur->val)
					return 0;
				value.size = strlen(cur->val);
				value.data = cur->val;
				return true;
			}
		}
		for (int i = 0; i < 52; i++)
		{
			if (!cur->ptr[i])
				continue;
			*s++ = i < 26 ? 'A' + i : 'a' + i - 26;
			len++;
			if (get(cur->ptr[i], s, len, N, value))
			{
				return true;
			}
			else
			{
				s--;
				len--;
			}
		}
		return false;
	}

	bool get(int N, Slice &key, Slice &value)
	{
		char *s = (char *)malloc(65 * sizeof(char));
		int len = 0;
		if (!get(root, s, len, N, value))
			return 0;
		key.size = len;
		key.data = (char *)malloc((len + 1) * sizeof(char));
		for (int i = 0; i < len; i++)
			key.data[i] = s[i];
		key.data[len] = '\0';
		free(s);
		return true;
	}

	bool del(node *cur, int &N)
	{
		if (!cur)
			return 0;
		if (cur->val)
		{
			N--;
			if (!N)
			{
				if (!cur->val)
					return 0;
				free(cur->val);
				cur->val = NULL;
				return true;
			}
		}
		for (int i = 0; i < 52; i++)
		{
			if (!cur->ptr[i])
				continue;
			if (del(cur->ptr[i], N))
			{
				return true;
			}
		}
		return false;
	}

	bool del(int N)
	{
		if (!del(root, N))
			return false;
		return true;
	}
};
