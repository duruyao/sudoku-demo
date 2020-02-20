/**
 * @file demo.c
 * using demo of sudoku algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-02-18
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "sudoku.h"

int main(int argc, char **argv) {
    int   N       = 9;
    int   GEN     = 1;
    int   MULT    = 0;
    char *INFILE  = (char*)"data/input.txt"; 
    char *OUTFILE = (char*)"data/output.txt";
    
    int opt = 0;
    int cnt = 0;
    int ret = 0;

    Sudoku *sudo   = NULL;
    FILE   *input  = NULL;
    FILE   *output = NULL;

    if (argc < 2 || argc > 7) {
        fprintf(stderr,
                "Usage:\n"
                "   %s -g -n <Num> [-o <Outfile>]\n"
                "               Generate Num x Num sudoku to outfile\n\n"
                "   %s -s [-i <Infile>] [-o <Outfile>] [-m]\n"
                "               Write solution of sudo from infile to outfile\n",
                argv[0], argv[0]);
        ret = 1;
        goto end;
    } else {
        while ((opt = getopt(argc, argv, "gsmn:i:o:")) != -1) {
            switch (opt) {
                case 'g':
                    GEN = 1;
                    break;
                case 's':
                    GEN = 0;
                    break;
                case 'n':
                    N = atoi(optarg);
                    break;
                case 'm':
                    MULT = 1;
                    break;
                case 'i':
                    INFILE = optarg;
                    break;
                case 'o':
                    OUTFILE = optarg;
                    break;
                default:;
            }
        }
    }
        
    if (GEN) {
        if ((ret = init_sudo(&sudo, &N, NULL)) < 0)
            goto end;

        if ((ret = gen_sudo(sudo)) < 0)
            goto end;

        if((output = fopen(OUTFILE, "w")) == NULL) {
            ret = 1;
            fprintf(stderr, "Error opening '%s'\n", OUTFILE);
            goto end;
        }

        output_sudo((sudo->data), (sudo->size), output);
        
        fprintf(stdout, "Generate %d x %d sudoku to '%s'\n", N, N, OUTFILE);
        
        output_sudo(sudo->data, sudo->size, stdout);
    } else {
        if((input = fopen(INFILE, "r")) == NULL) {
            ret = 1;
            fprintf(stderr, "Error opening '%s'\n", INFILE);
            goto end;
        }

        if((output = fopen(OUTFILE, "w")) == NULL) {
            ret = 1;
            fprintf(stderr, "Error opening '%s'\n", OUTFILE);
            goto end;
        }

        if ((ret = init_sudo(&sudo, &N, input)) < 0)
            goto end;
        fprintf(stdout, "Read %d x %d sudoku from '%s'\n", N, N, INFILE);

        output_sudo(sudo->data, sudo->size, stdout);
       
        fprintf(stdout, "Try to find solution(s)\n");
        if ((ret = solve_sudo(sudo, MULT, output)) < 0)
            goto end;
        
        cnt = ret;
        fprintf(stdout, "Write %d solution(s) to '%s'\n", cnt, OUTFILE);
    }
    
end:
    free_sudo(&sudo);
    if (input) fclose(input);
    if (output) fclose(output);
    return ret;
}

