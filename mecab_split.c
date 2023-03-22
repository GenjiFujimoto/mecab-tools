#include <mecab.h>
#include <stdio.h>
#include <string.h>

#define MAX_SENTENCE_LEN 4096
#define MAX_WORD_LEN 1024
#define MAX_TYPE_LEN 16

#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", mecab_strerror (mecab)); \
    mecab_destroy(mecab); \
    return -1; }


void wrap_word(char *output, const char *word)
{
  sprintf(output, "<a href=\"bword:%s\">%s</a> ", word, word);
}

int main (int argc, char **argv)
{

  char input[MAX_SENTENCE_LEN];
  if (fgets(input, MAX_SENTENCE_LEN, stdin) == NULL)
      return 1;

  mecab_model_t *model, *another_model;
  mecab_t *mecab;
  mecab_lattice_t *lattice;
  const mecab_node_t *node;
  const char *result;
  int i;
  size_t len;

  model = mecab_model_new(argc, argv);
  CHECK(model);
  mecab = mecab_model_new_tagger(model);
  CHECK(mecab);
  lattice = mecab_model_new_lattice(model);
  CHECK(lattice);
  mecab_lattice_set_sentence(lattice, input);
  mecab_parse_lattice(mecab, lattice);

  char output[MAX_SENTENCE_LEN];
  output[0] = '\0';

  char word_cur[MAX_WORD_LEN];
  char word_next[MAX_WORD_LEN];

  char type_cur_first[MAX_TYPE_LEN];
  char type_cur_second[MAX_TYPE_LEN];
  char type_next_first[MAX_TYPE_LEN];
  char type_next_second[MAX_TYPE_LEN];

  char next[MAX_WORD_LEN];

  node = mecab_lattice_get_bos_node(lattice)->next;
  for(; node->next; node = node->next)
  {
    strncpy(word_cur, node->surface, MAX_WORD_LEN);
    word_cur[node->length]='\0';

    sscanf(node->feature, "%15[^,],%15[^,],", type_cur_first, type_cur_second);
    sscanf((node->next)->feature, "%15[^,],%15[^,],", type_next_first, type_next_second);

    // Correct undesired splits
    while ( (strncmp(type_cur_first, "動詞", MAX_TYPE_LEN) == 0 && strncmp(type_next_first, "助動詞", MAX_TYPE_LEN) == 0)
	|| (strncmp(type_cur_first, "助動詞", MAX_TYPE_LEN) == 0 && strncmp(type_next_first, "助動詞", MAX_TYPE_LEN) == 0)
	|| strncmp(type_next_second, "接尾", MAX_TYPE_LEN) == 0
	|| strncmp(type_cur_first, "接頭詞", MAX_TYPE_LEN) == 0
	|| (strncmp(type_next_second, "接続助詞", MAX_TYPE_LEN) == 0 && strncmp((node->next)->surface, "が", (node->next)->length) != 0)
	|| (strncmp(type_cur_second, "数", MAX_TYPE_LEN) == 0 && strncmp(type_next_second, "数", MAX_TYPE_LEN) == 0))
    {
      strncat(word_cur, (node->next)->surface, (node->next)->length);

      node = node->next;
      sscanf(node->feature, "%15[^,],%15[^,],", type_cur_first, type_cur_second);
      sscanf((node->next)->feature, "%15[^,],%15[^,],", type_next_first, type_next_second);
    }
    if (strncmp(type_cur_first, "記号", MAX_TYPE_LEN) != 0)
      wrap_word(next, word_cur);
    else
      strncpy(next, word_cur, MAX_WORD_LEN);
    strcat(output,next);
  }
  printf("%s", output);


  mecab_destroy(mecab);
  mecab_lattice_destroy(lattice);
  mecab_model_destroy(model);

  return 0;
}

