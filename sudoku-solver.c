#include<stdio.h>
/*
 * Program to solve 9X9 sudoku puzzles of all difficulty levels.
 * Author - @rohit01
 */

#define  FALSE  0
#define  TRUE   1
#define  ERROR  1

int input_sudoku(int sudoku[9][9]);
int solve_sudoku(int sudoku[9][9], const int guess_in_progress=FALSE);
int print_sudoku(const int sudoku[9][9]);
void copy_sudoku(int source[9][9], int destination[9][9]);
int solve_at_xy(const int sudoku[9][9], const int row_no, const int column_no);
int check_conflict(const int sudoku[9][9], const int test_value, 
                   const int row_no, const int column_no);
int match_sudoku(int sudoku1[9][9], int sudoku2[9][9]);


int main() {
    int sudoku[9][9];
    if (input_sudoku(sudoku) == 1) {
        return ERROR;
    }
    printf("\nSudoku entered:\n");
    print_sudoku(sudoku);
    solve_sudoku(sudoku);
    printf("\nSolution:\n");
    print_sudoku(sudoku);
}


int input_sudoku(int sudoku[9][9]) {
    int c, i, j;
    i = j = 0;
    printf("Please enter sudoku row wise (0 for blank):\n");
    while ((i < 9) && (j < 9) && ((c = getchar()) != EOF)) {
        if ((c >= '0') && (c <= '9')) {
            sudoku[i][j++] = c - '0';
            if (j == 9) {
                ++i;
                j = 0;
            }
        }
    }
    if (i < 9) {
        printf("Error: Invalid Sudoku. Aborting!\n");
        return ERROR;
    }
    return 0;
}


int print_sudoku(const int sudoku[9][9]) {
    int i, j, c;
    printf("+-------+-------+-------+");
    for(i=0; i<9; ++i) {
        printf("\n|");
        for(j=0; j<9; ++j) {
            c = (sudoku[i][j] == 0) ? ' ' : '0' + sudoku[i][j];
            printf(" %c", c);
            if((j + 1) % 3 == 0) {
                printf(" |");
            }
        }
        if((i + 1) % 3 == 0) {
            printf("\n+-------+-------+-------+");
        }
    }
    printf("\n");
    return 0;
}


int solve_sudoku(int sudoku[9][9], const int guess_in_progress) {
    int i, j, guess, solution[9][9], temp_sudoku[9][9];

    copy_sudoku(solution, sudoku);
    do {
        copy_sudoku(temp_sudoku, solution);
        for(i = 0; i < 9; ++i) {
            for(j = 0; j < 9; ++j) {
                if(solution[i][j] == 0) {
                    solution[i][j] = solve_at_xy(solution, i, j);
                }
            }
        }
    } while (match_sudoku(solution, temp_sudoku) != 0);

    for(i = 0; i < 9; ++i) {
        for(j = 0; j < 9; ++j) {
            if(solution[i][j] == 0) {
                if(guess_in_progress == TRUE) {
                    return ERROR;
                }
                for(guess = 1; guess <= 9; ++guess) {
                    if (check_conflict(solution, guess, i, j)) {
                        continue;
                    }
                    copy_sudoku(temp_sudoku, solution);
                    temp_sudoku[i][j] = guess;
                    if (solve_sudoku(temp_sudoku, TRUE) != 0) {
                        continue;
                    }
                    copy_sudoku(solution, temp_sudoku);
                }
            }
        }
    }
    copy_sudoku(sudoku, solution);
    return 0;
}


void copy_sudoku(int destination[9][9], int source[9][9]) {
    int i, j;
    for(i = 0; i < 9; ++i) {
        for(j = 0; j < 9; ++j) {
            destination[i][j] = source[i][j];
        }
    }
}


int match_sudoku(int sudoku1[9][9], int sudoku2[9][9]) {
    int i, j;
    for(i = 0; i < 9; ++i) {
        for(j = 0; j < 9; ++j) {
            if (sudoku1[i][j] != sudoku2[i][j]) {
                return ERROR;
            }
        }
    }
    return 0;
}

int solve_at_xy(const int sudoku[9][9], const int row_no, const int column_no) {
    int test_value, i, j, row_start, column_start;

    for (test_value = 1; test_value <= 9; ++test_value) {
        if (check_conflict(sudoku, test_value, row_no, column_no) == 1) {
            continue;
        }
        // Row check //
        for(i = 0; i < 9; ++i) {
            if((sudoku[row_no][i] != 0) || (i == column_no)) {
                continue;
            }
            if(check_conflict(sudoku, test_value, row_no, i) == 0) {
                break;
            }
        }
        if(i == 9) {
            return test_value;
        }
        // Column check //
        for(i = 0; i < 9; ++i) {
            if((sudoku[i][column_no] != 0) || (i == row_no)) {
                continue;
            }
            if(check_conflict(sudoku, test_value, i, column_no) == 0) {
                break;
            }
        }
        if(i == 9) {
            return test_value;
        }
        // Box check //
        row_start = row_no - (row_no % 3);
        column_start = column_no - (column_no % 3);
        for(i = row_start; i < row_start + 3; ++i) {
            for(j = column_start; j < column_start + 3; ++j) {
                if((sudoku[i][j] != 0) || (i == row_no && j == column_no)) {
                    continue;
                }
                if(check_conflict(sudoku, test_value, i, j) == 0) {
                    break;
                }
            }
            if(j < column_start + 3) {
                break;
            }
        }
        if((i == row_start + 3) && (j == column_start + 3)) {
            return test_value;
        }
    }
    return 0;
}

int check_conflict(const int sudoku[9][9], const int test_value, 
                   const int row_no, const int column_no) {
    /* Return's 1 if there is a conflict.
     * Return's 0 if there is no conflict.
     */
    int i, j, row_start, column_start;
    // Row check //
    for(i = 0; i < 9; ++i) {
        if(sudoku[row_no][i] == test_value) {
            return ERROR;
        }
    }
    // column check //
    for(i = 0; i < 9; ++i) {
        if(sudoku[i][column_no] == test_value) {
            return ERROR;
        }
    }
    // Box Check //
    row_start = row_no - (row_no % 3);
    column_start = column_no - (column_no % 3);
    for(i = row_start; i < row_start + 3; ++i) {
        for(j = column_start; j < column_start + 3; ++j) {
            if(sudoku[i][j] == test_value) {
                return ERROR;
            }
        }
    }
    return 0;
}

