#include<fstream.h>
#include<graphics.h>
#include<conio.h>
struct memx
{
	int  x1, x2, y1,y2;
	void read();
}mem;
void memx::read()
{
	int gd = 9;
	int gm = 2;
	initgraph(&gd,&gm,"C:\\TC\\BGI\\");
	ifstream g("LVL1.TXT", ios::binary);
	setfillstyle(1,RED);
	while(g.read((char*)&mem, sizeof(mem)))
	{
		if(y1==y2) bar(x1 - 7, y1 - 7, x2+7, y1 + 7);
			else line(x1,y1,x2,y2);
		getch();
	}
	cout<<"DONE";
	getch();
	closegraph();
}
void main()
{
	mem.read();
}

