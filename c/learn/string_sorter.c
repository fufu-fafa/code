#include <stdio.h>

int inside_of(const char input, const char arr[], const int which) {
    for (int i = 0; i < which; i++) {
        if (input == arr[i]) return 1;
    }
    return 0;
}

int main() {
    const char low_alphabets[] = "abcdefghijklmnopqrstuvwxyz";
    const char char_arr[] = "some unsorted string";
    const int input_size = sizeof(char_arr) - 1;

    int char_number = 0;
    char sorted[input_size];
    for (int x = 0; x < 26; x++) {
        for (int y = 0; y < input_size; y++) {
            if ((low_alphabets[x] == char_arr[y]) && !(inside_of(char_arr[y], sorted, char_number))) {
                sorted[char_number] = char_arr[y];
                char_number++;
            }
        }
    }
    for (int z = 0; z < char_number; z++) printf("%c, ", sorted[z]);
    printf("\ninputsize: %d\nchar_number: %d\n", input_size, char_number);
    return 0;
}
