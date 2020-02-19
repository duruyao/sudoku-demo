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
/* struct declaration                                                         */
/*                                                                            */
/******************************************************************************/

struct sudoku;
typedef struct sudoku Sudoku;


/******************************************************************************/
/*                                                                            */
/* macro defination                                                           */
/*                                                                            */
/******************************************************************************/


#define NB_1(u) ({                                      \
    uint64_t _u_ = (uint64_t)(u);                       \
    uint8_t  cnt = 0;                                   \
    while (_u_) {                                       \
        (_u_) &= (_u_ - 1);                             \
        cnt++;                                          \
    }                                                   \
    cnt;                                                \
})


/******************************************************************************/
/*                                                                            */
/* function declaration                                                       */
/*                                                                            */
/******************************************************************************/


extern int  init_sudo       (Sudoku **, int *, FILE *);

extern int  free_sudo       (Sudoku **);

extern int  output_sudo     (Sudoku *, FILE *);

extern int  gen_sudo        (Sudoku *);

extern int  dfs_gen         (int8_t **, int, int8_t, int);

extern int  solve_sudo      (Sudoku *);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__SUDOKUH__ */

