#include<stdio.h>
/*
 * Author - @rohit01
 * Program to Solve any sudoku (even Diabolical).
*/


int print_s(int S[9][9]);
int solve(int S[9][9], int status /*Input status = 0*/);
int guess_n_solve(int S[9][9]);
int findx(int x, int *H_arr, int *V_arr[9], int *B_arr[3]);
int check(int S[9][9]);
int find_at_xy(int S[9][9], const int x, const int y);
int alloc_values(int S[9][9], int **H_arr, int *V_arr[9], int *B_arr[3], int x, int y);
int compare(int S1[9][9], int S2[9][9]);
int is_correct(int S[9][9]);
int add_2_database(void);
int game_4m_database(const int game_no, int S[9][9]);

int main()
{
    int S[9][9], i, j, game_no;

    add_2_database();
    printf("\nInput game no.: ");
    scanf(" %d", &game_no);
    game_4m_database(game_no, S);
    printf("\n\n");
    print_s(S);
    printf("\n\n");
    if (solve(S, 0) == 1)
    {
        printf("\nThe program was unable to solve the Sudoku Completely...!!\n");
    }
    /*
    printf("Input numbers: ");
    for(i = 0; i < 9; i++)
    {   for(j = 0; j < 9; j++)
        {   scanf(" %d", &S[i][j]);
        }
    }
    printf("\nYou have entered the following Sudoku: \n");
    solve(S, 0);
    */
    printf("The Solution is given below:\n");
    print_s(S);
    return 0;
}
print_s(int S[9][9])
{
    int i, j, k;
//	textcolor(RED);   //**Scope for more features in windows.
    for (i = 0; i < 9; i++)
    {
        if ( i == 0)
        {
            for (k=0; k<13; k++)
            {
                printf("--");
            }
            printf("\b ");
            printf("\n");
        }
        for (j=0; j<9; j++)
        {
            if ( j==0 )
            {
                printf("| ");
            }
            if (S[i][j]>=1 && S[i][j]<=9)
            {
                printf("%d ", S[i][j]);
            }
            else
            {
                printf("  ");
            }
            if ( (j+1)%3==0 )
            {
                printf("| ");
            }
        }
        if ( (i+1)%3 == 0)
        {
            printf("\n");
            for (j=0; j<9; j++)
            {
                if ( j==0 )
                {
                    printf("| ");
                }
                printf("  ");
                if ( (j+1)%3==0 )
                {
                    printf("| ");
                }
            }
            printf("\n");
            for (k=0; k<13; k++)
            {
                printf("--");
            }
            printf("\b ");
        }
        printf("\n");
    }
    return 0;
}
int solve(int S[9][9], int status /*to avoid recursion, default=0*/)
{   int i, j, x, y;
    int S2[9][9];
    for (i = 0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            if (!(S[i][j]>=0 && S[i][j]<=9))
            {
                S[i][j] = 0;
            }
            S2[i][j] = S[i][j];
        }
    }
    i = j = x = y = 0;
    while ( check(S) == 1)
    {
        for ( i ; i<9; i++)
        {
            if (i != x)
            {
                j = 0;
            }
            for ( j ; j<9; j++)
            {
                if (S[i][j] == 0)
                {
                    break;
                }
            }
            if (j != 9 && S[i][j] == 0)
            {
                break;
            }
        }
        if (i >= 9)
        {
            if (compare(S, S2) == 0)
            {
                break;
            }
            for (i = 0; i<9; i++)
            {
                for (j=0; j<9; j++)
                {
                    S2[i][j] = S[i][j];
                }
            }
            i = 0;
            j = 0;
            continue;
        }
        x = i;	//Position of Zero = i,j = x,y
        y = j;
        S[x][y] = find_at_xy(S, x, y);
        j++;
    }
    if (status == 0 && check(S) == 1)
    {   if(guess_n_solve(S) != 0)
        {   return 1;
        }
    }
    else if(check(S) == 1)
    {   return 1;
    }
    return 0;
}
int alloc_values(int S[9][9], int **H_arr, int *V_arr[9], int *B_arr[3], int x, int y)
{
    int k, l;
    *H_arr = S[x];
    for (k=0; k<9; k++)
    {
        V_arr[k] = &S[k][y];
    }
    k = ((x)/3)*3;
    l = ((y)/3)*3;
    B_arr[0] = &S[k][l];
    B_arr[1] = &S[k+1][l];
    B_arr[2] = &S[k+2][l];
    return 0;
}
int find_at_xy(int S[9][9], const int x, const int y)
{
    int k_put, key;
    int *H_arr = 0, *V_arr[9], *B_arr[3];
    int i, j, m, n, k2_put=1;
    for (key=1; key <= 9; key++)
    {
        k2_put=1;
        alloc_values(S, &H_arr, V_arr, B_arr, x, y);
        k_put = findx(key, H_arr, V_arr, B_arr);
        if (k_put == 1)
        {
            continue;
        }
        //Now we will test the surity of key in for expression  ***S[i][j] = key***
        // For the BOX::
        i = ((x)/3)*3;
        j = ((y)/3)*3;      // i,j = position of begining of BOX...
        for (m = i; m< i+3 && k2_put==1; m++)
        {
            for (n = j; n < j+3 && k2_put==1; n++)
            {
                if ((m != x || n != y) && (S[m][n] == 0))
                {
                    alloc_values(S, &H_arr, V_arr, B_arr, m, n);
                    k2_put = findx(key, H_arr, V_arr, B_arr);
                }
            }
        }
        if (k_put==0 && k2_put==1)
        {
            return key;
        }
        k2_put = 1;
        // For the Vertical line::
        for (m = 0; m< 9 && k2_put==1; m++)
        {
            if ((m != x) && (S[m][y] == 0))
            {
                alloc_values(S, &H_arr, V_arr, B_arr, m, y);
                k2_put = findx(key, H_arr, V_arr, B_arr);
            }
        }
        if (k_put==0 && k2_put==1)
        {
            return key;
        }
        k2_put = 1;
        // For the Horizontal line::
        for (m = 0; m< 9 && k2_put==1; m++)
        {
            if ((m != y) && (S[x][m] == 0))
            {
                alloc_values(S, &H_arr, V_arr, B_arr, x, m);
                k2_put = findx(key, H_arr, V_arr, B_arr);
            }
        }
        if (k_put==0 && k2_put==1)
        {
            return key;
        }
    }
    return 0;
}
int findx(int x, int *H_arr, int *V_arr[9], int *B_arr[3])
{
    int i, j, find = 0;
    for (i = 0; i<9; i++)
    {
        if (H_arr[i] == x)
        {
            find = 1;
            break;
        }
    }
    for (i = 0; i<9 && find != 1; i++)
    {
        if (*V_arr[i] == x)
        {
            find = 1;
            break;
        }
    }
    for (i=0; i<3 && find != 1; i++)
    {
        for (j=0; j<3; j++)
        {
            if (*(B_arr[i]+j) == x)
            {
                find = 1;
            }
        }
    }
    return find;
}
int check(int S[9][9]) 
{
    int i, j, find_0 = 0;
    for (i=0; i<9 && find_0 != 1; i++)
    {
        for (j=0; j<9; j++)
        {
            if ( S[i][j] == 0)
            {
                find_0 = 1;
            }
        }
    }
    return find_0;
}
int compare(int S1[9][9], int S2[9][9])
{
    int i, j;
    for (i = 0; i<9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (S1[i][j] != S2[i][j])
            {
                return 1;
            }
        }
    }
    return 0;
}
int guess_n_solve(int S[9][9])
{   int i, j, x, key, k_put;
    int *H_arr = 0, *V_arr[9], *B_arr[3];
    int S2[9][9];
    for (i = 0; i<9; i++)
    {   for (j=0; j<9; j++)
        {   S2[i][j] = S[i][j];
        }
    }
    i = j = x = key = 0;
    while ( check(S2) == 1)
    {   for ( i ; i<9; i++)
        {   if (i != x)
            {   j = 0;
            }
            for ( j ; j<9; j++)
            {   if (S2[i][j] == 0)
                {   break;
                }
            }
            if (j != 9 && S2[i][j] == 0)
            {   break;
            }
        }
        if (i >= 9)
        {   return 1;
        }
        for(key=1; key <=9; key++)
        {   alloc_values(S2, &H_arr, V_arr, B_arr, i, j);
            k_put = findx(key, H_arr, V_arr, B_arr);
            if (k_put == 1)
            {
                continue;
            }
            S2[i][j] = key;
            if(solve(S2, 1) == 0)
            {   for(i = 0; i<9; i++)
                {   for(j = 0; j < 9; j++)
                    {   S[i][j] = S2[i][j];
                    }
                }
                return 0;
            }
            else
            {   int i, j;
                for(i = 0; i<9; i++)
                {   for(j = 0; j < 9; j++)
                    {   S2[i][j] = S[i][j];
                    }
                }
            }
        }
        x = i;	//Position of Zero = i,j == x, j
        j++;
    }
    return 0;
}
int is_correct(int S[9][9])      // Yet to be tested...
{   int i, j, key, *H_arr, *V_arr[9], *B_arr[3];
    for (i = 0; i < 9; i++)
    {   for (j = 0; j < 9; j++)
        {   key = S[i][j];
            S[i][j] = 0;
            alloc_values(S, &H_arr, V_arr, B_arr, i, j);
            if (findx(key, H_arr, V_arr, B_arr) == 1)
            {   S[i][j] = key;
                return 1;
            }
            S[i][j] = key;
        }
    }
    return 0;
}
int add_2_database(void)
{   int i=0, j=0,k=0,x=0, no_of_game=0, S[9][9], S2[9][9];
    char new_s[250], c;
    FILE *fp;
    fopen("database.rs", "a"); //dummy line for error correction!!
    if((fp = fopen("database.rs", "r")) == 0)
    {   no_of_game = 0;
    }
    else
    {   fseek(fp, 0, SEEK_END);
        no_of_game = (ftell(fp))/89;
    }
    fclose(fp);
    printf("\nInput the Sudoku numbers (row wise, 0 for null value):\n");
    i = 0;
    j = 0;
    while(i>=0 && i<9)
    {   scanf(" %s", new_s);
        k = 0;
        while(new_s[k] != '\0')
        {   if(new_s[k]>='0' && new_s[k]<='9')
            {   if(i < 9)
                {   if(i != x)
                    {   j = 0;
                    }
                    if(j < 9)
                    {   S[i][j] = new_s[k] - '0';
                        j++;
                        if(j == 9)
                        {   j = 0;
                            i++;
                        }
                    }
                }
                if(i != 9)
                {   x=i;
                }
            }
            k++;
        }
    }
    printf("\nYou have entered the following Sudoku:\n");
    print_s(S);
    for(i=0; i<9; i++)
    {   for(j=0; j<9; j++)
        {   if(S[i][j]<0 || S[i][j]>9)
            {   S[i][j] = 0;
            }
            S2[i][j] = S[i][j];
        }
    }
    solve(S2, 0);
    if(check(S2)==0 && is_correct(S2)==0)
    {   printf(" \nSave the Sudoku? (y/n) ");
        fflush(stdin);
        scanf(" %c", &c);
        if(c=='y' || c=='Y')
        {   fp = fopen("database.rs", "a");
            fprintf( fp, " %5d ", ++no_of_game);
            for(i=0; i<9; i++)
            {   for(j=0; j<9; j++)
                {   fputc(S[i][j] + '0', fp);
                }
            }
            fputc('\n', fp);
            fclose(fp);
            printf("\nSave Successful.\n");
        }
        return 0;
    }
    else
    {   printf("\nThe Sudoku entered is incorrect or not Solvable.\n");
    }
    return 1;
}
int game_4m_database(const int game_no, int S[9][9])
{   int i=0, j=0,k=0,x=0, S2[9][9];
    char new_s[100];
    FILE *fp;
    fopen("database.rs", "a"); //dummy line to eliminate error if file does not exists..
    fp = fopen("database.rs", "r");
/*    {   printf("\nError opening the Database..\n");
        return 1;
    }
    //else */
    {   while( fscanf(fp, " %d %s", &i, new_s))
        {   if(i==game_no || i==EOF || i=='\0')
            {   if(i == game_no)
                {   printf("\ni == Game_No\n");
                }
                break;
            }
        }
    }
/*    if(i != game_no)
    {   printf("\nError opening game no - %d\n", game_no);
    }
*/
    for( i=0, k=0; i < 9; i++)
    {   for( j = 0; j < 9; j++)
        {   S[i][j] = new_s[k++] - '0';
        }
    }
    for(i=0; i<9; i++)
    {   for(j=0; j<9; j++)
        {   if(S[i][j]<0 || S[i][j]>9)
            {   S[i][j] = 0;
            }
            S2[i][j] = S[i][j];
        }
    }
    solve(S2, 0);
    if(check(S2)==0 && is_correct(S2)==0)
    {   return 0;
    }
    return 1;
}
