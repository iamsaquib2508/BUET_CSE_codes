#ifndef ACCESSORIES_H_INCLUDED
#define ACCESSORIES_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Screen Generator.h"
int high_score[5];
int cur_score;
int first=0,ind,temp,change=0;
int bil=0;
char to[100][200];
int choice=10,counte=0;
struct rect
{
    int x1, x2, y1, y2;
} ;

typedef struct rect rc;

rc play, ins, quit;

rc cover[4];



int mini(int a,int b)
{
    return a<b?a:b;
}

void cShow()
{

    int i;

    iSetColor(255,255,255);
    iFilledRectangle(0,0,1340,700);
    iSetColor(255,10,10);
    iFilledCircle(10,0, 40);
    iFilledCircle(1340,0, 40);
    iFilledCircle(1340,700, 40);
    iFilledCircle(10,700, 40);
    cover[0].x1=550;
    cover[0].x2=900;
    cover[0].y1=500;
    cover[0].y2=600;
    cover[1].x1=550;
    cover[1].x2=900;
    cover[1].y1=300;
    cover[1].y2=400;
    cover[2].x1=550;
    cover[2].x2=900;
    cover[2].y1=100;
    cover[2].y2=200;
    cover[3].x1=550;
    cover[3].x2=900;
    cover[3].y1=100;
    cover[3].y2=200;
    iSetColor(247,62,9);

    //iText(670,550,"START");
    for(i=0; i<3; i++)
    {
        if(i==0) iSetColor(32,238,249);
        if(i==1) iSetColor(105,252,123);
        if(i==2) iSetColor(248,38,33);
        iFilledRectangle(cover[i].x1,cover[i].y1,cover[i].x2-cover[i].x1,cover[i].y2-cover[i].y1);
    }
    // if(bil)
    {
        iSetColor(0,0,0);
        iText(680,540,"START");
        iSetColor(0,0,0);
        iText(650,340,"INSTRUCTION");
        iSetColor(0,0,0);
        iText(690,140,"QUIT");
    }

    // show play ins quit

}


int show_score()
{

    int i;
    FILE *fp=fopen("save_score.txt","r");
    for(i=0; i<5; i++)
        fscanf(fp,"%d",&high_score[i]);

    fclose(fp);

    fp=fopen("save_score.txt","w");

    if(cur_score>=high_score[ind])
    {
        if(change==0)
        {
            high_score[ind]=cur_score;
            change=0;
        }


        else if(change==1)
        {
            temp=high_score[ind];
            high_score[ind]=cur_score;
            high_score[ind+1]=temp;
            change=0;
        }

        if( ind!=0 && cur_score>=high_score[ind-1])
        {
            ind--;
            change=1;

        }

    }


    for(i=0; i<5; i++)
        fprintf(fp,"%d\n",high_score[i]);
    fclose(fp);






    return 0;

}

void accessories()
{

    show_score();
    //iSetColor(0, 0, 0);
    //iFilledRectangle(0,0,1040,940);
    iSetColor(255, 100, 10);
    iFilledRectangle(1040,0,10,940);
    iSetColor(128,0,0);
    iFilledRectangle(1050,0,300,940);
    iSetColor(249, 6, 146);
    iText(1060,665," HIGH SCORE ");
    int i,j;
    char cur[10],high[10];
    for(i=0; i<5; i++)
    {
        sprintf(high,"%d",high_score[i]);
        iText(1070,620-i*30,high);
    }



    iSetColor(128, 255, 255);
    iText(1060,450," YOUR SCORE ");

    sprintf(cur,"%d",cur_score);
    iText(1070,420,cur);
    iSetColor(14,139,241);
    iText(1060,370," CHANCES REMAINING ");

    for(i=0; i<mini(life,3); i++)
    {
        iSetColor(0,255,0);
        iFilledCircle(1075+i*50,330,10);
    }
    for(; i<3; i++)
    {
        iSetColor(255,0,0);
        iFilledCircle(1075+i*50,330,10);
    }
    for(j=3; j<life; j++)
    {
        iSetColor(0,255,0);
        iFilledCircle(1075+(j-3)*50,290,10);
    }
    for(; j<5; j++)
    {
        iSetColor(255,0,0);
        iFilledCircle(1075+(j-3)*50,290,10);
    }
    if(life==0)
    {
        if(cur_score==high_score[0])
            iText(450,550,"New High Score!!");
        iSetColor(69,250,214);
        iText(465,450,"GAME OVER");
        iText(450,420,"press 's' to restart");
    }

    iSetColor(222,213,99);
    iText(1060,200," press 'End' to exit");
    iText(1070,170," 'p' to pause");

    iText(1070,140," 'r' to resume");



}
void inst()
{




    FILE *fp=fopen("read","wb");
    char ch[10000],c;
    int k=0;


    FILE *from=fopen("readme.txt","r");


    while(!feof(from))
    {
        fgets(to[k++],10000,from);
        counte++;
    }



}
void insShow()
{
    int k;
    iSetColor(0,255,255);
    iFilledRectangle(0,0,1340,700);

    iSetColor(0,0,0);
    for(k=0; k<counte; k++)
    {
        iText(10,600-k*25,to[k]);


    }
    iSetColor(222,187,68);
    iFilledRectangle(cover[3].x1,cover[3].y1,cover[3].x2-cover[3].x1,cover[3].y2-cover[3].y1);
    iSetColor(0,0,0);
    iText(700,140,"BACK");
}
#endif // ACCESSORIES_H_INCLUDED
