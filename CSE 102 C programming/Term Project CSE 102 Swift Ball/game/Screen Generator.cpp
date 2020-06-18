
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include "iGraphics.h"

//#include "game/Screen Generator.h"
//#include "game/accessories.h"
#include "accessories.cpp"//600//1200

int wid=1500, high=925, screen[925][1500]= {};
int life, ghy=0, kk;
int ftime=0, score;
int red=1;
//int ball_x, ball_y, rad;
//rec= wid 60, hi 10
//ball= rad 8
//
int kal;
int Vy, Vx=0, VVy=Vy=2;
void drawrec(double x,double y);
void drawspike();
void putball(int x, int y);
void lifestart();
void gamestart();
void gen_last_line();
void checkinball();
void moveball();
void sup();
void gs();
void scr2im();
void test1();
void test2();




/*
process
1. gamestart
2. genlastline
3. putball
4. sup
5. checkinball
6. moveball
7. scr->image + drawspike

1. createimage
2. drawspike


*/


int mmin(int a, int b)
{
    return (a<b)? a:b;
}

int mmax(int a, int b)
{
    return (a>b)? a:b;
}

void drawspike()
{
    for(double j=0; j<wid; j+=20)
    {
        iSetColor(255,0,0);
        iLine(j,high,j+10,high-15);
        iLine(j+10,high-15,j+10,high);
    }
    return;
}

struct ball
{
    int x, y, r;
} b;

void putball(int x, int y)
{
    b.x=x+30;
    b.y=y+b.r+5;
    printf("x %d bx %d y %d by %d\n", x, b.x, y, b.y);
    return;
}

void lifestart()
{
    int i, j;
    kk=life;
    Vx=0;
    printf("%d----\n", kk);
    for(i=0; i<high; i++)
    {
        for(j=0; j<wid; j++)
        {
            if(screen[i][j]==1)
            {
                putball(j,i+250);
                break;
            }
        }
        if(j<wid) break;
    }
    return;
}

void gs()
{
    int j;
    for(j=0;j<wid;j++)
    {
        screen[0][j]=0;

    }
}

void gen_last_line()
{
    int j;


    kal=rand()%1440;
    drawrec(kal,0);
    for(j=0; j<wid; j++)
    {

        if(kal<=j && j<=kal+60)
        {
            //if(red%6==0) screen[0][j]=2, red=1;
            //SSSSSSSSSSSSSSSSelse
            screen[0][j]=1, red++;
            //printf("kkorse---");
        }
        else screen[0][j]=0;
    }

    return;
}

void gamestart()
{
    show_score();
    srand((unsigned)time(0));
    b.r=12;
    life=3;
    //cur_score=10;
    Vx=0;
    iResumeTimer(0);
    screen[925][1490]= {};
    //screen[600][1200]= {};
    //memset(screen,0,sizeof(int)*600*1200);
    gen_last_line();
    putball(kal,500);
    screen[b.y][b.x]=3;
    test1();
    kk=life;

    return;
}

void test1()
{
        lifestart();
        //kk=life;
   // test2();
        return;
}

void test2()
{
    checkinball();
    printf("%d\n\n\n", life);
    if(kk==life)
    {
        //printf("%d\n\n", life);
        moveball();
        scr2im();
    }
    else
    {
        test1();
        return;
    }
}

//int screen index description: 0 >> air, 1 >> bar, 2 >> spike, 3 >> ball



void sup()   //screen up by 1 index
{
    int i, j, temp;
    for(i=high-10; i>=0; i--)
    {
        for(j=0; j<wid; j++)
        {
            if(screen[i][j]!=4)
            screen[i+2][j]=screen[i][j], screen[i+3][j]=screen[i][j], screen[i+5][j]=screen[i][j], screen[i+4][j]=screen[i][j];
        }

    }
    return;
}

void checkinball()
{
    int x, y=b.y-b.r+1;
    //printf("check ");
    for(x=b.x-b.r; x<=(b.r+b.x); x++) //adjustment //done :D //not yet
    {
        if(screen[y][x]==1 || screen[y+1][x]==1 || screen[y+2][x]==1 || screen[y+3][x]==1 || screen[y+4][x]==1) //and final check remaining
        {
            b.y+=6;
            break;
        }
        else if(screen[y][x]==2 || screen[y+1][x]==2 || screen[y+2][x]==2 || screen[y+3][x]==2 || b.y<=b.r || b.y+b.r+15>high)
        {
            life--;
            //break;
            //printf("2222\n");
            break;
        }
    }
    return;
}

void drawrec(double x, double y)
{
    double xx[]= {x,x+60,x+60,x};
    double yy[]= {y,y,y+1,y+1};
    iPolygon(xx,yy,4);
    return;
}



void moveball() //edit
{
    double rat;
    int sy=1, sx=1, f=0;
    int fbx=b.x, fby=b.y;
    rat=Vx/Vy;
    //printf("%lf\n\n", rat);
    {
        while(1 && sy<=Vy)
        {
            //if(screen[mmax(b.y-b.r,0)][b.x]==0)
            rat=Vx/Vy;
            b.y--;
            sy++;
            sx=(int) ((sy-f)*rat);
            b.x=fbx+sx;
            if(b.x<0 || b.x>wid-200)
            {
                Vx=-Vx;
                f=sy;
            }
        }
        b.x+=(Vx-sx);


    }

    return;
}


void scr2im ()
{
    drawspike();
    int i, j;
    for(i=1; i<high; i++)
    {
        for(j=0; j<wid; j++)
        {

            if(screen[i][j]==1)
            {
                iSetColor(0,255,0);
                //printf("hzbrl");
                drawrec(j,i);
                break;
            }

            else if(screen[i][j]==2)
            {
                iSetColor(255,0,0);
                drawrec((double)j,(double)i);
                //printf("--2 er vitor--");
                break;
            }

        }
        //iFilledCircle(j, i, 20);
    }
    iSetColor(180,165,187);
    //b.x=ghy; b.y=100, b.r=5;
    iFilledCircle((double)b.x,(double)b.y,(double)b.r);


    return;
}


