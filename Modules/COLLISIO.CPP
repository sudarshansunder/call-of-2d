#include<all.h>
struct
{
	int x,y;
}a={30,240},b={610,240};
void main()
{
	clrscr();
	int gd=9,gm=2;
	initgraph(&gd,&gm,"C:\\TC\\BGI");
	setfillstyle(1,WHITE);
	circle(a.x,a.y,10);
	floodfill(a.x,a.y,WHITE);
	setfillstyle(1,RED);
	circle(b.x,b.y,10);
	floodfill(b.x,b.y,WHITE);
	getch();
	while((a.x+10)!=(b.x-10))
	{
		setfillstyle(1,BLACK);
		circle(a.x-1,a.y,10);
		floodfill(a.x-1,a.y,BLACK);
		setfillstyle(1,BLACK);
		circle(b.x+1,b.y,10);
		floodfill(b.x+1,b.y,BLACK);
		a.x++;b.x--;
		setfillstyle(1,WHITE);
		circle(a.x,a.y,10);
		floodfill(a.x,a.y,WHITE);
		setfillstyle(1,RED);
		circle(b.x,b.y,10);
		floodfill(b.x,b.y,WHITE);
		delay(2);
	}
	while((a.x>=100)&&(b.x<=540))
	{
		setfillstyle(1,BLACK);
		circle(a.x-1,a.y,10);
		floodfill(a.x-1,a.y,BLACK);
		setfillstyle(1,BLACK);
		circle(b.x+1,b.y,10);
		floodfill(b.x+1,b.y,BLACK);
		setfillstyle(1,WHITE);
		circle(a.x,a.y,10);
		floodfill(a.x,a.y,WHITE);
		setfillstyle(1,RED);
		circle(b.x,b.y,10);
		floodfill(b.x,b.y,WHITE);
		a.x--;b.x++;
		delay(5);
	}


	getch();
	closegraph();
}

