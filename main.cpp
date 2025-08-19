#include "cmdLine.h"
#include "palindrome.cpp"

int main(int argc, const char* argv[]) {
  //Commandline variables
  bool print_help = false;
  bool reverse_search = false;
  bool random_search = false;
  std::string input_text = "|";
  std::string dictionary_path = "dictionary.txt";
  std::string output_path = "palindromes.txt";

  //Commandline setup
  CmdLine cmd("Palindromer");
  cmd.addArgument({ "-h","--help" }, "Print this help message", &print_help);
  cmd.addArgument({ "-d","--dictionary" }, "Path to load the list of valid words", &dictionary_path);
  cmd.addArgument({ "-m","--montecarlo" }, "Use Monte Carlo Search instead of brute-force", &random_search);
  cmd.addArgument({ "-o","--output" }, "Output path for generated palindromes", &output_path);
  cmd.addArgument({ "-r","--reverse" }, "Build palindrome from the inside-out", &reverse_search);
  cmd.addArgument({ "-t","--text" }, "Palindrome input text with '|' as a center divider", &input_text);

  //Parse the command line
  if (!cmd.parse(argc, argv)) {
    return 1;
  }

  //Print the help menu
  if (print_help) {
    cmd.printHelp();
    return 0;
  }

  //Check for the divider
  const size_t firstPipeIx = input_text.find_first_of('|');
  const size_t lastPipeIx = input_text.find_last_of('|');
  if (firstPipeIx == std::string::npos) {
    std::cout << "ERROR: Input next needs at least one '|' divider." << std::endl;
    return 1;
  } else if (firstPipeIx != lastPipeIx) {
    std::cout << "ERROR: Input next must only have one '|' divider." << std::endl;
    return 1;
  }

  //Get starting string from argument
  std::string startFor = input_text.substr(0, firstPipeIx);
  std::string startBac = input_text.substr(firstPipeIx + 1);
  if (reverse_search) { std::swap(startFor, startBac); }

  //Load word frequency list
  std::cout << "Loading Dictionary " << dictionary_path << "..." << std::endl;
  if (!LoadDictionary(dictionary_path)) {
    std::cout << "Failed to load dictionary from file: " << dictionary_path << std::endl;
    return 1;
  }

  //Generate all the palindromes
  std::cout << "Generating..." << std::endl;
  std::set<std::string> palindromes;
  if (random_search) {
    RandSearch(palindromes, startFor, startBac);
  } else {
    BruteSearch(palindromes, startFor, startBac);
  }
  std::cout << "Found " << palindromes.size() << " continuations" << std::endl;

  //Save the results to a text file
  std::cout << "Saving..." << std::endl;
  std::ofstream fout(output_path);
  if (!fout.is_open()) {
    std::cout << "Failed to save results to file: " << output_path << std::endl;
    return 1;
  }
  for (const std::string& str : palindromes) {
    if (reverse_search) {
      const size_t ix = str.find_first_of('|');
      fout << "|" << str.substr(ix + 1, str.length() - ix - 1) << startBac << " " << startFor << str.substr(0, ix) << "|" << std::endl;
    } else {
      fout << startFor << str << startBac << std::endl;
    }
  }

  //Quit
  std::cout << "Done." << std::endl;
  return 0;
}