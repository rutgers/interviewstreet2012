#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) { printf("FAIL\n"); exit(1); }
    //return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

void insertEdge(int r, int c, int player, int raw_[11][11])
{
    int bw_, bh_ = bw_ = 11;
    int rOdd = r & 1;
    int cOdd = c & 1;
    /* Vertical Slice */
    raw_[r][c] = player;
    if (rOdd && !cOdd)
    {
        if (c == 0)
        {
            if (raw_[r][c+2] && raw_[r-1][c+1] && raw_[r+1][c+1])
                raw_[r][c+1] = player;
        }
        else if (c == bw_)
        {
            if (raw_[r][c-2] && raw_[r-1][c-1] && raw_[r+1][c-1])
                raw_[r][c-1] = player;
        }
        else
        {
            if (raw_[r][c+2] && raw_[r-1][c+1] && raw_[r+1][c+1])
                raw_[r][c+1] = player;
            if (raw_[r][c-2] && raw_[r-1][c-1] && raw_[r+1][c-1])
                raw_[r][c-1] = player;
        }
    }
    /* Horizontal Slice */
    else if (!rOdd && cOdd)
    {
        if (r == 0)
        {
            if (raw_[r+2][c] && raw_[r+1][c-1] && raw_[r+1][c+1])
                raw_[r+1][c] = player;
        }
        else if (r == bh_)
        {
            if (raw_[r-2][c] && raw_[r-1][c-1] && raw_[r-1][c+1])
                raw_[r-1][c] = player;
        }
        else
        {
            if (raw_[r+2][c] && raw_[r+1][c-1] && raw_[r+1][c+1])
                raw_[r+1][c] = player;
            if (raw_[r-2][c] && raw_[r-1][c-1] && raw_[r-1][c+1])
                raw_[r-1][c] = player;
        }
    }
}

int main () {
    int player;
    int array[11][11];
    int r, c;
    char buf[32];
    int check;
    int i,o;
    FILE *fp = fopen("in1.txt","r");
    fscanf(fp,"%d",&player);
    for (i = 0; i < 11; ++i)
        fscanf(fp,"%d %d %d %d %d %d %d %d %d %d %d",&array[i][0],&array[i][1],&array[i][2],&array[i][3],&array[i][4],&array[i][5],&array[i][6],&array[i][7],&array[i][8],&array[i][9],&array[i][10]);
    printf("%d\n", player);
    for (i = 0; i < 11; ++i) {
        for (int o = 0; o < 11; ++o)
            printf("%d ", array[i][o]);
        puts("");
    }
    puts("\n\n");
    std::string tmp;
    tmp = exec ("./test_in2 < in1.txt");
    sscanf(tmp.c_str(), "%d %d", &r, &c);
    fclose(fp);
    for (;;) {
        cout << tmp;
        insertEdge(r,c,player,array);
        //array[r][c] = player;
        printf("%d\n",player);
        for (i = 0; i < 11; ++i) {
            for (o = 0; o < 11; ++o)
                printf("%d ", array[i][o]);
            puts("");
        }
        player ^= 3;
        fp = fopen("inNEW.txt","w");
        fprintf(fp, "%d\n",player);
        for (i = 0; i < 11; ++i)
            fprintf(fp,"%d %d %d %d %d %d %d %d %d %d %d\n",array[i][0],array[i][1],array[i][2],array[i][3],array[i][4],array[i][5],array[i][6],array[i][7],array[i][8],array[i][9],array[i][10]);
        fclose(fp);
        if (player == 1)
            tmp = exec ("./test_in1 < inNEW.txt");
        else
            tmp = exec ("./test_in2 < inNEW.txt");
        cout << "\n\n\n\n";
        sscanf(tmp.c_str(), "%d %d", &r, &c);
        check = 0;
        for (i = 0; i < 11; ++i)
        for (o = 0; o < 11; ++o)
            if (array[i][o] == 0)
                goto cont;
        if (i == o && o == 11)
            goto exitfile;
        cont:
        scanf("%c",buf); /* wait for input */
    }
    exitfile:
    printf("Goodbye!\n");
    return 0;
}
