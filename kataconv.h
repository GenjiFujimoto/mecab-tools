#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

int kata2hira (const unsigned char * katakana, unsigned char ** hiragana_ptr)
{
    unsigned char * h;
    unsigned char * hiragana;

    hiragana = (unsigned char *) strdup ((const char *)katakana);
    h = hiragana;
    while (* h) {
        /* Check that this is within the katakana block from E3 82 A0
           to E3 83 BF. */
        if (h[0] == 0xe3 && (h[1] == 0x82 || h[1] == 0x83) && h[2] != '\0') {
            /* Check that this is within the range of katakana which
               can be converted into hiragana. */
            if ((h[1] == 0x82 && h[2] >= 0xa1) ||
                (h[1] == 0x83 && h[2] <= 0xb6) ||
                (h[1] == 0x83 && (h[2] == 0xbd || h[2] == 0xbe))) {
                /* Byte conversion from katakana to hiragana. */
                if (h[2] >= 0xa0) {
                    h[1] = h[1] - 1;
                    h[2] -= 0x20;
                }
                else {
                    h[1] = h[1] - 2;
                    h[2] += 0x20;
                }
            }
            h += 3;
        }
        else {
            h++;
        }
    }
    * hiragana_ptr = hiragana;
    return 0;
}

