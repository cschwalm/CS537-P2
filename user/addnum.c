#include <syscall.c>

void add(int x) {
    int y= 0;

    y = addnum(x);
    printf("%d\n", y);

}
