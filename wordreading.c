#include <stdlib.h>
#include <string.h>
#include <mecab.h>
#include "kataconv.h"

#define FORMAT_WITH_READING "<ruby><rb>%.*s</rb><rt>%s</rt></ruby>"
#define FORMAT_NO_READING   "%.*s "
#define MAX_LEN 1024

void add_furigana(char *input, char *output) {
    mecab_t *mecab = mecab_new2(""); // initialise MeCab
    const mecab_node_t *node = mecab_sparse_tonode(mecab, input); // analyse text

    char kata_reading[32];
    char next[256];

    for (; node; node = node->next) {
	if (node->stat == MECAB_BOS_NODE || node->stat == MECAB_EOS_NODE) continue;

        const char *word = node->surface;
	sscanf(node->feature, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,],", kata_reading);
	unsigned char *reading;
	kata2hira ((const unsigned char*) kata_reading, &reading);

        if (strncmp(word, (char*)reading, node->length) != 0)
	    sprintf(next, FORMAT_WITH_READING, word->length, word, reading);
        else
	    sprintf(next, FORMAT_NO_READING, word->length, word);
	strcat(output, next);
    }

    mecab_destroy(mecab); // clean up MeCab
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
      printf("You need to give an argument");
      return 1;
    }

    char input[MAX_LEN];
    strncpy(input, argv[1], MAX_LEN - 1);
    input[MAX_LEN - 1] = '\0';

    char output[MAX_LEN] = "";

    add_furigana(input, output);

    printf("%s\n", output);

    return 0;
}

