#include <stdlib.h>
#include <string.h>
#include <mecab.h>
#include "kataconv.h"

#define MAX_LEN 1024

void hira_reading(char *input, char *output) {
    mecab_t *mecab = mecab_new2("");
    const mecab_node_t *node = mecab_sparse_tonode(mecab, input);

    char kata_reading[64];

    for (; node; node = node->next) {
	if (node->stat == MECAB_BOS_NODE || node->stat == MECAB_EOS_NODE) continue;

	sscanf(node->feature, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%[^,],", kata_reading);
	if(strlen(kata_reading) == 0) {
	  strcpy(kata_reading, node->surface);
	  kata_reading[node->length] = '\0';
	}
	unsigned char *reading;
	kata2hira ((const unsigned char*) kata_reading, &reading);

	strcat(output, (char*)reading);
    }

    mecab_destroy(mecab);
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
    hira_reading(input, output);

    printf("%s\n", output);

    return 0;
}

