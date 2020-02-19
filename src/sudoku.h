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

struct status;
typedef struct status Status;

struct sudoku;
typedef struct sudoku Sudoku;


/******************************************************************************/
/*                                                                            */
/* function declaration                                                       */
/*                                                                            */
/******************************************************************************/


extern int  init_sudo       (Sudoku **, int *, FILE *);

extern int  free_sudo       (Sudoku **);

extern int  output_sudo     (Sudoku *, FILE *);

extern int  gen_sudo        (Sudoku *);

extern int  dfs_gen         (uint8_t **, int, uint8_t, int);

extern int  solve_sudo      (Sudoku *);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__SUDOKUH__ */

