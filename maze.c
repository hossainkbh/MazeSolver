#include <stdio.h>
#include <stdlib.h>
int R, C;
char m[101][101];
void carve(int r, int c)
{
    int d[4] = {0, 1, 2, 3};
    int dr[4] = {-2, 2, 0, 0};
    int dc[4] = {0, 0, -2, 2};
    int i, j, t, nr, nc;
    m[r][c] = ' ';
    for(i = 3; i > 0; i--)
    {
        j = rand() % (i + 1);
        t = d[i];
        d[i] = d[j];
        d[j] = t;
    }
    for(i = 0; i < 4; i++)
    {
        nr = r + dr[d[i]];
        nc = c + dc[d[i]];
        if(nr > 0 && nr < R - 1 && nc > 0 && nc < C - 1 && m[nr][nc] == '#')
        {
            m[r + dr[d[i]] / 2][c + dc[d[i]] / 2] = ' ';
            carve(nr, nc);
        }
    }
}
void gen(int r, int c)
{
    int i, j;
    if(r % 2 == 0)
        R = r - 1;
    else
        R = r;
    if(c % 2 == 0)
        C = c - 1;
    else
        C = c;
    for(i = 0; i < R; i++)
    {
        for(j = 0; j < C; j++)
        {
            m[i][j] = '#';
        }
    }
    carve(1, 1);
    m[1][1] = 'S';
    m[R - 2][C - 2] = 'E';
}
void show()
{
    int i, j;
    for(i = 0; i < R; i++)
    {
        for(j = 0; j < C; j++)
        {
            if(m[i][j] == '#')
                printf("\033[1;34m# \033[0m");
            else if(m[i][j] == 'S')
                printf("\033[1;42m S\033[0m");
            else if(m[i][j] == 'E')
                printf("\033[1;41m E\033[0m");
            else if(m[i][j] == '*')
                printf("\033[1;33m* \033[0m");
            else
                printf("  ");
        }
        printf("\n");
    }
}
int solve(int r, int c)
{
    char old;
    if(r < 0 || c < 0 || r >= R || c >= C)
        return 0;
    if(m[r][c] == '#' || m[r][c] == '*')
        return 0;
    if(m[r][c] == 'E')
        return 1;
    old = m[r][c];
    m[r][c] = '*';
    if(solve(r + 1, c) || solve(r, c + 1) || solve(r - 1, c) || solve(r, c - 1))
    {
        if(old == 'S')
            m[r][c] = 'S';
        return 1;
    }
    m[r][c] = old;
    return 0;
}
void saveMaze()
{
    FILE *fp;
    int i, j;
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    fp = fopen(filename, "w");
    if(fp == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }
    fprintf(fp, "%d %d\n", R, C);
    for(i = 0; i < R; i++)
    {
        for(j = 0; j < C; j++)
        {
            fprintf(fp, "%c", m[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Maze saved successfully.\n");
}
void loadMaze()
{
    FILE *fp;
    int i, j;
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }
    fscanf(fp, "%d %d\n", &R, &C);
    for(i = 0; i < R; i++)
    {
        for(j = 0; j < C; j++)
        {
            fscanf(fp, "%c", &m[i][j]);
        }
        fscanf(fp, "\n");
    }
    fclose(fp);
    printf("Maze loaded successfully.\n");
}
int main()
{
    int ch, r, c, i, j;
    do
    {
        printf("\n=== Random Maze Generator & Solver ===\n1. Generate Maze\n2. Solve Maze\n3. Save Maze\n4. Load Maze\n5. Exit\nEnter your choice: ");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                printf("Rows and Columns: ");
                scanf("%d %d", &r, &c);
                gen(r, c);
                show();
                break;
            case 2:
                for(i = 0; i < R; i++)
                {
                    for(j = 0; j < C; j++)
                    {
                        if(m[i][j] == '*')
                            m[i][j] = ' ';
                    }
                }
                if(solve(1, 1))
                    show();
                else
                    printf("No path found.\n");
                break;
            case 3:
                saveMaze();
                break;
            case 4:
                loadMaze();
                show();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(ch != 5);
    return 0;
}
