#include<stdio.h>
#include<stdlib.h>
#include "Screen Generator.cpp"
int high_score,cur_score;

int maxi(int a,int b)
{
    return a>b?a:b;
}

int show_score()
{

    FILE *fp=fopen("save_score.txt","r");
    fscanf(fp,"%d",&high_score);
    high_score=maxi(high_score,cur_score);
    rewind(fp);
    if( cur_score>=high_score )
    {
        fprintf(fp,"%d",cur_score);
        fclose(fp);


    }
    return 0;

}

void accessories()
{
    show_score();
    iSetColor(255, 100, 10);
    iFilledRectangle(1500,0,10,940);
    iSetColor(249, 6, 146);
    iText(1520,840," HIGH SCORE ");
    int i;
    char cur[10],high[10];
    sprintf(high,"%d",high_score);

    iText(1525,810,high);
    iSetColor(128, 255, 255);
    iText(1520,760," YOUR SCORE ");
    sprintf(cur,"%d",cur_score);
    iText(1525,730,cur);
    iSetColor(14,139,241);
    iText(1520,690," CHANCES REMAINING ");

    for(i=0;i<life;i++)
    {
        iSetColor(0,255,0);
        iFilledCircle(1535+i*50,640,10);
    }

    if(life<=0)
    {
        iSetColor(69,250,214);
        iText(620,350,"GAME OVER");
        iText(600,300,"press 's' to restart");
    }
    for(;i<3;i++)
    {
        iSetColor(255,0,0);
        iFilledCircle(1535+i*50,640,10);
    }
    iSetColor(222,213,99);
    iText(1520,250," press 'End' to exit");
    iText(1530,230," 'p' to pause");

    iText(1530,210," 'r' to resume");



}



