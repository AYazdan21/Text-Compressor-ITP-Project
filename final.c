#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 256
#define MAX_SIZE 65536
#define MOD 100003
#define P 337

int last[MOD];
int next[MAX_SIZE];
int indx[MAX_SIZE];
int lenght[MAX_SIZE];
char *words[MAX_SIZE];
int pos_index = 0;

char trans[MAX_SIZE];
int prev[MAX_SIZE], first[MAX_SIZE], pos = 0;

void output(int x, FILE *fout){
    if (x >= 0)
    {
        output(prev[x], fout);
        fwrite(&trans[x], 1, 1, fout);
    }
}

int hash(char *word, int len){
    int i, h = P;
    for (i = 0; i < len; ++i)
    {
        h = (P * h + word[i]) % MOD;
    }
    return h % MOD;
}

int get(char *word, int len){
    int i;
    int h = hash(word, len);

    for (i = last[h]; i != -1; i = next[i])
    {
        if (len == lenght[i] && !memcmp(words[i], word, len))
        {
            return indx[i];
        }
    }

    return -1; //
}

void add(char *word, int len){
    int h = hash(word, len);
    next[pos_index] = last[h];
    indx[pos_index] = pos_index;
    lenght[pos_index] = len;
    words[pos_index] = calloc(len, sizeof(char));
    memcpy(words[pos_index], word, len);
    last[h] = pos_index++;
}

void write(unsigned short x, FILE *fout){
    fwrite(&x, sizeof(unsigned short), 1, fout);
}

void generate(FILE *fin, FILE *fout){
    int i = 0;
    char word[MAX_SIZE];
    memset(word, 0, sizeof word);

    for (i = 0; i < ALPHA; ++i)
    {
        char c[2] = {i, 0};
        add(c, 1);
    }

    char c = 0;
    i = 0;
    while (fread(&c, sizeof(char), 1, fin))
    {
        word[i++] = c;
        int x = get(word, i);

        if (x == -1)
        {
            if (pos_index < MAX_SIZE)
            {
                add(word, i);
            }

            x = get(word, i - 1);
            write(x, fout);

            word[(i = 1) - 1] = c;
        }
    }

    unsigned int x = get(word, i);
    write(x, fout);
}

void decode(FILE *fin, FILE *fout){
    memset(prev, -1, sizeof prev);

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
            output(old, fout);
            fwrite(&c, 1, 1, fout);
            c = trans[first[old]];
        }
        else
        {
            output(new, fout);
            c = trans[first[new]];
        }

        if (old < MAX_SIZE && pos < MAX_SIZE)
        {
            first[pos] = first[old];
            prev[pos] = old;
            trans[pos++] = c;
        }

        old = new;
    }
}

int main(int argc, char *argv[])
{
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
    fptr_in = fopen(argv[2], "rb");
    fptr_out = fopen(argv[3], "wb");

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

    if (!strcmp(argv[1], "encode"))
    {
        memset(next, -1, sizeof next);
        memset(last, -1, sizeof last);
        generate(fptr_in, fptr_out);
        printf("\033[0;32m");
        printf("%s\n", "Compressed successfully!");
        printf("\033[0m");
    }
    else if (!strcmp(argv[1], "decode"))
    {
        decode(fptr_in, fptr_out);
        printf("\033[0;32m");
        printf("%s\n", "Decompressed successfully!");
        printf("\033[0m");
    }
    else
    {
        printf("\033[0;31m");
        printf("%s\n", "Request not valid! Please specify you request with 'encode' or 'decode'.");
        printf("\033[0m");
        fclose(fptr_in);
        fclose(fptr_out);
        return 0;
    }
    fclose(fptr_in);
    fclose(fptr_out);

    return 0;
}
