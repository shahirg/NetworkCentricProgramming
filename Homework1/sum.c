#include <stdio.h>
#include <unistd.h>

int sum(int start, int end, int total) {
    return start == end ? total + start : sum(start + 1, end, total + start);
}
int main(int argc, char **argv) {
    printf("%d\n", sum(1, 100, 0));
    return 0;
}