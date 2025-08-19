#pragma once
#include <string>
#include <random>
#define NUM_LETTERS 26

class Trie {
public:
  class Iter {
  public:
    Iter(Trie** nodes) : n(nodes), ix(-1) {}
    inline int getIx() const { return ix; }
    inline char getLetter() const { return (char)(ix + 'A'); }
    inline Trie* get() const { return n[ix]; }
    bool next() {
      while (true) {
        ix += 1;
        if (ix >= NUM_LETTERS) { return false; }
        if (n[ix] != nullptr) { return true; }
      }
    }

  private:
    Trie** n;
    int ix;
  };

  class CircIter {
  public:
    CircIter(Trie** nodes, int startIx) : n(nodes), endIx(-1), ix(startIx-1) {}
    inline int getIx() const { return ix; }
    inline char getLetter() const { return (char)(ix + 'A'); }
    inline Trie* get() const { return n[ix]; }
    bool next() {
      while (true) {
        ix = (ix + 1) % NUM_LETTERS;
        if (ix == endIx) { return false; }
        if (endIx < 0) { endIx = ix; }
        if (n[ix] != nullptr) { return true; }
      }
    }

  private:
    Trie** n;
    int endIx;
    int ix;
  };

  class PalIter {
  public:
    PalIter(Trie** _triFor, Trie** _triBac) : triFor(_triFor), triBac(_triBac), ix(-1) {}
    inline int getIx() const { return ix; }
    inline char getLetter() const { return (char)(ix + 'A'); }
    inline Trie* getFor() const { return triFor[ix]; }
    inline Trie* getBac() const { return triBac[ix]; }
    bool next() {
      while (true) {
        ix += 1;
        if (ix >= NUM_LETTERS) { return false; }
        if (triFor[ix] == nullptr) { continue; }
        if (triBac[ix] == nullptr) { continue; }
        return true;
      }
    }

  private:
    Trie** triFor;
    Trie** triBac;
    int ix;
  };

  Trie(Trie* _parent = nullptr);
  ~Trie();

  void add(const std::string& str);
  bool has(const std::string& str) const;
  inline bool hasIx(int ix) const { return nodes[ix] != nullptr; }
  inline bool hasLetter(char c) const { return nodes[int(c - 'A')] != nullptr; }
  inline bool isRoot() const { return parent == nullptr; }
  inline bool isLeaf() const { 
    for (int i = 0; i < NUM_LETTERS; ++i) {
      if (nodes[i] != nullptr) { return false; }
    }
    return true;
  }
  inline Trie* decend(int ix) const { return nodes[ix]; }
  inline Trie* decendLetter(char c) const { return nodes[int(c - 'A')]; }

  Iter iter() { return Iter(nodes); }
  CircIter circIter(int startIx) { return CircIter(nodes, startIx); }
  PalIter palIter(Trie* triBac) { return PalIter(nodes, triBac->nodes); }

  //template<class T>
  //Trie* randomNode(T& rng, char& c) const {
  //  static const std::uniform_int_distribution<int> dist(0, NUM_LETTERS - 1);
  //  int ix = dist(rng);
  //  const int endIx = ix;
  //  while (true) {
  //    if (nodes[ix] != nullptr) {
  //      c = (char)(ix + 'A');
  //      return nodes[ix];
  //    }
  //    ix = (ix + 1) % NUM_LETTERS;
  //    if (ix == endIx) { return nullptr; }
  //  }
  //}

  Trie* nodes[NUM_LETTERS];
  bool isWord;
  Trie* parent;
};
