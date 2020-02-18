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

extern void print_sudo      (uint8_t **, int);

extern int  gen_sudo        (uint8_t **, int);

extern int  dfs_gen         (uint8_t **, int, uint8_t, int);

extern int  init_stat_tab   (uint8_t **, int **, int);

extern int  solve_sudo      (uint8_t **, int);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__SUDOKUH__ */

