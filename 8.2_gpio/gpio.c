#include <stdio.h>
#include <unistd.h>
int main(void) {
    FILE *p = NULL;
    int i=0;
    p = fopen("/sys/class/gpio/export", "w");
    fprintf(p, "4"); // open gpio 4
    fclose(p);
    p = fopen("/sys/class/gpio/gpio4/direction", "w");
    fprintf(p, "out");
    fclose(p);
    for (i=0; i<10; ++i) {
        usleep(200000);
        p = fopen("/sys/class/gpio/gpio4/value", "w");
        fprintf(p, "1");
        fclose(p);
        usleep(200000);
        p = fopen("/sys/class/gpio/gpio4/value", "w");
        fprintf(p, "0");
        fclose(p);
    }
    p = fopen("/sys/class/gpio/unexport", "w");
    fprintf(p, "4");
    fclose(p);
    p = NULL;
    return 0;
}
