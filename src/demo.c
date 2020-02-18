/**
 * @file demo.c
 * demo of sudoku algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-02-18
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "sudoku.h"

int main(int argc, char **argv) {
    int N = 64;
    uint8_t **arr = NULL;

    arr = (uint8_t **)malloc(sizeof(uint8_t *) *N);
    for (int i = 0; i < N; i++) {
        arr[i] = (uint8_t *)malloc(sizeof(uint8_t) *N);
        memset(arr[i], 0, sizeof(uint8_t) * N);
    }

    if (gen_sudo(arr, N))
        print_sudo(arr, N);
    else
        printf("No solution\n");

    while (N--) free(arr[N]);
    free(arr);
    return 0;
}

