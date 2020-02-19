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
    int N   = 9;
    int ret = 0;
    Sudoku *sudo    = NULL;
    FILE   *input   = NULL;
    FILE   *output  = NULL;
    char   *infile  = "data/input.txt";
    char   *outfile = "data/output.txt";
    
    if (argc > 2) {
        fprintf(stderr,
                "Usage:\n"
                "%s <N>     Generate N x N sudoku and write to '%s'\n"
                "%s         Solve sudoku from '%s' and write result to '%s'\n",
                argv[0], outfile, argv[0], infile, outfile);
        ret = 1;
        goto end;
    } else if (argc == 2) {
        N = atoi(argv[1]);
        if ((ret = init_sudo(&sudo, &N, NULL)) < 0)
            goto end;

        if ((ret = gen_sudo(sudo)) < 0)
            goto end;

        if((output = fopen(outfile, "w")) == NULL) {
            ret = 1;
            fprintf(stderr, "Error opening '%s'\n", outfile);
            goto end;
        }

        if ((ret = output_sudo(sudo, output)) < 0)
            goto end;
        fprintf(stdout, "Generate %d x %d sudoku to '%s'\n\n", N, N, outfile);
        
        if ((ret = output_sudo(sudo, stdout)) < 0)
            goto end;
    } else {
        if((input = fopen(infile, "r")) == NULL) {
            ret = 1;
            fprintf(stderr, "Error opening '%s'\n", infile);
            goto end;
        }

        if ((ret = init_sudo(&sudo, &N, input)) < 0)
            goto end;

        fprintf(stdout, "Read %d x %d sudoku from '%s'\n\n", N, N, infile);
        if ((ret = output_sudo(sudo, stdout)) < 0)
            goto end;
        
        if ((ret = solve_sudo(sudo)) < 0)
            goto end;

        if ((ret = output_sudo(sudo, output)) < 0)
            goto end;
        fprintf(stdout, "Generate %d x %d sudoku to '%s'\n\n", N, N, outfile);
        
        if ((ret = output_sudo(sudo, stdout)) < 0)
            goto end;
    }
    
end:
    free_sudo(&sudo);
    if (input) fclose(input);
    if (output) fclose(output);
    return ret;
}

