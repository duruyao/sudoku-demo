/**
 * @file sudoku.c 
 * souces file of sudoku algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-02-18
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <stack>
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


/******************************************************************************/
/*                                                                            */
/* struct defination                                                          */
/*                                                                            */
/******************************************************************************/


std::stack<uint64_t> mstack;


/******************************************************************************/
/*                                                                            */
/* function defination                                                        */
/*                                                                            */
/******************************************************************************/


/*
 * initialize a Sudoku type data.
 *
 * after using the Sudoku type data, it should be released by calling
 * free_sudo().
 *
 * @param sudo_pp is a pointer to a pointer to an allocated Sudoku type data.
 * @param N_p     is a pointer to row number, that will be set as the size of
 *                sudoku when input is NULL, otherwise it will be returned as
 *                size of sudoku.
 * @param input   is a pointer to FILE type data, as standard input, whose data
 *                will be written in an empty sudoku table. it could be NULL,
 *                in this case, no data in sudoku table.
 *
 * @return 0 on sucess, otherwise nagative number.
 *
 */

int init_sudo(Sudoku **sudo_pp, int *N_p, FILE *input) {
    int ret   = 0;
    int valid = 0;
    int N     = *N_p;
    int n     = sqrt(N);
    Sudoku   *sudo  = NULL;
    int8_t   **data = NULL;
    uint64_t **stat = NULL;
    int8_t     num  = 0;
    uint64_t   poss = 0;

    for (int i = 1; !valid && i <= N; i++)
        if (i * i == N)
            valid = 1;
    if (!valid && !input) {
        fprintf(stderr, "Invalid parameter (%d cannot be row of sukouk)\n", N);
        return -2;
    }

    /* allocate Sudoku type data */
    if ((sudo = (Sudoku *)malloc(sizeof(Sudoku) * 1)) == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        ret = -1;
        goto fail;
    }
    
    /* read size of sudoku */
    if (input) {
        if (fscanf(input, "%d", &N) == EOF) {
            fprintf(stderr, "Error reading input file\n");
            ret = -1;
            goto fail;
        }
        n    = sqrt(N);
        *N_p = N;
    }

    /* allocate and initialize 2-D array to store data */
    if ((data = (int8_t **)malloc(sizeof(int8_t *) * N)) == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        ret = -1;
        goto fail;
    }
    for (int i = 0; i < N; i++) {
        if ((data[i] = (int8_t *)malloc(sizeof(int8_t) * N)) == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            ret = -1;
            goto fail;
        }
        memset(data[i], 0, sizeof(int8_t) * N);
    }

    *sudo_pp      = sudo;
    sudo->data    = data;
    sudo->stat    = NULL;
    sudo->size    = N;
    sudo->nb_data = 0;

    if (input == NULL)
        return 0;

    /* allocate and initialize 2-D array as status table */
    if ((stat = (uint64_t **)malloc(sizeof(uint64_t *) * N)) == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        ret = -1;
        goto fail;
    }
    for (int i = 0; i < N; i++) {
        if ((stat[i] = (uint64_t *)malloc(sizeof(uint64_t) * N)) == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            ret = -1;
            goto fail;
        }
        for (int j = 0; j < N; j++) {
            stat[i][j] = pow(2, N) - 1;
        }
    }

    sudo->stat = stat;

    /* read data from input file and set status table */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fscanf(input, "%" SCNd8 "", &num) == EOF) {
                fprintf(stderr, "Error reading input file\n");
                ret = -1;
                goto fail;
            }
            data[i][j] = 0 - abs(num);

            /* numeric of the current cell cannot be modified */
            if (data[i][j] < 0) {
                sudo->nb_data++;
                poss = (uint64_t)pow(2, num - 1);
                stat[i][j] = poss;

                /* remove the possibility from current row and column */
                for (int k = N; k--; ) {
                    /* error: valid sudoku */
                    if ((data[i][j] == data[i][k] && j != k) || 
                            (data[i][j] == data[k][j] && i != k)) {
                        fprintf(stderr,
                                "Invalid data found (%d,%d) (%d,%d)  (%d,%d)\n",
                                i, j, i, k, k, j);
                        ret = -1;
                        goto fail;
                    }
                    stat[i][k] &= (~poss);
                    stat[k][j] &= (~poss);
                }
                
                /* remove the possibility from current n*n subsquares */
                for (int x = n * (i / n); x < n * (i / n) + n; x++) {
                    for (int y = n * (j / n); y < n * (j / n) + n; y++) {
                        /* error: valid sudoku */
                        if ((x != i || y != j) && data[x][y] == data[i][j]) {
                            fprintf(stderr,
                                    "Invalid data found (%d,%d) (%d,%d)\n",
                                    i, j, x, y);
                            ret = -1;
                            goto fail;
                        }
                        stat[x][y] &= (~poss);
                    }
                }
            }
        }
    }

fail:
    if (ret < 0) {
        if (sudo)
            free(sudo);
        if (data) {
            for (int i = 0; data[i] && i < N; i++)
                if (data[i])
                    free(data[i]);
            free(data);
        }
        if (stat) {
            for (int i = 0; stat[i] && i < N; i++)
                if (stat[i])
                    free(stat[i]);
            free(stat);
        }
    }

    return ret;
}

/*
 * release a Sudoku type data.
 *
 * @param sudo_pp is a pointer to a pointer to an allocated Sudoku type data,
 *                and its value will be set as NULL.
 *
 * @return 0 on sucess, otherwise nagative number.
 *
 */

int free_sudo(Sudoku **sudo_pp) {
    Sudoku *sudo = *sudo_pp;

    if (sudo == NULL)
        return 0;
    if (sudo->data) {
        for (int n = sudo->size; n--; )
            if (sudo->data[n])
                free(sudo->data[n]);
        free(sudo->data);
    }
    if (sudo->stat) {
        for (int n = sudo->size; n--; )
            if (sudo->stat[n])
                free(sudo->stat[n]);
        free(sudo->stat);
    }
    free(sudo);
    *sudo_pp = NULL;

    return 0;
}

/*
 * printf a sudoku table with grid to standard output, like this.
 *    |   |
 * ---+---+---
 *    |   |
 * ---+---+---
 *    |   |
 * for a standard sudoku, whose number of rows is equal to its number
 * of columns, and is also equal to its number of subsquares.
 *
 * @param data   is an allocated 2-D array contains of data of sudoku.
 * @param N      is size of sudoku.
 * @param output is a pointer to FILE, as a standard output.
 *
 * @return 0 on success, otherwise negative number.
 *
 */

int output_sudo(int8_t **data, int N, FILE *output) {
    int  n      = sqrt(N);
    int  nb_bit = 0;        /* number of bits of decimal of the N */
    char fmt[8] = {'\0'};
    char FMT[8] = {'\0'};
    char str[8] = {'\0'};

    if (!data) {
        fprintf(stderr, "Data table of sudoku is empty\n");
        return -1;
    }

    if (!output)
        return -1;

    for (int tmp = N; tmp; nb_bit++, tmp /= 10);
    snprintf(fmt, 4, "%%%du", nb_bit);
    snprintf(FMT, 4, "%%%dc", nb_bit);

    for (int i = 0; i < N; i++) {
        if (i != 0 && i % n == 0) {
            for (int x = n; x--; ) {
                for (int y = n; y--; fprintf(output, "-"))
                    for (int k = nb_bit; k--; fprintf(output, "-"));
                if (x)
                    fprintf(output, "-+");
            }
            fprintf(output, "-\n");
        }

        for (int j = 0; j < N; j++) {
            if (data[i][j] == 0)
                snprintf(str, 8, FMT, ' ');
            else
                snprintf(str, 8, fmt, (unsigned)abs(data[i][j]));

            if (j != 0 && j % n == 0) {
                fprintf(output, " | %s", str);
            } else {
                fprintf(output, " %s", str);
            }
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
    return 0;
}

/*
 * generate N * N randomly ordered sudoku.
 *
 * @param sudo is a pointer to allocated Sudoku type data,
 *             and Sudoku::data cannot be NULL.
 *
 * @return 0 on success, otherwise negative number.
 *
 */

int gen_sudo(Sudoku *sudo) {
    int     ret = 0;
    int     N   = sudo->size;
    int8_t *q   = NULL; 
    int8_t *que = NULL; 
    
    if (sudo->data == NULL) {
        fprintf(stderr, "Data buffer of sudoku is empry\n");
        return -1;
    }

    que = (int8_t *)malloc(sizeof(int8_t) * N);
    if (que == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return -1;
    }
    q = que;

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

    for (int idx = 0; ret == 0 && idx < N; idx++) {
        if (!dfs_gen(sudo->data, 0, que[idx], N)) {
            fprintf(stderr, "Error generating sudoku\n");
            ret = -2;
        }
        sudo->nb_data += N;
    }

    if (que) free(que);
    return ret;
}

/*
 * generate standard sudoku using depth first search method.
 *
 * @param data    is an allocated 2-d array of 8 bits unsigned integer.
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

int dfs_gen(int8_t **data, int sub, int8_t numeric, int N) {
    if (sub == N)
        return 1;
    int ret = 0;
    int n = sqrt(N);
    int x = (sub / n) * n;
    int y = (sub % n) * n;
    for (int i = x; i < x + n; i++) {
        for (int j = y; j < y + n; j++) {
            if (data[i][j] != 0)
                continue;

            int flag = 1;
            for (int k = 0; flag && k < N; k++) {
                /* the numeric is existed in current row or column,
                 * and it cannot be put in this position (i, j) */
                if (data[i][k] == numeric || data[k][j] == numeric)
                    flag = 0;
            }

            if (flag) {
                data[i][j] = numeric;
                if (dfs_gen(data, sub + 1, numeric, N))
                    return 1;
                else
                    data[i][j] = 0;
            }
        }
    }
    return ret;
}

/*
 * solve a sudoku.
 *
 * @param sudo   is a pointer to allocated Sudoku type data,
 *               both Sudoku::data and Sudoku::stat cannot be NULL.
 * @param mult   means whether find all the solutions,
 *               find 1 solution if mult is 0, otherwise find all.
 * @param cnt    is pointer to a integer whose value will be set as number
 *               of solutions.
 * @param output is a pointer to FILE type data as the standard output,
 *               which could be NULL.
 *
 * @return 0 on success, otherwise negative number.
 *
 */

int solve_sudo(Sudoku *sudo, int mult, uint64_t *cnt, FILE *output) {
    int N = sudo->size;

    if (!(sudo->data) || !(sudo->stat)) {
        fprintf(stderr, "Data table or status table of sudoku is empty\n");
        return -1;
    }

    if (sudo->nb_data < N * N) {
        *cnt = 0; 
        if (dfs_solve(0, &(sudo->nb_data), sudo->size, cnt,
                      mult, sudo->data, sudo->stat, output) == 0) {
            fprintf(stderr, "No solution(s) found\n");
            return -2;
        }
    } else
        *cnt = 1;

    return 0;
}

/*
 * solve sudoku using depth first search method.
 *
 * @param idx    is index in the sudoku (from 0 to N * N).
 * @param nb     is number cells that is not empty.
 * @param N      is size of sudoku.
 * @param cnt    is pointer to a integer whose value will be set as number
 *               of solutions.
 * @param mult   is 0, find 1 solution, mult is 1, find all the solutions.
 * @param data   is an allocated 2-d array which as data table.
 * @param stat   is an allocated 2-d array which as status table.
 * @param output is a pointer to FILE type data as the standard output,
 *               which could be NULL.
 *
 * @return 1 on success, otherwise 0.
 *
 */

int dfs_solve(int idx, unsigned *nb, int N, uint64_t *cnt,
              int mult, int8_t **data, uint64_t **stat, FILE *output) {
    int ret  = 0;
    int flag = 0;
    int n    = sqrt(N);
    int i    = idx / N;
    int j    = idx % N;
    uint64_t poss   = 0;
    uint64_t stat_c = 0; /* copy of stat[i][j] */
    uint64_t stat_o = 0; /* old copy of stat_c */

    if (*nb == N * N) {
        (*cnt)++;
        output_sudo(data, N, output);
        output_sudo(data, N, stdout);
        return 1;
    } else if (!data[i][j] && !stat[i][j]) {
        return 0;
    } else if (data[i][j] < 0) { /* the numeric is constant */
        return (dfs_solve(idx + 1, nb, N, cnt, mult, data, stat, output));
    }
    
    /* the numeric is not consant */

    // (1) is slower than (2)
    // for (int stat_idx = 0; stat_idx < N; stat_idx++) {
    //     poss = pow(2, stat_idx);
    //     if ((poss & stat[i][j]) == 0)
    //         continue;
    // 
    //     data[i][j] = stat_idx + 1;
    //     (*nb)++;
    
    // (2) is slower than (3)
    // tat_c = stat[i][j];
    // for (int stat_idx = 0; stat_c; stat_idx++, stat_c >>= 1) {
    //     if (!(stat_c & 1))
    //         continue;
    //     poss = pow(2, stat_idx);
    //     data[i][j] = stat_idx + 1;
    //     (*nb)++;
    
    // (3)
    stat_c = stat[i][j];
    stat_o = stat[i][j];
    while (stat_c) {
        stat_c  &= (stat_c - 1);
        poss     = stat_o ^ stat_c;
        stat_o   = stat_c;
        /* LOG_2(p) is faster than log(p) / log(2) */
        data[i][j] = LOG_2(poss) + 1;
        (*nb)++;
    
        /* save previous possibilities of some cells */
        for (int k = N; k--; ) {
            mstack.push(stat[i][k]);
            mstack.push(stat[k][j]);
        }
        for (int x = n * (i / n); x < n * (i / n) + n; x++)
            for (int y = n * (j / n); y < n * (j / n) + n; y++)
                mstack.push(stat[x][y]);
        
        /* remove the possibility from the current row and column */
        for (int k = N; k--; ) {
            stat[i][k] &= (~poss);
            stat[k][j] &= (~poss);
        }
        /* remove the possibility from the current n*n subsquares */
        for (int x = n * (i / n); x < n * (i / n) + n; x++) {
            for (int y = n * (j / n); y < n * (j / n) + n; y++) {
                stat[x][y] &= (~poss);
            }
        }

        if ((flag = dfs_solve(idx + 1, nb, N, cnt, mult,
                              data, stat, output)) == 0 || mult) {
            if (flag)
                ret = 1;
            data[i][j] = 0;
            (*nb)--;

            /* revert the possibility from the current n*n subsquares */
            for (int x = n * (i / n) + n - 1; x >= n * (i / n); x--) {
                for (int y = n * (j / n) + n - 1; y >= n * (j / n); y--) {
                    stat[x][y] = mstack.top();
                    mstack.pop();
                }
            }
            /* revert the possibility from the current row and column */
            for (int k = 0; k < N; k++) {
                stat[k][j] = mstack.top();
                mstack.pop();
                stat[i][k] = mstack.top();
                mstack.pop();
            }
        } else
            return 1;
    }

    return ret;
}

