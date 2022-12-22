struct memx
{
	int  x1, x2, y1, y2;
	void read(char *s, int a[]);
	void reset()
	{
		x1=x2=y1=y2=0;
	}
}mem;
int rd;
void memx::read(char *s, int a[])
{
	ifstream g(s, ios::binary);
	while(g.read((char*)&mem, sizeof(mem)))
	{
		if(y1>49)
		{
			if(y1==y2) bar(x1 - 7, y1 - 7, x2+7, y1 + 7);
				else line(x1,y1,x2,y2);
		}
		if(rd==0)
		{
			if(rd<3) rd++;
			else
			{
				if(y1>49)
				{
					a[0] = x1;
					a[1] = y1-50;
				}
				else
				{
					a[0] = x2;
					a[1] = y2-50;
				}
			}
		}
	}
	if(a[0] == 0)
	{
		a[0] = x1;
		a[1] = y1-50;
	}
}
void l_draw(char *s, int a[])
{
	mem.reset();
	mem.read(s,a);
	mem.reset();
}

