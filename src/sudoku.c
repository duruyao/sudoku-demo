/**
 * @file sudoku.c 
 * souces file of sudoku algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-02-18
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "sudoku.h"

/*

9 x 9 Sudoku

 1 2 3 | 4 5 6 | 7 8 9
 5 9 7 | 3 2 8 | 4 1 6
 4 6 8 | 9 7 1 | 5 3 2
-------+-------+-------
 9 5 4 | 7 8 3 | 2 6 1
 7 1 6 | 5 9 2 | 3 4 8
 3 8 2 | 1 6 4 | 9 5 7
-------+-------+-------
 6 7 5 | 8 3 9 | 1 2 4
 2 4 9 | 6 1 5 | 8 7 3
 8 3 1 | 2 4 7 | 6 9 5

*/

static const int POSS[] =  {0,      /* 000000000 */
                            1,      /* 000000001 */
                            2,      /* 000000010 */
                            4,      /* 000000100 */
                            8,      /* 000001000 */
                            16,     /* 000010000 */
                            32,     /* 000100000 */
                            64,     /* 001000000 */
                            128,    /* 010000000 */
                            256,    /* 100000000 */
                            511};   /* 111111111 */


/******************************************************************************/
/*                                                                            */
/* struct defination                                                          */
/*                                                                            */
/******************************************************************************/


struct status {
    uint8_t ele;
    int     nb;
    int     poss;
};


/******************************************************************************/
/*                                                                            */
/* function defination                                                        */
/*                                                                            */
/******************************************************************************/


/*
 * printf a sudoku table with grid, e.g.
 *    |   |
 * ---+---+---
 *    |   |
 * ---+---+---
 *    |   |
 * for a standard sudoku, whose number of rows is equal to its number
 * of columns, and is also equal to its number of subsquares.
 *
 * @param arr is an allocated array of 8 bits unsigned integer.
 * @param N   is the size of sudoku.
 *
 */

void print_sudo(uint8_t **arr, int N) {
    int tmp     = N;
    int nb_bit  = 0;
    int n       = sqrt(N);
    char fmt[4] = {'\0'};
    char str[8] = {'\0'};

    while (tmp) {
        tmp /= 10;
        nb_bit++;       /* is number of bits of decimal of the N */
    }

    snprintf(fmt, 4, "%%%du", nb_bit);

    for (int i = 0; i < N; i++) {
        if (i != 0 && i % n == 0) {
            for (int x = n; x--; ) {
                for (int y = n; y--; printf("-"))
                    for (int k = nb_bit; k--; printf("-"));
                if (x)
                    printf("-+");
            }
            printf("-\n");
        }

        for (int j = 0; j < N; j++) {
            snprintf(str, 8, fmt, (unsigned)arr[i][j]);
            if (j != 0 && j % n == 0) {
                printf(" | %s", str);
            } else {
                printf(" %s", str);
            }
        }
        printf("\n");
    }
}

/*
 * generate N * N randomly ordered sudoku.
 *
 * @param arr is an allocated array of 8 bits unsigned integer.
 * @param N   is the size of sudoku.
 *
 * @return 1 on success, otherwise 0.
 *
 */

int gen_sudo(uint8_t **arr, int N) {
    int ret = 1;
    uint8_t *que = (uint8_t *)malloc(sizeof(uint8_t) * N);
    uint8_t *q   = que;

    /* generate a randomly ordered queue contaque of 
     * numeric (from 1 to N) */
    for (int i = 0; i < N; que[i] = i + 1, i++);
    srand((unsigned)time(NULL));
    for (int num = N, i = 0; num > 0; q++, num--) {
        i = rand() % num;
        if (*q != q[i]) {
            *q  += q[i];
            q[i] = *q - q[i];
            *q  -= q[i];
        }
    }

    for (int idx = 0; idx < N; idx++) {
        ret = dfs_gen(arr, 0, que[idx], N);
        if (ret == 0)
            break;
    }

    if (que) free(que);
    return ret;
}

/*
 * generate standard sudoku using depth first search method.
 *
 * @param arr     is an allocated array of 8 bits unsigned integer.
 * @param sub     is the index of subsquare of sudoku, e.g.
 *                 0 | 1 | 2
 *                ---+---+---
 *                 3 | 4 | 5
 *                ---+---+---
 *                 6 | 7 | 8
 * @param numeric is current numeric should be put in sudoku.
 * @param N       is the size of sudoku.
 *
 * @return 1 on success, otherwise 0.
 *
 */

int dfs_gen(uint8_t **arr, int sub, uint8_t numeric, int N) {
    if (sub == N)
        return 1;
    int ret = 0;
    int n = sqrt(N);
    int x = (sub / n) * n;
    int y = (sub % n) * n;
    for (int i = x; i < x + n; i++) {
        for (int j = y; j < y + n; j++) {
            if (arr[i][j] != 0)
                continue;

            int flag = 1;
            for (int k = 0; flag && k < N; k++) {
                /* the numeric is existed in current row or column,
                 * and it cannot be put in this position (i, j) */
                if (arr[i][k] == numeric || arr[k][j] == numeric)
                    flag = 0;
            }

            if (flag) {
                arr[i][j] = numeric;
                if (dfs_gen(arr, sub + 1, numeric, N))
                    return 1;
                else
                    arr[i][j] = 0;
            }
        }
    }
    return ret;
}

int init_stat_tab(uint8_t **arr, int **tab, int N) {
    int n    = sqrt(N);
    int poss = 0;

    /* tanslate input array to sudoku status table */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (arr[i][j] = 0)
                tab[i][j] = POSS[10];
            else
                tab[i][j] = 0 - arr[i][j];
        }
    }

    /* initialize sudoku status table */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tab[i][j] < 0) {
                poss = POSS[0 - tab[i][j]];

                /* remove the possibility from current row and column */
                for (int k = 0; k < N; k++) {
                    if (tab[i][k] > 0)
                        tab[i][k] = tab[i][k] & (~poss);
                    /* error: invalid sudoku */
                    else if (k != j && tab[i][k] == tab[i][j])
                        return -1;
                    else if (tab[i][k] == 0)
                        return -2;

                    if (tab[k][j] > 0)
                        tab[k][j] = tab[k][j] & (~poss);
                    /* error: valid sudoku */
                    else if (k != i && tab[k][j] == tab[i][j])
                        return -1;
                    else if (tab[k][j] == 0)
                        return -2;
                }

                /* remove the possibility from current n*n subsquares */
                for (int x = i - (i % n); x < i - (i % n) + n; x++) {
                    for (int y = j - (j % n); y < j - (j % n) + n; j++) {
                        if (tab[x][y] > 0)
                            tab[x][y] = tab[x][y] & (~poss);
                        /* error: valid sudoku */
                        else if ((x != i || y != j) && tab[x][y] == tab[i][i])
                            return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int solve_sudo(uint8_t **arr, int N) {
    int   ret = 0;
    int **tab = NULL;
    if ((tab = (int **)malloc(sizeof(int *) * N)) == NULL)
        return -1;
    for (int i = 0; i < N; i++)
        if ((tab[i] = (int *)malloc(sizeof(int) * N)) == NULL)
            return -1;
    if ((ret = init_stat_tab(arr, tab, N)) < 0)
        goto end;

end:
    while (N--) free(tab[N]);
    free(tab);
    return ret;
}

