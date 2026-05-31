# Text Compressor

A high-performance text compression tool implementing the **LZW (Lempel-Ziv-Welch)** algorithm in C. This project provides efficient lossless compression and decompression for text files using dictionary-based encoding.

## 🚀 Features

- **Lossless Compression**: Perfectly preserves original data during compression and decompression
- **LZW Algorithm**: Industry-standard dictionary-based compression technique
- **Fast Performance**: Optimized C implementation with hash-based dictionary lookup
- **Large Dictionary Support**: Handles up to 65,536 dictionary entries
- **Binary Output**: Generates compact binary compressed files
- **Simple CLI**: Easy-to-use command-line interface

## 📋 Algorithm Overview

The LZW compression algorithm works by:

1. **Building a Dictionary**: Starts with all single-character entries (256 ASCII characters)
2. **Pattern Recognition**: Identifies repeated sequences in the input text
3. **Dynamic Encoding**: Adds new patterns to the dictionary as they're discovered
4. **Index Output**: Outputs dictionary indices instead of full strings

This approach is particularly effective for text files with repeated patterns, achieving significant compression ratios.

## 🛠️ Installation

### Prerequisites

- GCC compiler (or any C compiler)
- Make (optional, for build automation)

### Compilation

```bash
gcc final.c -o textcompressor
```

Or with optimization flags:

```bash
gcc -O3 final.c -o textcompressor
```

## 📖 Usage

### Basic Syntax

```bash
./textcompressor <operation> <input_file> <output_file>
```

### Compress a File

```bash
./textcompressor encode input.txt compressed.bin
```

### Decompress a File

```bash
./textcompressor decode compressed.bin output.txt
```

### Examples

```bash
# Compress a text file
./textcompressor encode document.txt document.compressed

# Decompress back to original
./textcompressor decode document.compressed document_restored.txt

# Compress a large log file
./textcompressor encode server.log server.log.compressed
```

## 🔧 Technical Details

### Dictionary Implementation

- **Hash Table**: Uses polynomial rolling hash for O(1) average lookup time
- **Collision Handling**: Chaining method for hash collisions
- **Maximum Size**: 65,536 entries (16-bit indices)
- **Hash Function**: `h = (P * h + char) % MOD` where P=337, MOD=100003

### File Format

- **Compressed Format**: Binary file containing 16-bit dictionary indices
- **Encoding**: Little-endian unsigned short integers
- **Dictionary**: Dynamically built during compression/decompression

### Performance Characteristics

- **Time Complexity**: O(n) for both compression and decompression
- **Space Complexity**: O(dictionary_size) ≈ O(65,536)
- **Best Case**: Highly repetitive text (compression ratio > 50%)
- **Worst Case**: Random data (may expand slightly due to overhead)

## 📊 Compression Ratios

Typical compression ratios for different file types:

| File Type | Compression Ratio |
|-----------|-------------------|
| Plain Text | 40-60% |
| Source Code | 50-70% |
| Log Files | 60-80% |
| Random Data | 0-10% |

*Note: Actual ratios depend on content repetitiveness*

## ⚠️ Limitations

- **Maximum Dictionary Size**: 65,536 entries (after which no new patterns are added)
- **Binary Output**: Compressed files are not human-readable
- **File Size**: Best suited for files with repetitive patterns
- **Memory Usage**: Requires ~1MB RAM for dictionary storage

## 🐛 Error Handling

The program provides clear error messages for common issues:

- **File Not Found**: "Could not find the file you are trying to compress!"
- **Invalid Operation**: "Request not valid! Please specify 'encode' or 'decode'."
- **Argument Errors**: "Too many inputs!" or "Not enough inputs!"

## 🧪 Testing

Test the compressor with the included sample file:

```bash
# Compress the sample
./textcompressor encode input.txt test.compressed

# Decompress
./textcompressor decode test.compressed test_output.txt

# Verify integrity
diff input.txt test_output.txt
```

If `diff` produces no output, the compression/decompression was successful!

## 📁 Project Structure

```
text-compressor/
├── final.c          # Main source code
├── LICENSE.txt      # MIT License
├── input.txt        # Sample input file
└── README.md        # This file
```

## 🔬 How It Works

### Compression Process

1. Initialize dictionary with 256 single-character entries
2. Read input character by character
3. Build longest matching sequence in dictionary
4. Output index of matched sequence
5. Add new sequence (match + next char) to dictionary
6. Repeat until end of file

### Decompression Process

1. Initialize dictionary with 256 single-character entries
2. Read 16-bit indices from compressed file
3. Output corresponding dictionary entry
4. Reconstruct dictionary using same logic as compression
5. Handle special case where index references not-yet-added entry
6. Repeat until end of file

## 🤝 Contributing

Contributions are welcome! Here are some ideas for improvements:

- Add support for different compression levels
- Implement adaptive dictionary reset
- Add progress bar for large files
- Support for directory compression
- Multithreading for faster processing
- GUI interface

## 📄 License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details.

## 👤 Author

**Amirreza Yazdanpanah**

## 🙏 Acknowledgments

- Based on the LZW algorithm by Abraham Lempel, Jacob Ziv, and Terry Welch
- Inspired by classic compression utilities like `compress` and GIF encoding

## 📚 References

- [LZW Compression Algorithm](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)
- [Data Compression Techniques](https://www.cs.duke.edu/csed/curious/compression/lzw.html)

---

**Note**: This is an educational implementation. For production use, consider established compression libraries like zlib, bzip2, or LZMA.
