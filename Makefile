CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = palindromer
SOURCES = main.cpp cmdLine.cpp trie.cpp
OBJECTS = $(SOURCES:.cpp=.o)
GUI_TARGET = palindromer-visual
GUI_SOURCES = visualUI.cpp trie.cpp
WX_CONFIG = `wx-config --libs --cxxflags`

.PHONY: all clean

all: $(TARGET) $(GUI_TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(GUI_TARGET): $(GUI_SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(WX_CONFIG) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

install-visual: $(GUI_TARGET)
	cp $(GUI_TARGET) /usr/local/bin/

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all     - Build the palindromer executable"
	@echo "  clean   - Remove compiled files"
	@echo "  install - Install to /usr/local/bin (requires sudo)"
	@echo "  help    - Show this help message"
