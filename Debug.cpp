#pragma once

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

int indent = 1;
string indent_str = "\t";

void __indent() {
  int i = indent;
  while (i--) {
    cerr << indent_str;
  }
}

void __print(char x) { cerr << '\'' << x << '\''; }
void __print(bool x) { cerr << (x ? "true" : "false"); }
void __print(const char *x) { cerr << '\"' << x << '\"'; }
void __print(const std::string &x) { cerr << '\"' << x << '\"'; }
template <typename T> void __print(const T &x) { cerr << x; }

// to make it visible to other __print
template <typename T> void __print(const set<T> &x);
template <typename T> void __print(const multiset<T> &x);
template<typename Key, typename Mapped, typename Cmp_Fn, typename Tag> void __print(const tree<Key, Mapped, Cmp_Fn, Tag, tree_order_statistics_node_update> &x);
template <typename T, typename C, typename Comp> void __print(priority_queue<T, C, Comp>& x);
template <typename T> void __print(const vector<T> &x);
template <typename T> void __print(const deque<T> &x);
template <typename T> void __print(stack<T> x);
template <typename T> void __print(queue<T> x);
template <typename T> void __print(const list<T> &x);
template <typename T> void __print(const array<T, 2> &x);
template <typename T> void __print(const array<T, 3> &x);
template <typename T> void __print(const array<T, 4> &x);
template <typename T> void __print(const valarray<T> &x);
template <typename T> void __print(const vector<vector<T>> &x);
template <typename T> void __print(const deque<deque<T>> &x);
template <typename T, typename V> void __print(const map<T, V> &x);
template <typename T, typename V> void __print(const unordered_map<T, V> &x);
template <typename T, typename V> void __print(const pair<T, V> &x);


template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}

template <typename T, typename V> void __print(const map<T, V> &x) {
  cerr << '{' << endl;
  indent++;
  for (const auto &[key, val] : x) {
    __indent();
    __print(key);
    cerr << ": ";
    __print(val);
    cerr << "," << endl;
  }
  indent--;
  __indent();
  cerr << '}';
}

template <typename T, typename V> void __print(const unordered_map<T, V> &x) {
    cerr << '{' << endl;
    indent++;
    for (const auto &[key, val] : x) {
        __indent();
        __print(key);
        cerr << ": ";
        __print(val);
        cerr << "," << endl;
    }
    indent--;
    __indent();
    cerr << '}';
}

template <typename T> void __print(const vector<T> &x) {
  int f = 0;
  cerr << '[';
  for (const auto &i : x)
    cerr << (f++ ? ", " : ""), __print(i);
  cerr << ']';
}

template <typename T> void __print(const deque<T> &x) {
    int f = 0;
    cerr << '[';
    for (const auto &i : x)
        cerr << (f++ ? ", " : ""), __print(i);
    cerr << ']';
}

template <typename T> void __print(queue<T> x) {
    int f = 0;
    cerr << '[';
    while (!x.empty())
        cerr << (f++ ? ", " : ""), __print(x.front()), x.pop();
    cerr << ']';
}

template <typename T> void __print(stack<T> x) {
    cerr << "\n";
    cerr << "-----------------------------\n";
    cerr << "|            TOP            |\n";
    cerr << "-----------------------------\n";
    while (!x.empty())
        cerr << "      ", __print(x.top()) ,cerr << "\n", x.pop();
    cerr << "-----------------------------\n";
    cerr << "|           Bottom          |\n";
    cerr << "-----------------------------\n";
}

template <typename T, typename C, typename Comp> void __print(priority_queue<T, C, Comp>& x) {
    cerr << "\n";
    cerr << "-----------------------------\n";
    cerr << "|            TOP            |\n";
    cerr << "-----------------------------\n";
    while (!x.empty())
    cerr << "      ", __print(x.top()) ,cerr << "\n", x.pop();
    cerr << "-----------------------------\n";
    cerr << "|           Bottom          |\n";
    cerr << "-----------------------------\n";
}

template <typename T> void __print(const vector<vector<T>> &x) {
    cerr << '[';
    indent++;
    for (const auto &i : x) {
        cerr << endl;
        __indent();
        __print(i);
        cerr << ",";
    }
    cerr << endl;
    indent--;
    __indent();
    cerr << ']';
}

template <typename T> void __print(const deque<deque<T>> &x) {
    cerr << '[';
    indent++;
    for (const auto &i : x) {
        cerr << endl;
        __indent();
        __print(i);
        cerr << ",";
    }
    cerr << endl;
    indent--;
    __indent();
    cerr << ']';
}

template <typename T> void __print(const set<T> &x) {
  int f = 0;
  cerr << '{';
  for (const auto &i : x)
    cerr << (f++ ? ", " : ""), __print(i);
  cerr << '}';
}

template <typename T> void __print(const multiset<T> &x) {
    int f = 0;
    cerr << '{';
    for (const auto &i : x)
        cerr << (f++ ? ", " : ""), __print(i);
    cerr << '}';
}

template<typename Key, typename Mapped, typename Cmp_Fn, typename Tag> void __print(const tree<Key, Mapped, Cmp_Fn, Tag, tree_order_statistics_node_update> &x) {
    int f = 0;
    cerr << '{';
    for (const auto &i : x)
        cerr << (f++ ? ", " : ""), __print(i);
    cerr << '}';
}

template <typename T> void __print(const list<T> &x) {
  vector<T> v(begin(x), end(x));
  __print(v);
}

template <typename T> void __print(const array<T, 2> &x) {
  vector<T> v(begin(x), end(x));
  __print(v);
}

template <typename T> void __print(const array<T, 3> &x) {
  vector<T> v(begin(x), end(x));
  __print(v);
}

template <typename T> void __print(const array<T, 4> &x) {
  vector<T> v(begin(x), end(x));
  __print(v);
}

template <typename T> void __print(const valarray<T> &x) {
  vector<T> v(begin(x), end(x));
  __print(v);
}

void _print() { cerr << endl; }

template <typename T, typename... V> void _print(T t, V... v) {
  __print(t);
  if (sizeof...(v))
    cerr << ", ";
  _print(v...);
}

#define debug(x...)                                                                                 \
  cerr << #x << " = ",                                                                              \
  _print(x)

#define Time(i, x...)                                                                                     \
  auto ci##i = chrono::high_resolution_clock::now();                                                      \
  x                                                                                                       \
  auto cf##i = chrono::high_resolution_clock::now();                                                      \
  cerr << "\n\t\t Time(" << #i << ") = { ";                                                               \
  cerr << chrono::duration_cast<chrono::milliseconds>(cf##i - ci##i).count() << " milliseconds. , ";      \
  cerr << chrono::duration_cast<chrono::microseconds>(cf##i - ci##i).count() << " microseconds. }\n";

template <typename RandomIt> void debug_itr(RandomIt start, RandomIt end) {
  cerr << '[';
  for (RandomIt it = start; it != end; it++)
    cerr << (it != start ? ", " : ""), __print(*it);
  cerr << ']' << endl;
}

template <typename T>
void debug_bits(T val, int splitby = 4, int numofbits = 16) {
  bitset<sizeof(T) * 8> bits(val);
  int start = numofbits - 1;
  for (int i = start; i >= 0; i--)
    cerr << ((start - i) % splitby == 0 && i != start ? " " : "") << bits[i];
  cerr << endl;
}

