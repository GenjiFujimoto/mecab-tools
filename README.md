


This is a collection of programs written in C which use MeCab to split Japanese sentences into its components and provide furigana.

## Explanation
furigana_link_split.c splits the sentece, provides furigana and turns the words into links, which was programmed to be used with goldendict.\
yomi.c converts the input into hiragana\
wordreading.c simply provides furigana in ruby format

At the beginning of wordreading.c you can also adjust the format to your needs. The format specifier `%.*s` is used for the word and `%s` for the reading.

## Compilation
`gcc -o yomi yomi.c -lmecab`
