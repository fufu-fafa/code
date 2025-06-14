#include <stdio.h>

int main() {
    char sumn_words[69];
    printf("input sumn ionknow: ");
    scanf("%68s", sumn_words);
    printf("%s%c" , sumn_words, '\n');
    printf("%p%c", &sumn_words, '\n');
}
