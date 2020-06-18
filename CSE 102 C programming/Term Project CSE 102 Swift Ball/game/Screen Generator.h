#ifndef SCREEN_GENERATOR_H_INCLUDED
#define SCREEN_GENERATOR_H_INCLUDED

#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>


int wid=1040, high=700, screen[687][1040]= {};
int life, ghy=0, kk;
int ftime=1, score;

int red, p, paused;
double sc_rate=1;
int red_den=15, buden;
int highsc=0;
int kal;
int Vy, Vx=0, VVy=Vy=2;
double Vb;
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
void bullet(int k);
void mvbullet();


void drawDiamond(double  x,double y)
{
    double xxx[4]= {x+7,x,x-7,x};
    double yyy[4]= {y,y-7,y,y+7};
    //iSetColor()
    iFilledPolygon(xxx,yyy,4);

}


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


void putd()
{
    int i,j,t=0;
    for(i=0; i<high-15; i++)
    {
        for(j=0; j<wid; j++)
        {
            if(screen[i][j]==1)
            {
                t++;
                if(t==((p-1)*2+1))
                    {
                        screen[i+12][j+30]=4;
                        /*screen[i+12][j+29]=4;
                        screen[i+12][j+28]=4;
                        screen[i+12][j+27]=4;
                        screen[i+12][j+26]=4;
                        screen[i+12][j+31]=4;
                        screen[i+12][j+32]=4;
                        screen[i+12][j+33]=4;
                        screen[i+12][j+34]=4;
                        screen[i+12][j+25]=4;
                        screen[i+12][j+35]=4;*/
                    }
                break;
            }
        }
        if(t==((p-1)*2+1) && j<wid) break;
    }







    return;
}

struct piir
{
    int x, y;
};
typedef struct piir pii;

pii bus[20];


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
} b, bul;

void putball(int x, int y)
{
    b.x=x+30;
    b.y=y+b.r+5;
    return;
}



void lifestart()
{

    int i, j, t=0;
    kk=life;
    Vx=0;
    if(life)
        for(i=0; i<high-15; i++)
        {
            for(j=0; j<wid; j++)
            {
                if(screen[i][j]==1)
                {
                    t++;
                    if(t==((p-1)*5+1))
                        putball(j,i);
                    break;
                }
            }
            if(t==((p-1)*12+1) && j<wid) break;
        }
    return;
}

void drawb(int x, int y)
{
    iSetColor(255,0,0);
    double xp[]={x+bul.r, x, x-bul.r};
    double yp[]={y, y+bul.r, y};
    iFilledPolygon(xp, yp, 3);
    iFilledRectangle(x-bul.r, y-20, bul.r+bul.r, 20);
    return;
}

void putb(int x, int y)
{
    //int y=30, x=b.x,
    //int l, p;
    bus[bus[0].x+1].x=x;
    bus[bus[0].x+1].y=y;
    bus[0].x++;
    bullet(1);
    return;
}

void gs()
{
    int j;
    for(j=0; j<wid; j++)
    {
        screen[0][j]=0;

    }
}

void gen_last_line()
{
    int j;


    kal=rand()%(wid-60);
    drawrec(kal,0);
    for(j=0; j<wid; j++)
    {

        if(kal<=j && j<kal+60)
        {
            if(red%(red_den)==0)
            {
                screen[0][j]=2;

            }
            else
            {
                screen[0][j]=1;
            }
        }
        else screen[0][j]=0;
    }
    red++;
    if(red>60000) red=0;

    return;

}

void gamestart()
{

    if(ftime==0) //call someone;
        srand((unsigned)time(0));
    b.r=10, bul.r=7;
    memset(bus,0,sizeof(bus));
    life=1, bus[0].x=0;
    buden=300;  Vb=2, Vy=2;
    p=8, highsc=0, sc_rate=1;
    red=1, red_den=15;
    Vx=0;

    iResumeTimer(0);
    PlaySound("drm64_wario5.wav", NULL, SND_ASYNC);
    screen[687][1040]= {};
    memset(screen,0,sizeof(screen));
    gen_last_line();
    putball(kal,500);
    screen[b.y][b.x]=3;
    lifestart();
    kk=life;

    return;
}

void test2()
{
    checkinball();

    if(kk<=life)
    {

        moveball();
        scr2im();
    }
    else
    {
        lifestart();
        return;
    }
}

void sup()
{
    int i, j, temp;
    for(i=high-15-4; i>=0; i--)
    {
        for(j=0; j<wid; j++)
        {

            screen[i+2][j]=screen[i][j], screen[i+3][j]=screen[i][j], screen[i+5][j]=screen[i][j], screen[i+4][j]=screen[i][j];
        }

    }
    return;
}

void checkinball()
{
    int x, y=b.y-b.r+1, fo=0, ki;
    for(x=b.x-b.r; x<=(b.r+b.x); x++)
    {
        if(screen[y][x]==1 || screen[y+1][x]==1 || screen[y+2][x]==1 || screen[y+3][x]==1 || screen[y+4][x]==1)
        {
            b.y+=6;
            break;
        }
        else if(screen[y][x]==2 || screen[y+1][x]==2 || screen[y+2][x]==2 || screen[y+3][x]==2 || b.y<=b.r || b.y+b.r+15>high)
        {
            if(life==1)
            {
                PlaySound("smb_gameover.wav", NULL, SND_ASYNC);


                    //ms();
            }

            life--;

            break;

        }
        else
        {
            ki=y-1;
            for(;ki<=b.r+b.y;ki++)
            {
                if(screen[ki][x]==5)
                {



                    if(life==1)
                    {
                        PlaySound("smb_gameover.wav", NULL, SND_ASYNC);


                    //ms();
                    }
                    life--;

                    fo=1;
                    break;
                }
            }
        }
        if(fo) break;
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
    if(Vy)
        rat=Vx/(double)Vy;
    {
        while(1 && sy<=Vy)
        {
            if(Vy)
                rat=Vx/Vy;
            b.y--;
            sy++;
            sx=(int) ((sy-f)*rat);
            b.x=fbx+sx;
            if(b.x<b.r || b.x>wid-b.r)
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
    for(i=1; i<high-15; i++)
    {
        for(j=0; j<wid-60; j++)
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
            else if(screen[i][j]==4)
            {
                iSetColor(237,249,6);
                drawDiamond(j,i);
            }

        }
        //iFilledCircle(j, i, 20);
    }
    if(screen[b.y][b.x]==4 || screen[b.y][b.x+1]==4  || screen[b.y][b.x+2]==4 || screen[b.y][b.x+3]==4 ||
       screen[b.y][b.x-1]==4 || screen[b.y][b.x-2]==4  || screen[b.y][b.x-3]==4 || screen[b.y][b.x-4]==4  ||
       screen[b.y+1][b.x]==4  )
    {
        life=mmin(++life,5);
        screen[b.y][b.x]=0;
        screen[b.y][b.x+1]=0;
        screen[b.y][b.x+2]=0;
        screen[b.y][b.x+3]=0;
        screen[b.y][b.x-1]=0;
        screen[b.y][b.x-2]=0;
        screen[b.y][b.x-3]=0;
        screen[b.y][b.x-4]=0;
        screen[b.y+1][b.x]=0;
        kk=life;
    }
    iSetColor(13,242,231);
    //b.x=ghy; b.y=100, b.r=5;
    iFilledCircle((double)b.x,(double)b.y,(double)b.r);

    bullet(1);
    for(i=1; i<=bus[0].x; i++)
    {

        drawb(bus[i].x,bus[i].y-bul.r);
    }


    return;
}


void bullet(int k)
{
    int i, j, l, p;
    if(k==0)
    {
        for(i=1;i<=bus[0].x;i++)
        {
            bus[i].x=bus[i+1].x;
            bus[i].y=bus[i+1].y;
        }
        bus[0].x --;
    }
    else if(k==1)
    {
        for(i=1; i<=bus[0].x; i++)
        {
            int x=bus[i].x, y=bus[i].y;
            for(l=bul.r; l>=0; l--)
            {
                for(p=0; p<=(l+l); p++)
                    //if(screen[y+5-l][x-l+p]==0)
                    screen[y+5-l][x-l+p]=5;
                    //else screen[y+5-l+50][x-l+p]=5;
            }
            for(l=y; l<=(y-20); l--)
            {
                for(p=x-bul.r; p<=(x+bul.r); p++)
                {
                    //if(screen[l][p]==0)
                    screen[l][p]=5;
                    //else screen[l+50][p]=5;
                }
            }
        }
    }
    return;
}



void mvbullet()
{
    int sy=1, i;
    while(sy<=Vb)
    {
        for(i=1;i<=bus[0].x;i++)
        {
            bus[i].y ++;


            if(bus[i].y>high-13)
            {
                bullet(0);
            }
        }
        sy++;
    }
}




#endif // SCREEN_GENERATOR_H_INCLUDED
