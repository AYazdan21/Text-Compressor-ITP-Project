#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 256      // Alphabet size, typically 256 for extended ASCII
#define MAX_SIZE 65536 // Maximum dictionary size
#define MOD 100003     // Modulus for hash function
#define P 337          // A prime number used in the hash function

// Arrays for dictionary implementation
int last[MOD];              // Stores the last index of a specific hash value
int next[MAX_SIZE];         // Points to the next element in the collision chain
int indx[MAX_SIZE];         // Stores the index of the word in the dictionary
int lenght[MAX_SIZE];       // Stores the length of the word
char *words[MAX_SIZE];      // Stores the actual words in the dictionary
int pos_index = 0;          // Keeps track of the current position in the dictionary

// Arrays used for decoding
char trans[MAX_SIZE];       // Stores the translation characters
int prev[MAX_SIZE];         // Stores the previous index for a decoded sequence
int first[MAX_SIZE];        // Stores the first index of a sequence
int pos = 0;                // Keeps track of the position during decoding

// Recursive function to output the decoded sequence
void output(int x, FILE *fout){
    if (x >= 0)
    {
        output(prev[x], fout);
        fwrite(&trans[x], 1, 1, fout); // Write the character to the output file
    }
}

// Hash function to calculate a hash value for a word
int hash(char *word, int len){
    int i, h = P;
    for (i = 0; i < len; ++i)
    {
        h = (P * h + word[i]) % MOD; // Polynomial rolling hash function
    }
    return h % MOD;
}

// Function to check if a word exists in the dictionary
int get(char *word, int len){
    int i;
    int h = hash(word, len);

    for (i = last[h]; i != -1; i = next[i])
    {
        if (len == lenght[i] && !memcmp(words[i], word, len))
        {
            return indx[i]; // Return the index if the word exists
        }
    }

    return -1; // Word not found
}

// Function to add a word to the dictionary
void add(char *word, int len){
    int h = hash(word, len);
    next[pos_index] = last[h];
    indx[pos_index] = pos_index;
    lenght[pos_index] = len;
    words[pos_index] = calloc(len, sizeof(char)); // Allocate memory for the word
    memcpy(words[pos_index], word, len); // Copy the word to the dictionary
    last[h] = pos_index++; // Update the last index for this hash value
}

// Function to write a 16-bit value to the output file
void write(unsigned short x, FILE *fout){
    fwrite(&x, sizeof(unsigned short), 1, fout);
}

// Function to compress the input file
void generate(FILE *fin, FILE *fout){
    int i = 0;
    char word[MAX_SIZE];
    memset(word, 0, sizeof word);

    // Initialize the dictionary with single-character words (extended ASCII)
    for (i = 0; i < ALPHA; ++i)
    {
        char c[2] = {i, 0};
        add(c, 1);
    }

    char c = 0;
    i = 0;
    while (fread(&c, sizeof(char), 1, fin))
    {
        word[i++] = c; // Add character to the current word
        int x = get(word, i); // Check if the word is in the dictionary

        if (x == -1)
        {
            if (pos_index < MAX_SIZE)
            {
                add(word, i); // Add the word to the dictionary if it doesn't exist
            }

            x = get(word, i - 1); // Get the longest match
            write(x, fout); // Write the index of the match to the output file

            word[(i = 1) - 1] = c; // Start a new word with the last character
        }
    }

    unsigned int x = get(word, i); // Get the index for the last word
    write(x, fout); // Write the last index to the output file
}

// Function to decompress the input file
void decode(FILE *fin, FILE *fout){
    memset(prev, -1, sizeof prev);

    // Initialize the translation table with single characters (extended ASCII)
    for (pos = 0; pos < ALPHA; ++pos)
    {
        trans[pos] = pos;
        first[pos] = pos;
    }

    int old = MAX_SIZE, new;
    char c;

    while (fread(&new, sizeof(unsigned short), 1, fin))
    {
        if (new >= pos)
        {
            output(old, fout); // Output the previous sequence
            fwrite(&c, 1, 1, fout); // Write the last character
            c = trans[first[old]]; // Get the first character of the old sequence
        }
        else
        {
            output(new, fout); // Output the current sequence
            c = trans[first[new]]; // Get the first character of the new sequence
        }

        if (old < MAX_SIZE && pos < MAX_SIZE)
        {
            first[pos] = first[old]; // Update the sequence information
            prev[pos] = old;
            trans[pos++] = c; // Add the new translation
        }

        old = new; // Update the old sequence index
    }
}

// Main function: handles file input/output and command-line arguments
int main(int argc, char *argv[])
{
    // Error handling for incorrect number of arguments
    if (argc > 4)
    {
        printf("\033[0;31m");
        printf("%s\n", "Too many inputs! Please try again later.");
        printf("\033[0m");
        return 0;
    }
    else if (argc < 4)
    {
        printf("\033[0;31m");
        printf("%s\n", "Not enough inputs! Please try again later.");
        printf("\033[0m");
        return 0;
    }

    FILE *fptr_in, *fptr_out;
    fptr_in = fopen(argv[2], "rb"); // Open the input file in binary mode
    fptr_out = fopen(argv[3], "wb"); // Open the output file in binary mode

    // Error handling for file opening
    if (fptr_in == NULL)
    {
        printf("\033[0;31m");
        printf("%s\n", "Could not find the file you are trying to compress! Please try again later.");
        printf("\033[0m");
        fclose(fptr_in);
        fclose(fptr_out);
        return 0;
    }

    if (fptr_out == NULL)
    {
        printf("\033[0;31m");
        printf("%s\n", "An error occured while trying to compress your file! Please try again later.");
        printf("\033[0m");
        fclose(fptr_in);
        fclose(fptr_out);
        return 0;
    }

    // Check if the operation is "encode" (compression)
    if (!strcmp(argv[1], "encode"))
    {
        memset(next, -1, sizeof next);
        memset(last, -1, sizeof last);
        generate(fptr_in, fptr_out); // Compress the file
        printf("\033[0;32m");
        printf("%s\n", "Compressed successfully!");
        printf("\033[0m");
    }
    // Check if the operation is "decode" (decompression)
    else if (!strcmp(argv[1], "decode"))
    {
        decode(fptr_in, fptr_out); // Decompress the file
        printf("\033[0;32m");
        printf("%s\n", "Decompressed successfully!");
        printf("\033[0m");
    }
    // Handle invalid requests
    else
    {
        printf("\033[0;31m");
        printf("%s\n", "Request not valid! Please specify your request with 'encode' or 'decode'.");
        printf("\033[0m");
        fclose(fptr_in);
        fclose(fptr_out);
        return 0;
    }
    fclose(fptr_in);
    fclose(fptr_out);

    return 0;
}
