#include "trie.h"
#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <random>

//=====  Brute-force parameters  =====
#define MAX_PALINDROMES 2000000
#define MAX_DEPTH 30

//=====  Monte-Carlo search parameters =====
//Try to stop after this length
#define STOP_LENGTH 100
//Max possible length
#define MAX_LENGTH 120
//Max number to generate
#define MAX_ITERS_GEN 1000000
//Probability of continuing if this is a word
#define PROB_CONT 0.75f

//Using global variables makes the recursive calls more compact
Trie g_tri_for;
Trie g_tri_bac;

//Random number generator
std::mt19937 mt(std::time(0));

//Dictionary should be list of words separated by newlines
bool LoadDictionary(const std::string& fname) {
  int num_words = 0;
  std::ifstream fin(fname);
  if (!fin.is_open()) { return false; }
  std::string line;
  std::unordered_set<std::string> all_words;
  while (std::getline(fin, line)) {
    for (auto& c : line) c = toupper(c);
    g_tri_for.add(line);
    std::reverse(line.begin(), line.end());
    g_tri_bac.add(line);
    num_words += 1;
  }
  std::cout << "Loaded " << num_words << " words." << std::endl;
  return true;
}

bool CanAddAnyLetter(int startIx, Trie* tri_for, Trie* tri_bac, int& ix, bool endable) {
  Trie::CircIter circIter = tri_for->circIter(startIx);
  while (circIter.next()) {
    ix = circIter.getIx();
    if (tri_bac == nullptr || tri_bac->hasIx(ix) || (endable && tri_bac->isWord)) {
      return true;
    }
  }
  return false;
}

bool FindFirstWordEnd(Trie* trie, char& c) {
  if (trie == nullptr) { return false; }
  for (int i = 0; i < NUM_LETTERS; ++i) {
    Trie* branch = trie->nodes[i];
    if (branch != nullptr && branch->isWord) {
      c = (char)('A' + i);
      return true;
    }
  }
  return false;
}

void BruteSearchRec(Trie* triFor, Trie* triBac, std::vector<char>& strFor, std::vector<char>& strBac, int depth, std::set<std::string>& palindromes) {
  if (palindromes.size() > MAX_PALINDROMES) { return; }
  if (depth >= MAX_DEPTH || depth < 0) {
    std::string str_for(strFor.begin(), strFor.end());
    std::string str_bac(strBac.begin(), strBac.end());
    std::reverse(str_bac.begin(), str_bac.end());
    palindromes.insert(str_for + '|' + str_bac);
    return;
  }

  if (triFor->isWord) {
    //Try iteration with a space on the forward side
    strFor.push_back(' ');
    const int numSpacesFor = std::count(strFor.begin(), strFor.end(), ' ');
    const int numSpacesBac = std::count(strBac.begin(), strBac.end(), ' ');
    const bool forceEnd = (numSpacesBac >= 2 || numSpacesFor >= 2 || (!strBac.empty() && strBac.back() == ' '));
    BruteSearchRec(&g_tri_for, triBac, strFor, strBac, (forceEnd ? -1 : depth + 1), palindromes);
    strFor.pop_back();
  }
  if (triBac->isWord) {
    //Try iteration with a space on the backward side
    strBac.push_back(' ');
    const int numSpacesFor = std::count(strFor.begin(), strFor.end(), ' ');
    const int numSpacesBac = std::count(strBac.begin(), strBac.end(), ' ');
    const bool forceEnd = (numSpacesBac >= 2 || numSpacesFor >= 2 || (!strFor.empty() && strFor.back() == ' '));
    BruteSearchRec(triFor, &g_tri_bac, strFor, strBac, (forceEnd ? -1 : depth + 1), palindromes);
    strBac.pop_back();
  }

  //Try iterations with no spaces
  Trie::PalIter iter = triFor->palIter(triBac);
  while (iter.next()) {
    char c = iter.getLetter();
    strFor.push_back(c);
    strBac.push_back(c);
    BruteSearchRec(iter.getFor(), iter.getBac(), strFor, strBac, depth + 1, palindromes);
    strFor.pop_back();
    strBac.pop_back();
  }
}

void BruteSearch(std::set<std::string>& palindromes, const std::string& startFor, const std::string& startBac) {
  std::vector<char> strFor;
  std::vector<char> strBac;
  Trie* triFor = &g_tri_for;
  Trie* triBac = &g_tri_bac;
  const size_t forIxSize = startFor.find_last_of(' ');
  const size_t bacIxSize = startBac.find_first_of(' ');
  int forIx = (forIxSize == std::string::npos ? 0 : ((int)forIxSize) + 1);
  int bacIx = (bacIxSize == std::string::npos ? startBac.length() - 1 : ((int)bacIxSize) - 1);
  while (forIx < (int)startFor.length()) {
    const int ix = startFor[forIx] - 'A';
    if (ix < 0 || ix >= NUM_LETTERS) { return; }
    triFor = triFor->nodes[ix];
    if (triFor == nullptr) { return; }
    forIx += 1;
  }
  while (bacIx >= 0) {
    const int ix = startBac[bacIx] - 'A';
    if (ix < 0 || ix >= NUM_LETTERS) { return; }
    triBac = triBac->nodes[ix];
    if (triBac == nullptr) { return; }
    bacIx -= 1;
  }
  BruteSearchRec(triFor, triBac, strFor, strBac, 0, palindromes);
}

void RandSearch(std::set<std::string>& palindromes, const std::string& startFor, const std::string& startBac) {
  std::vector<char> v_for;
  std::vector<char> v_bac;
  Trie* triFor = &g_tri_for;
  Trie* triBac = &g_tri_bac;
  const size_t forIxSize = startFor.find_last_of(' ');
  const size_t bacIxSize = startBac.find_first_of(' ');
  int forIx = (forIxSize == std::string::npos ? 0 : ((int)forIxSize) + 1);
  int bacIx = (bacIxSize == std::string::npos ? startBac.length() - 1 : ((int)bacIxSize) - 1);
  while (forIx < (int)startFor.length()) {
    const int ix = startFor[forIx] - 'A';
    if (ix < 0 || ix >= NUM_LETTERS) { return; }
    triFor = triFor->nodes[ix];
    if (triFor == nullptr) { return; }
    forIx += 1;
  }
  while (bacIx >= 0) {
    const int ix = startBac[bacIx] - 'A';
    if (ix < 0 || ix >= NUM_LETTERS) { return; }
    triBac = triBac->nodes[ix];
    if (triBac == nullptr) { return; }
    bacIx -= 1;
  }

  //Initialize distributions
  std::uniform_int_distribution<int> randIx(0, NUM_LETTERS - 1);
  std::uniform_real_distribution<float> rand(0.0f, 1.0f);
  Trie* tri_for;
  Trie* tri_bac;

  for (int i = 0; i < MAX_ITERS_GEN; ++i) {
    tri_for = triFor;
    tri_bac = triBac;
    v_for.clear();
    v_bac.clear();

    while (v_for.size() < MAX_LENGTH) {
      //If this got to a leaf node, we must start a new word.
      //Or if this could be the end of the word, start a new word with some probability
      if (tri_for->isLeaf() || (tri_for->isWord && rand(mt) > PROB_CONT)) {
        v_for.push_back(' ');
        tri_for = &g_tri_for;
      }

      //Choose random start for search
      int startIx = randIx(mt);

      //Check if any forward letters are valid backward letters
      int ix = -1;
      bool canAddLetter = CanAddAnyLetter(startIx, tri_for, tri_bac, ix, true);

      //If nothing could be valid, end now
      if (!canAddLetter && !tri_bac->isWord) { break; }

      //If we must end the word or we can with some probability, do it
      assert(canAddLetter);
      if (tri_bac->isWord && (!tri_bac->hasIx(ix) || rand(mt) > PROB_CONT)) {
        //It was already a valid word, end it first
        v_bac.push_back(' ');
        tri_bac = &g_tri_bac;
        //Find the new index and make sure the letter can be added
        canAddLetter = CanAddAnyLetter(startIx, tri_for, tri_bac, ix, true);
        if (!canAddLetter) { break; }
        assert(tri_bac->hasIx(ix));
      }

      //At this point the letter should be able to add for both directions
      const char c = (char)('A' + ix);
      assert(tri_for->hasIx(ix));
      assert(tri_bac->hasIx(ix));
      v_for.push_back(c);
      v_bac.push_back(c);
      tri_for = tri_for->decend(ix);
      tri_bac = tri_bac->decend(ix);

      //If past the stop length, quit as soon a valid ending is found.
      if (v_for.size() >= STOP_LENGTH && tri_for->isWord && tri_bac->isWord) {
        break;
      }
    }

    //Sentence is done, see if it can end
    bool isValid = false;
    if (tri_for->isWord && tri_bac->isWord) {
      isValid = true;
    } else if (tri_for->isWord) {
      char endChar;
      if (FindFirstWordEnd(tri_bac, endChar)) {
        v_bac.push_back(endChar);
        v_bac.push_back(' ');
        isValid = true;
      }
    } else if (tri_bac->isWord) {
      char endChar;
      if (FindFirstWordEnd(tri_for, endChar)) {
        v_for.push_back(endChar);
        v_for.push_back(' ');
        isValid = true;
      }
    }

    if (v_for.size() >= 10 && isValid) {
      std::string str_for(v_for.begin(), v_for.end());
      std::string str_bac(v_bac.begin(), v_bac.end());
      std::reverse(str_bac.begin(), str_bac.end());
      palindromes.insert(str_for + str_bac);
    }
  }
}