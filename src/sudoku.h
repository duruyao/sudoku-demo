/**
 * @file sudoku.h 
 * head file of sudoku algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-02-18
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __SUDOKUH__
#define __SUDOKUH__

#ifdef  __cplusplus
extern "C" {
#endif /* __plusplus */


/******************************************************************************/
/*                                                                            */
/* struct defination                                                          */
/*                                                                            */
/******************************************************************************/

struct sudoku {
    unsigned   size;         /* number of rows                                */
    unsigned   nb_data;      /* number of cells that is not empty             */
    int8_t   **data;         /* 2-D array to store numeric of every cell      */
    uint64_t **stat;         /* 2-D array to store status of possibilities    */
};

typedef struct sudoku Sudoku;


/******************************************************************************/
/*                                                                            */
/* macro defination                                                           */
/*                                                                            */
/******************************************************************************/


/*
 * find number of '1' in binary format of a input data, the function is more
 * faster than some methods.
 *
 * @param data shoule be an unsigned integer, otherwise it will be converted
 *             to be an unsigned integer.
 *
 * @return a 8 bits unsigned integer as number of '1'.
 *
 */

#define NB_1(data) ({                                   \
    uint64_t _dt_  = (uint64_t)(data);                  \
    uint8_t  _cnt_ = 0;                                 \
    while (_dt_) {                                      \
        (_dt_) &= (_dt_ - 1);                           \
        _cnt_++;                                        \
    }                                                   \
    _cnt_;                                              \
})

/*
 * get log_2(data).
 *
 * @param data shoule be an unsigned integer, otherwise it will be converted
 *             to be an unsigned integer, and should be N-th power of 2.
 *
 * @return a positive as reault of log_2(data) on sucess, otherwise -1.
 */

#define LOG_2(data) ({                                  \
    uint64_t _dt_  = (uint64_t)(data);                  \
    int8_t   _cnt_ = -1;                                \
    if (!(_dt_ & (_dt_ - 1))) {                         \
        while (_dt_) {                                  \
            _dt_ >>= 1;                                 \
            _cnt_ ++;                                   \
        }                                               \
    }                                                   \
    _cnt_;                                              \
})


/******************************************************************************/
/*                                                                            */
/* function declaration                                                       */
/*                                                                            */
/******************************************************************************/


extern int  init_sudo       (Sudoku **, int *, FILE *);

extern int  free_sudo       (Sudoku **);

extern int  output_sudo     (int8_t **, int, FILE *);

extern int  gen_sudo        (Sudoku *);

extern int  dfs_gen         (int8_t **, int, int8_t, int);

extern int  solve_sudo      (Sudoku *, int, uint64_t *, FILE *);

extern int  dfs_solve       (int, unsigned *, int, uint64_t *,
                                  int, int8_t **, uint64_t **, FILE *);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__SUDOKUH__ */

