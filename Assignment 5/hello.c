#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4
#define MEMORY_SIZE 32

int checkP2(int x);
int* dTob(int n, int l);
int bTod(int a[], int l);

int main(){
    int pgs = PAGE_SIZE;
    int ms = MEMORY_SIZE;
    int nof = ms / pgs;
    int offset_bit = checkP2(pgs);
    int m = checkP2(ms);
    int pg_num_bit = m - offset_bit;
    int la[] = {8, 4, 3, 2, 15, 18, 25};
    
    int pmt[] = {3, 6, 8, 12, 2};
    
    printf("Execution Command in Terminal: ./p1\n");
    printf("Page size: %d\n", pgs);
    printf("Memory size: %d\n", ms);
    printf("Number of frames required: %d\n", nof);
    printf("Page number bits: %d\n", pg_num_bit);
    printf("Offset bits: %d\n", offset_bit);
    printf("Number of address spaces: %d\n", m);
    printf("Page Table_____\n");
    for(int i = 0; i < sizeof(pmt) / sizeof(pmt[0]); i++) {
        printf("%d -> %d\n", i, pmt[i]);
    }
    
    for(int i = 0; i < sizeof(la) / sizeof(la[0]); i++) {
        int logical_address = la[i];
        int page_number = logical_address >> offset_bit;
        int offset = logical_address & ((1 << offset_bit) - 1);
        if(page_number >= sizeof(pmt) / sizeof(pmt[0]) || pmt[page_number] == -1) {
            printf("%d is an invalid page number\n", logical_address);
        } else {
            int frame_number = pmt[page_number];
            int physical_address = (frame_number * pgs) + offset;
            if(physical_address >= ms) {
                printf("%d is an invalid physical address\n", physical_address);
            } else {
                printf("Corresponding physical address of logical address %d: %d\n", logical_address, physical_address);
            }
        }
    }

    return 0;
}

int checkP2(int x){
    int count = 0;
    while (x > 1){
        if (x % 2 != 0){
            return -1;
        }
        x /= 2;
        count++;
    }
    return count;
}

int* dTob(int n, int l){
    static int arr[32];
    for (int i = l - 1; i >= 0; i--){
        arr[i] = n % 2;
        n = n / 2;
    }
    return arr;
}

int bTod(int a[], int l){
    int decimal = 0;
    for (int i = 0; i < l; i++){
        decimal += a[i] * (1 << (l - 1 - i));
    }
    return decimal;
}
