# Palindromer
This tool helps create palindromes by brute-forcing all continuations from any starting point.
For more information of how the software functions see this [YouTube Video](https://youtu.be/ap08_AGPh8s).

## Building

### Linux/macOS
To build on Linux or macOS, use the provided Makefile:

```bash
make                # Build the palindromer executable
make clean          # Remove compiled files
make install        # Install to /usr/local/bin (requires sudo)
make help           # Show available targets
```

### VS Code
If using VS Code, you can build the project using the integrated build task:
- Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS)
- Type "Tasks: Run Task"
- Select "Build Palindromer"

## How to use
Mac and Linux users: run `make` to compile the executable for Unix-based systems.

Text is inputted by passing an uppercase string with `-t` that contains a `|` separator in the center.
This will generally look something like this: `t="WAS|SAW"`. Do not add punctuation.
The palindrome normally build from the outside to the center, but this can be reversed using `-r`.

There are two algorithms to generate continuations. The brute-force method (default) will find
all palindromes up to the next full words in both directions. This is usually best because you
won't miss anything that could come next and is equivalent to a breadth-first search.

The other option is a Monte-Carlo search. This uses randomness to explore branches deeply but
will generally not contain all possibilities. This is best when you're looking for some
inspiration to get started. To use Monte-Carlo search add `-m` to the command.

Palindromer by default uses `dictionary.txt` as the dictionary and `palindromes.txt` as the output file.
This can be changed with `-d="your_dictionary.txt"` and `-o="output_name.txt"`.

## An Example
Start with a list of all start and end palindrome pairs:

```bash
./palindromer
```

After looking through `palindromes.txt`, suppose we would like to continue developing `NOTE NO| ONE TON`

```bash
./palindromer -t="NOTE NO| ONE TON"
```

Next let's continue with: `NOTE NO ERASER |RES ARE ONE TON`.

```bash
./palindromer -t="NOTE NO ERASER |RES ARE ONE TON"
```

Lastly, I find a way to finish the sentence with `NOTE: NO ERASER OF ORES ARE ONE TON.`
It may not be very meaningful, but the whole process took under 10 minutes.
