#include <stdio.h>
#include <string.h>

struct sumn_special {char name[64]; double special;};

int main() {
    const char name_var[] = "some random name, should work?";
    struct sumn_special specials[32];
    struct sumn_special *pointr = specials;

    for (int n = 0; n < 32; n++) {
        strcpy(pointr->name, name_var);
        pointr->special = n * n * 3.1415;
        pointr += 1;
    } 
    printf("name: %s\nspecial: %lf\npointer: %p\n", specials[4].name, specials[4].special, (void*)&specials[4]);

    return 1;
}
