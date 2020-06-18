#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
//#include "iGraphics.h"
#include "iGraphics2.h"

#include "game/Screen Generator.h"
#include "game/accessories.h"


int x = 300, y = 300, r = 20;
void call();

texture birdtexture;

void iTextureInit()
{
    char a[]="smurf.bmp";
    birdtexture.Create(a);
}

void ms()
{
    highsc=1;
    //printf("22\n");
    PlaySound("sm64_high_score.wav", NULL, SND_ASYNC);
    return;
}
int cx, cy, cby;

void iDraw()
{
    iClear();

    if(ftime==0)
    {
        scr2im();
        accessories();
        gs();
       // mvbullet();
        if(cur_score==high_score[0] && highsc==0 && life==0) ms();
        call();
        if(life==0)
        {
            iPauseTimer(0);
        }


        else if(life>0)
        {

            if(!paused)
                cur_score+=sc_rate;
            if(cur_score%500==0) red_den=mmax(--red_den,3) ;
            if(cur_score%1000==0) sc_rate+=.5;
            if(cur_score%200==0) putd();
            //if(cur_score%(300)==0) putb(b.x, 30), buden=mmax(--buden,100);
            test2();
        }
        drawTexture(100,100,birdtexture);

    }

    if(ftime==1)
    {
        iSetColor(233,45,56);
        cShow();


    }
    if(choice==2)
    {
        insShow();
    }






}
int inRange(int mx,int my)
{
    int i;
    for(i=0; i<3; i++)
    {

        if(choice!=2)
            if(cover[i].x1<=mx && mx<=cover[i].x2 && cover[i].y1<=my && my<=cover[i].y2 ) return i+1;
    }
    if( choice==2 && cover[i].x1<=mx && mx<=cover[i].x2 && cover[i].y1<=my && my<=cover[i].y2 ) return i+1;
    return 0;
}
/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
    printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && 1)
    {

        int k=inRange(mx,my);

        if(k==1 && choice!=2 && choice!=1)
        {
            choice=1;
            ftime=0;
            gamestart();
            iSetTimer(50,sup);



            iSetTimer(140,gen_last_line);
        }
        else if(k==2 && choice!=2 && choice!=1)
        {

            choice=2;

            //instruction
        }
        else if(k==3 && choice!=2 && choice!=1 )
        {
            choice=3;
            exit(0);
        }
        else if(k==4 )
        {
            choice=4;
            PlaySound("msatl2012og-3ds_home_menu_icon.wav", NULL, SND_ASYNC );
            ftime=1;
        }

    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

        x -= 10;
        y -= 10;
    }
}


void iKeyboard(unsigned char key)
{

    if (key == 'q')
    {

        exit(0);
    }
    if (key == 'p' )
    {
        paused=1;
        cx=Vx, cy=Vy, cby=Vb;
        Vx=1;
        Vy=Vb=0;
        iPauseTimer(0);
    }
    if (key == 'r' && paused)
    {

        if(paused) Vx=cx, Vy=cy, Vb=cby;
        paused=0;

        iResumeTimer(0);

    }

    if(key=='s' && life==0)
    {
        //call();
        show_score();
        cur_score=0;
        first=0,ind=4,change=0;
        gamestart();

    }

}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if(key==GLUT_KEY_RIGHT && !paused)
    {
        if(Vx>=0)
            Vx=mmin(++Vx,6);
        else Vx=0;
        //b.x+=(Vx);
    }
    if(key==GLUT_KEY_LEFT && !paused)
    {
        if(Vx<=0)
            Vx=mmax(--Vx,-6);
        else Vx=0;
        //b.x-=(Vx);
    }
    if(key==GLUT_KEY_UP)
    {
        //b.y+=8;
    }
    if(key==GLUT_KEY_DOWN)
    {
        //b.y-=8;
    }


}
void call()
{


    show_score();
}

int main()
{

    inst();
    if(ftime==1)
        PlaySound("msatl2012og-3ds_home_menu_icon.wav", NULL, SND_ASYNC );
    life=0;
    //iTextureInit();

    ind=4;

    if(!ftime)
        gamestart();
    iInitialize(1340, 700, "Swift Ball");





    return 0;
}
