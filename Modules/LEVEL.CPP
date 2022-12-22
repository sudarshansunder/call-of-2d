#include<graphics.h>
#include<math.h>
#include<fstream.h>
#include<conio.h>
#include<ctype.h>
//ob={xh =50,yh =370,50,390,50,410,1,1,0}
struct memx
{
	int  x1, x2, y1, y2;
}mem;
void store()
{
	ofstream f("LVL1.TXT", ios::app|ios::binary);
	f.write((char*)&mem, sizeof(memx));
	f.close();
}
struct mrker
{
	int x, y, r,m;
	mrker()
	{
		x = y = 15;
		r = 7;
	}
	void l_des();
};
void mrker::l_des()
{
	setfillstyle(2,RED);
	setcolor(BLACK);
       //	pieslice(x,y,0,360,r);
	m = 1;			//0 to not print/erase||1 to print
	char ch;
	int c = 0;
	int j = 1;
	do
	{
		setcolor(BLACK);
		int x_p = x, y_p = y;
		ch = getch();
		ch = tolower(ch);
		switch(ch)
		{
			case ' ': m = abs(m-1); j = m;break;
			case 'w': y-=2*r;break;
			case 's': y+=2*r;break;
			case 'a': x-=2*r;break;
			case 'd': x+=r*2;break;
			case 'c': cleardevice(); setcolor(YELLOW);line(0,460,640,460); setcolor(BLACK);break;
			default:break;
		}
		if((m==0)&&(c==0)&&(ch != 'x'))
		{
			setfillstyle(1,BLACK);
			pieslice(x_p,y_p,0,360,r);
			setfillstyle(1,RED);
			pieslice(x,y,0,360,r);
		}
		else if((m==0)&&(ch!='x'))
		{
			 c = m;
			// cout<<m<<' '<<x<<' '<<y;
			 mem.x2 = x;
			 mem.y2 = y;
			 store();
		}
		else
		{
			c = 1;
			//cout<<c;
			if(ch=='x' && j!=0)
			{
			       //	cout<<x<<' '<<y;
				mem.x2 = x;
				mem.y2 = y;
				store();
			}
			if(j == 1)
			{
			       //	cout<<x<<' '<<y;
				mem.x1 = x;
				mem.y1 = y;
				j++;
			}
			if( ch != 'x')
			{
				setfillstyle(2,RED);
				pieslice(x,y,0,360,r);
			}
		}

	}while(ch!='x');
}


void main()
{
	int gd = 9;
	int gm = 2;
	initgraph(&gd,&gm,"C:\\TC\\BGI\\");
	cleardevice();
	setcolor(YELLOW);
	line(0,460,640,460);
	mrker ob;
	ob.l_des();
	getch();
	closegraph();
}