#include <stdio.h>

int isPerfect(int num) {
    int sum = 0;
    for (int i = 1; i <= num / 2; i++) {
        if (num % i == 0) {
            sum += i;
        }
    }
    return (sum == num);
}

void perfectNumbers(int start, int end) {
    for (int i = start; i <= end; i++) {
        if (isPerfect(i)) {
            printf("%d\n", i);
        }
    }
}

int main() {
    int start, end;
    scanf("%d", &start);
    scanf("%d", &end);  
     
    perfectNumbers(start, end);
    return 0;
}
