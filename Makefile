palindromer:
	g++ -std=c++17 main.cpp cmdLine.cpp trie.cpp -o Palindromer
palindromer-visual:
	g++ -std=c++17 visualUI.cpp trie.cpp `wx-config --libs --cxxflags` -o PalindromerVisual