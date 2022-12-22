# include"all.h"
# include"L_DRAW.cpp"
# include"L_CREATE.cpp"
# include"game.h"
# include<time.h>
# define drv int gd=DETECT,gm=2;initgraph(&gd,&gm,"C:\\TC\\BGI");
//GLOBAL VARIABLES
int cx,cy,q;
int inv=0,c,lvl=100,_coin=0,xv[5],file=0,temp,pre_exp = 0,pre_lvl = 0;
int c_lvl = 0;
int dif = 1;
int g_o = -1;
int ary[2];
char opt;
/* STRUCTURES AND MEMBER FUNCTIONS */
class mouse
{
		REGS regs;
		int no_buttons;
	public:
		void show_mouse();
		void get_status();
		void hide_mouse();
		mouse()
		{
			regs.x.ax=0;
			int86(0x33,&regs,&regs);
			if(regs.x.ax==0xfff) no_buttons=regs.x.bx;
		}
};
void mouse::show_mouse()
{
	regs.x.ax=0x01;
	int86(0x33,&regs,&regs);
}
void mouse::hide_mouse()
{
	regs.x.ax=0x02;
	int86(0x33,&regs,&regs);
}
void mouse::get_status()
{
	regs.x.ax=0x03;
	int86(0x33,&regs,&regs);
	while(q!=1)
	{
		regs.x.ax=0x03;
		int86(0x33,&regs,&regs);
		q=regs.x.bx;
		cx=regs.x.cx;
		cy=regs.x.dx;
	}
	q=0;
}
mouse m;
struct g_level
{
	int noe;
	int epx,epy;
	int n;
}g_lvl;
struct gun
{
	char n[25];
	int dmg,fm,ul,color,h_color;
};
struct settings
{
	int color;
	int f_color,b_color;
	gun gn;
	char s[50];
}set;
struct cn
{
	int x,y;
	void gen();
	int check_coin();
	void coin(int c = 14);
}cn;
int cn::check_coin()
{
	int c=1;
	if((getpixel(x-9,y)==BLACK)&&(getpixel(x+9,y)==BLACK)&&(getpixel(x,y-9)==BLACK)&&(getpixel(x,y+9)==BLACK)) c=0;
	return c;
}
void cn::gen()
{
	_coin = 0;
	do
	{
		do
		{
			y = random(420);
		}while(y<=280);
		x=random(640);
	}while(check_coin());
}
void cn::coin(int c)
{
	setcolor(c);
	setfillstyle(1,c);
	fillellipse(x,y,8,8);
}
struct score
{
	 char name[50];
	 int kills;
	 int coins;
	 float score;
}scr ={"Player"};
class character
{
	public:
	int xh,yh; // Head
	int xb,yb; // Body
	int xl,yl; // Leg
	int o;
	int lvl;
	int exp;
	int glvl;
	int ammo;
	int cnoe;
	void reset()
	{
		xh = 50;
		yh = 370;
		xb = 50;
		yb = 390;
		xl = 50;
		yl = 410;
		ammo = 45;
		cnoe = 0;
	}
	int coll_leg();
	int coll_body_r();
	int coll_body_l();
	int coll_h_right();
	int coll_h_left();
	int coll_h_top();
	int frontier();
	void m_l(int x=5);
	void m_r(int x=5);
	void m_u();
	void m_d();
	void c_track();
	int exp_calc(int);
	void charac(int c = 2,int x = set.f_color,int x1 = set.b_color,int y=set.gn.color,int z=set.gn.h_color);
}ob={50,370,50,390,50,410,1,1,0,1};
class enemy : public character
{
	public:
	int skill,nob,cnb,arm,p;
	void enem(int x = 3);
	enemy();
	void e_track();
	int damage();
}*e[4];
int character::exp_calc(int i)
{
	int c = 0;
	if(i == 0)
		exp += 10;
	else if(i ==1)
		exp += 50;
	if(exp>=(lvl * 100))
	{
		exp = exp - (lvl * 100);
		lvl++;
		c = 1;
	}
	return c;
}
enemy::enemy()
{
	o = 0;
	cnb = 0;
	int i = 0;
	do
	{
		xb = random(620);
		i++;
	}while((xb>=30)&&(i<=5));
	if(i > 5) xb = 640 - i * (1+random(5));
	xh = xl = xb;
	do
	{
		yh = random(170);
	}while((yh-13)<50);
	yb = yh + 12;
	yl = yb + 12;
	p = i%2;
	//Calculating Damage
	arm = random(20);
	arm += 4 * dif;
	nob = (arm/(set.gn.dmg));
	do
	{
		skill = random(4);  // Damage = Skill x 5
	}while(skill == 0);
}
int e_c = 0,eno = -1;
void enemy::e_track()
{
	while(coll_leg())
	{
		level();
		m_d();
		delay(1);
		if(yh>=450)
		{
			e_c = 0;
			eno = -1;
		}
	}
}
struct game_state
{
	character ob2;
	score ob3;
	gun ob5;
	int cc,f;
	g_level ob6;
};
struct controls
{
	char u,d,sh,r,l,in,ex;
	controls()
	{
		ex = 27;
	}
	void reset_c()
	{
		u = 'W';
		d = 'S';
		l = 'A';
		r = 'D';
		sh = ' ';
		in = 'C';
		ex = char(27);
	}
}ctrl;
//MISC FUNCTIONS NEEDED
void save_control()
{
	ofstream f("control.dat");
	f.write((char*)&ctrl,sizeof(ctrl));
	f.close();
}
void load_controls()
{
	ifstream f("control.dat" );
	f.read((char*)&ctrl,sizeof(ctrl));
	f.close();
}
void save_name()
{
	ofstream f("name.dat");
	f<<scr.name<<'\n';
	f.close();
}
void load_name()
{
	ifstream f("name.dat");
	f.getline(scr.name,1000,'\n');
	f.close();
}
void save_state()
{
	if(temp!=1) file = 1;
	temp = 0;
	game_state ob4;
	ob4.ob2 = ob;
	ob4.ob3 = scr;
	ob4.cc = ::c;
	ob4.ob5 = set.gn;
	ob4.f = file;
	ob4.ob6 = g_lvl;
	ofstream f("save.dat" );
	f.write((char*)&ob4,sizeof(ob4));
	f.close();
}
void load_state()
{
	game_state ob4;
	ifstream f("save.dat" );
	f.read((char*)&ob4,sizeof(ob4));
	ob = ob4.ob2;
	scr = ob4.ob3;
	g_lvl = ob4.ob6;
	::c = ob4.cc;
	set.gn = ob4.ob5;
	file = ob4.f;
	f.close();
}
void writescore()
{
	ofstream f("highscor.dat",ios::app);
	f.write((char*)&scr,sizeof(scr));
	f.close();
}
void clearbuffer()
{
	while(kbhit()) getch();
}
void outcharxy(int x,int y,char ch)
{
	char s[5];
	s[0]=ch;
	s[1]='\0';
	outtextxy(x,y,s);
}
void outintxy(int x,int y,int n,int sz)
{
	settextstyle(0,0,sz);
	char s[10];
	itoa(n,s,10);
	outtextxy(x,y,s);
}
void sort_array(int ob[],int N)
{
	for(int i=0;i<N-1;i++)
	{
		for(int j=0;j<N-i-1;j++)
		{
			if(ob[j]<ob[j+1])
			{
				int t=ob[j];
				ob[j]=ob[j+1];
				ob[j+1]=t;
			}
		}
	}
}

void sort_array(score ob[],int N)
{
	for(int i=0;i<N-1;i++)
	{
		for(int j=0;j<N-i-1;j++)
		{
			if(ob[j].score<ob[j+1].score)
			{
				score t=ob[j];
				ob[j]=ob[j+1];
				ob[j+1]=t;
			}
		}
	}
}
void lf_bar()
{
	setfillstyle(1,LIGHTGREEN);
	bar(470,15,c,25);
	setcolor(GREEN);
	setlinestyle(SOLID_LINE,4,120);
	rectangle(465,10,605,30);
}
void up_lfbar(int n)
{
	if(c>=475)
	{
		setfillstyle(1,BLACK);
		int t=c;
		c+=n;
		bar(t,15,c,25);
	}
	else {
		if(g_o == -1) g_o = 0;
		game_over(0);
	      }
}
void sort_scores()
{
	ifstream f("HIGHSCOR.dat" );
	score ob[10];
	for(int i=0;f.read((char*)&ob[i],sizeof(score));i++);
	i++;
	f.close();
	sort_array(ob,i);
	ofstream g("HIGHSCOR.dat" );
	for(int j=0;j<i-1;j++)
	{
		 g.write((char*)&ob[j],sizeof(score));
	}
	g.close();
}
int chk_pre(int x, int i)
{
	if(i==0)
		return 0;
	else
	{

		i--;
		while(i>=0)
		{
			if(xv[i] == x) return 1;
			i--;
		}
		return 0;
	}
}
void coinsound()
{
	/*for(int i=50;i<1000;i+=300)
	{
		sound(i);
		delay(20);
	}*/
	nosound();
}
void fallsound()
{
	/*for(int i=30;i<500;i+=220)
	{
		sound(i);
		delay(20);
	}*/
	nosound();
}
void hitsound()     //Enemy Banging You
{
	/*for(int i=10000;i>=5000;i-=1000)
	{
		sound(i);
		delay(2);
	}*/
	nosound();
}
void deathsound()      //Enemy Bangs
{
	/*for(int i=1000;i<=5000;i-=50 )
	{
		sound(i);
		delay(2);
	} */
	nosound();
}
void outofammo()
{
	/*for(int i=10000;i>=100;i-=2000 )
	{
		sound(i);
		delay(2);
	}    */
	nosound();
}
void bsound()
{
	/*for(int i=5000;i>=1000;i-=1000 )
	{
		sound(i);
		delay(2);
	}
	nosound();*/
}
void rand_v()
{
	int n;
	do
	{
		n = random(4);
	}while((n==0));
	for(int i = 0; i<n;)
	{
		xv[i] = random(620);
		if(!chk_pre(xv[i],i))
			i++;
	}
	xv[n] = -1;
}
void platform()
{

	setfillstyle(1,8);
	for(int i = 0,j = 0; xv[i]!=-1;i++)
	{

		bar(j,420,xv[i],480);
		j = xv[i]+30;
		if(xv[i+1]==-1)
			bar(j,420,640,480);
	}
}
//COLLISION FUNCTIONS
int character::coll_h_top()
{
	int c=0;
	for(int j=xh-8;j<=xh+8;j++)
	{
		if(getpixel(j, yh-17)!=BLACK)
		{
			c=1;
		}
		if(o==1)
		{
			if(getpixel(j, yh-17)==YELLOW) c=2;

			for(int k=j-3;k<=j+3;k++)
				if(getpixel(k,yh-17) == 2) c = 3;
			if(getpixel(j,yh - 17)==RED) c = 4;
		}
	}
	return  c;
}
int character::coll_h_right()
{
	int c=0;
	for(int j= yh-10;j<= yh+10;j++)
	{
		if(((getpixel(xh+13,j) == GREEN)) && (o==0)) c = 0;
		if(getpixel( xh+13,j)!=BLACK)
		{
			 c=1;
		}
		if(o==1)
		{
			if(getpixel( xh+13,j)==YELLOW) c=2;
			for(int k=xh+10;k<=xh+16;k++)
				if(getpixel(k,j)==GREEN) c=3;
			if(getpixel(xh+13,j)==RED) c = 4;
		}
	}
	return c;
}
int character::coll_h_left()
{
	int c=0;
	for(int j= yh-10;j<= yh+10;j++)
	{
		if(((getpixel(xh-13,j) == GREEN)) && (o==0)) c = 0;
		if(getpixel( xh-13,j)!=BLACK)
		{
			c=1;

		}
		if(o==1)
		{
			if(getpixel( xh-13,j)==YELLOW) c=2;
			for(int k=xh-16;k<=xh-10;k++)
			{
				if(getpixel(k,j) == 2) c = 3;
			}
			if(getpixel(xh-13,j)==RED) c=4;

		}
	}
	return c;
}
int character::coll_body_r()
{
	int c = 0;
	for(int i =  yh+13; (i<= yl);i++)
	{

		if(((getpixel(xb+1,i) == GREEN)) && (o==0)) c = 0;
		if((getpixel( xb+1,i)!=BLACK))
		{
			c = 1;
		}
		if(o==1)
		{
			if(getpixel( xb+1,i)==YELLOW) c=2;
			for(int k=xb-2;k<=xb+4;k++)
				if(getpixel(k,i) == 2) c = 3;
			if(getpixel(xb+1,i)==RED) c=4;
		}
	}
	return c;

}
int character::coll_body_l()
{
	int c = 0;

	for(int i =  yh+16; (i<= yl);i++)
	{
		if(((getpixel(xb-1,i) == GREEN)) && (o==0)) c = 0;
		if((getpixel( xb-1,i)!=BLACK))
		{
			c = 1;
		}
		if(o==1)
		{
			for(int k=xb-4;k<=xb+2;k++)
				if(getpixel(k,i) == 2) c = 3;
			if(getpixel( xb-1,i)==YELLOW) c=2;
			if(getpixel(xb-1,i) == RED) c=4;
		}
	}
	return c;

}
int character::coll_leg()
{
	int c=1;
	if(getpixel( xl, yl+13)!=BLACK)
			c=0;
	if(o == 1)
	{
		if(getpixel( xl, yl+13)==YELLOW) c=2;
		if(getpixel(xl,yl+13)==RED) c=4;
	}
	return c;
}

//GAME OPERATIONS
int enemy::damage()
{
	cnb++;
	int c = 0;
	if(cnb >= nob )
	{
		o = 0;
		charac(0,0,0,0,0);
		platform();
		delete e[eno];
		eno = -1;
		e_c = 0;
		c = 1;
		cnoe ++;
		dif = (dif<4)? dif + 1:
			dif;
	}
	o = 1;
	return c;
}
int check_coll(int ip,int d)
{
	int c = 0;
	d = d*3;
	for(int i = ip;((i<=640)&&(i>=0))&&(!c); i+=d/3)
		if(getpixel((i+d),ob.yb)==2)
			c = i+d;
	return c;
}
void updateammo(int i)
{
	setcolor(BLACK);
	outintxy(330,50,ob.ammo,0);
	delay(10);
	ob.ammo +=i;
	if((ob.ammo <= 22)&&(ob.ammo > 9)) setcolor(12);
	else if(ob.ammo <= 9) setcolor(RED);
	else setcolor(WHITE);
	outintxy(330,50,ob.ammo,0);
}
void shootsound()
{
	/*for(int i=1000;i<=10000;i+=300)
	{
		sound(i);
		delay(2);
	}*/
	nosound();
}
int h = 0;
void flag()
{
	setcolor(RED);
	setfillstyle(11,RED);
	rectangle(ary[0]+1,ary[1]-1,ary[0]+41,ary[1] + 21);
	bar(ary[0]+2,ary[1],ary[0]+40,ary[1]+20);
	line(ary[0],ary[1]-10,ary[0],ary[1]+60);
}
void shoot()
{
	int p;
	p = (inv)? -1:1;
	int ipx = ob.xb + (p*13);
	int cp = check_coll(ipx, p);
	if(ob.ammo == 0)
	{
		 outofammo();
		 h++;
		 if(h == 5)
		 {
			h = 0;
			settextstyle(0,0,0);
			setcolor(WHITE);
			outtextxy(400,70,"Out Of Ammo!");
		 }
		 if(h==3)
		 {
			setcolor(BLACK);
			outtextxy(400,70,"Out Of Ammo!");
		 }
	}
	for(int j=0;(j<set.gn.fm)&&(ob.ammo);j++)
	{
		updateammo(-1);
		shootsound();
		for(int i=ipx;(i>=0)&&(i<=640);i+=(p*13))
		{
			setcolor(9);
			circle(i,ob.yb+5,2);
			delay(1);
			level();
			setcolor(BLACK);
			circle(i,ob.yb+5,2);
			if(((p*(cp - i)) < 13)&&(cp!=0)&&((p*(cp - i))>-1))
			{
				e[eno]->o = 0;
				e[eno]->charac(4);
				delay(30);
				e[eno]->charac();
				if(e[eno]->damage())
				{
					updatescore();
					updateammo(6);
					scr.kills++;
					cp = check_coll(i,p);
					ob.exp_calc(1);
					deathsound();
					ob.cnoe++;
					if(ob.cnoe >= g_lvl.noe)
					{
						flag();
					}
					break;
				}
				e[eno]->o = 1;
				i = -1000;
			}

		}
	}
}
void character::m_r(int i)
{
	if(o!=0)
		if(inv==1)
			invert();
	if((!coll_body_r())&&(!coll_h_right()))
	{
		if( xh+20<640)
		{
			charac(0,0,0,0);
			 xh+=i;
			 xl+=i;
			 xb+=i;
			level();
			platform();
			charac();
		}
		else
		{
			charac(0,0,0,0);
			 xh=17;
			 xl=17;
			 xb=17;
			level();
			platform();
			charac();
		}

	 }
	 if(o!=0)
	 if((coll_body_r()==2)||(coll_h_right()==2))
	 {
		 cn.coin(0);
		 _coin=1;
		 updatecoin();
		 updateammo(3);
		 exp_calc(0);
		 coinsound();
	 }
	 if(o!=0)
		if((coll_body_r()==3)||(coll_h_right()==3)||(coll_h_top()==3))
		 {
			up_lfbar(e[eno]->skill*-15);
			hitsound();
		 }
}
void character::m_l(int i)
{
	if(o!=0)
	if(inv==0)
		invert();
	if((!coll_body_l())&&(!coll_h_left()))
	{
		if( xh-20>0)
		{
			charac(0,0,0,0);
			 xh-=i;
			 xl-=i;
			 xb-=i;
			level();
			platform();
			charac();
		}
		else
		{
			charac(0,0,0,0);
			 xh=623;
			 xl=623;
			 xb=623;
			level();
			platform();
			charac();
		}
	}
	if(o!=0)
	if((coll_body_l()==2)||(coll_h_left()==2))
	 {
		 cn.coin(0);
		 _coin=1;
		 updatecoin();
		 updateammo(3);
		 exp_calc(0);
		 coinsound();
	 }
	 if(o!=0)
		 if((coll_body_l()==3)||(coll_h_left()==3)||(coll_h_top()==3))
		 {
			up_lfbar(e[eno]->skill*-15);
			hitsound();
		 }
}
int character::frontier()
{
	for(int i = yh - 13;i<yl+10;i++)
		if((getpixel(xh+13-((inv-1)*8), i) == 2) || (getpixel(xh-13 - (inv*8), i) == 2))
			return 3;
	return 0;
}
void character::c_track()
{
	int j=0,t,k=0;
	char ch;
	int pos= yl;
	int d = 0;
	while(coll_leg()&&(!d)&&(g_o==0))
	{
		level();
		do
		{
			t=bioskey(1);
			k++;
		}while(!t&&k<5);
		ch=t;
		ch = toupper(ch);
		if(ch==ctrl.l) m_l(2);
		else if(ch==ctrl.r) m_r(2);
		m_d();
		while((yh-12>=480)&&(opt!=27))
		{
			up_lfbar(-5);
			delay(10);
			d = 1;
		}
		if((coll_leg()==2))
		{
			 cn.coin(0);
			 _coin=1;
			 updatecoin();
			 exp_calc(0);
			 updateammo(3);
			 coinsound();
		}
		if((coll_leg()==4)||(coll_h_top()==4)||(coll_h_right()==4)||(coll_h_left()==4)||(coll_body_r()==4)||(coll_body_l()==4))
		{
			game_over(1);
		}
		delay(abs(20-j));
		j++;
	}

		if((( yl+10)-pos)>=140)
		{
			 up_lfbar(-25);
			 fallsound();
		}
}
void character::m_d()
{
	charac(0,0,0,0);
	 yh+=5;
	 yl+=5;
	 yb+=5;
	charac();

}
void character::m_u()
{
	charac(0,0,0,0);
	 yh-=5;
	 yl-=5;
	 yb-=5;
	charac();
}
void sound_jump()
{
	/*for(int i=20;i<1000;i+=75)
	{
		sound(50+i);
		delay(1);
	}*/
	nosound();
}
void jump()
{
	int j=0;
	int t;
	char ch;
	int p=ob.yl-50;
	sound_jump();
	for(int i=0;(i<=30)&&(ob.yl>=p)&&(!ob.coll_h_top());i++)
	{
		level();
		ob.m_u();
		if(ob.coll_h_top()==2)
		{
			 cn.coin(0);
			 _coin=1;
			 updatecoin();
			 updateammo(3);
			 ob.exp_calc(0);
			 coinsound();
		}
	}
	j = 0;
	clearbuffer();
	do
	{
		t = bioskey(1);
		delay(25);
		j++;
	}while(!t && j<4);
	ch = t;
	ch=toupper(ch);
	if(ch== ctrl.l)	 ob.m_l(30);
	else if(ch== ctrl.r) ob.m_r(30);
	else if(ch== ctrl.sh) shoot();
}
int i = 0;
void duck()
{
	if(ob.yb>ob.yl-2)
	{
		while(i>=0)
		{
			ob.charac(0,0,0,0);
			ob.yh-=3;
			ob.yb-=3;
			level();
			ob.charac();
			delay(10);
			i--;
		}
		return;
	}
	while((ob.yb<=ob.yl-2)&&(!ob.coll_h_top()))
	{
		ob.charac(0,0,0,0);
		ob.yh+=3;
		ob.yb+=3;
		level();
		ob.charac();
		delay(10);
		i=6;
	}
}
//GAME UI
void gun_lock(int n)
{
	cleardevice();
	setcolor(RED);
	settextstyle(0,0,2);
	outtextxy(50,220,"This Gun can be unlocked at level ");
	outintxy(350,250,n,2);
	getch();
}
void loadout()
{
	cleardevice();
	setcolor(RED);
	setfillstyle(9,BLUE);
	rectangle(31,160,100,200);
	bar(32,161,99,199);
	rectangle(499,49,611,91);
	bar(500,50,610,90);
	settextstyle(0,0,2);
	outtextxy(35,175,"BACK");
	settextstyle(0,0,1);
	outtextxy(510,68,"CHANGE NAME");
	settextstyle(0,0,3);
	setcolor(WHITE);
	outtextxy(230,50,"LOADOUT");
	setcolor(LIGHTGRAY);
	rectangle(170,120,270,230);
	setcolor(8);
	settextstyle(0,0,2);
	outtextxy(310,170,scr.name);
	settextstyle(0,0,10);
	setcolor(LIGHTBLUE);
	outtextxy(190,290,set.gn.n);
	{
		ob.xh = ob.xb = ob.xl = 220;
		ob.yh = 160;
		ob.yb = 180;
		ob.yl = 200;
	}
	ob.charac();
	char b;
	do
/*500*/	{
		m.show_mouse();
		m.get_status();
		delay(50);
		m.hide_mouse();
			if((cx>=31)&&(cx<=100)&&(cy>=160)&&(cy<=200))
			{
				b='0';
				bsound();
			}
			else if((cx>=500)&&(cx<=610)&&(cy>=50)&&(cy<=90))
			{
				b='0';
				bsound();
				cleardevice();
				delay(500);
				inp_name();
				save_name();
				delay(300);
			}
	}while(b!='0');
	delay(50);
}
void level_scrn_UI()
{
	cleardevice();
	setcolor(RED);
	line(0,30,640,30);
	settextstyle(0,0,2);
	setcolor(CYAN);
	outtextxy(280,10,"LEVELS");
	setcolor(BLUE);
	setfillstyle(11,BLUE);
	bar3d(70,60,200,250,4,1);
	bar3d(250,60,400,250,4,1);
	bar3d(450,60,600,250,4,1);
	bar3d(70,270,200,460,4,1);
	bar3d(250,270,400,460,4,1);
	bar3d(450,270,600,460,4,1);
	bar3d(0,210,40,270,3,1);
	setcolor(WHITE);
	settextstyle(0,1,0);
	outtextxy(25,218,"CUSTOM");
	settextstyle(0,0,12);
	outtextxy(95,105,"1");
	outtextxy(295,105,"2");
	outtextxy(495,105,"3");
	outtextxy(95,315,"4");
	outtextxy(295,315,"5");
	outtextxy(495,315,"6");
	setcolor(BLUE);
	setfillstyle(11,BLUE);
	bar(1,1,37,29);
	rectangle(0,0,38,30);
	settextstyle(0,0,1);
	setcolor(RED);
	outtextxy(5,12,"BACK");
	bar(550,1,635,29);
	rectangle(550,0,635,30);
	outtextxy(575,7,"VIEW");
	outtextxy(565,17,"LOADOUT");
}
void level_scrn()
{
	level_scrn_UI();
	char a1 = '0';
	do
	{
		m.show_mouse();
		m.get_status();
		delay(50);
		m.hide_mouse();
			if((cx<=200)&&(cx>=70)&&(cy<=250)&&(cy>=60))
			{
				if(ob.glvl >= 1)
				{
					bsound();
					g_lvl.n='1';
					g_lvl.noe = 2;
					click(70,60,200,250,"1",95,105,12);
					a1 = '1';
				}
			}
			else if((cx>=0)&&(cx<=50)&&(cy<=270)&&(cy>=210))
			{

				if(c_lvl!=0)
				{
					bsound();
					g_lvl.n = '7';
					g_lvl.noe = 8 + random(25);
					click(0,210,50,270,"CUSTOM",25,18,0);
					a1 = '1';
				}
			}
			else if((cx<=400)&&(cx>=250)&&(cy<=250)&&(cy>=60))
			{

				 if(ob.glvl >= 2)
				 {

					 bsound();
					 g_lvl.n='2';
					 g_lvl.noe = 8;
					 click(250,60,400,250,"2",295,105,12);
					 a1 = '1';
				 }
			}
			else if((cx<=600)&&(cx>=450)&&(cy<=250)&&(cy>=60))
			{

				if(ob.glvl >= 3)
				{
					bsound();
					g_lvl.n='3';
					g_lvl.noe = 12;
					click(450,60,600,250,"3",495,105,12);
					a1 = '1';
				}
			}
			else if((cx<=200)&&(cx>=70)&&(cy<=460)&&(cy>=270))
			{

				 if(ob.glvl >= 4)
				 {
					 bsound();
					 g_lvl.n='4';
					 g_lvl.noe = 15;
					 click(70,270,200,460,"4",95,315,12);
					 a1 = '1';
				 }
			}
			else if((cx<=400)&&(cx>=250)&&(cy<=460)&&(cy>=270))
			{

				 if(ob.glvl >= 5)
				 {
					 bsound();
					 g_lvl.n='5';
					 g_lvl.noe = 20;
					 click(250,270,400,460,"5",295,315,12);
					 a1 = '1';
				 }
			}
			else if((cx<=600)&&(cx>=450)&&(cy<=460)&&(cy>=270))
			{

				if(ob.glvl >= 6)
				{
					 bsound();
					 g_lvl.n='6';
					 g_lvl.noe = 25;
					 click(450,270,600,460,"6",495,315,12);
					 a1 = '1';
				}
			}
			else if((cx>=550)&&(cx<=635)&&(cy>=0)&&(cy<=30))
			{
				 bsound();
				 loadout();
				 level_scrn_UI();
				 a1 = '0';
			}
			else if((cx<=50)&&(cx>=0)&&(cy<=40)&&(cy>=0))
			{
				 bsound();
				 return;
			}
			else a1 = '0';
	}while(a1 == '0');
}
void level()
{
	setcolor(set.color);
	setfillstyle(1,set.color);
	if(g_lvl.n=='1')
	{
		l_draw("LVL1.TXT",ary);
	}
	else if(g_lvl.n=='2')
	{
		l_draw("LVL2.TXT",ary);
	}
	else if(g_lvl.n=='3')
	{
		l_draw("LVL3.TXT",ary);
	}
	else if(g_lvl.n=='4')
	{
		l_draw("LVL4.TXT",ary);
	}
	else if(g_lvl.n=='5')
	{
		l_draw("LVL5.TXT",ary);
	}
	else if(g_lvl.n=='6')
	{
		l_draw("LVL6.TXT",ary);
	}
	else if(g_lvl.n=='7')
	{
		l_draw("LVLC.TXT",ary);
	}
}
void updatescore()
{
	setcolor(BLACK);
	settextstyle(0,0,2);
	outintxy(120,12,scr.score);
	scr.score+=10;
	setcolor(BLUE);
	outintxy(120,12,scr.score);
	settextstyle(0,0,0);
}
void updatecoin()
{
	setcolor(BLACK);
	settextstyle(0,0,2);
	outintxy(308,12,scr.coins);
	scr.coins+=1;
	setcolor(RED);
	outintxy(308,12,scr.coins);
	settextstyle(0,0,0);
}
void interface()
{
	cn.coin();
	setbkcolor(BLACK);
	platform();
	setcolor(BLACK);
	setcolor(WHITE);
	line(0,41,640,41);
	line(180,0,180,41);
	line(360,0,360,41);
	setcolor(YELLOW);
	settextstyle(0,0,2);
	outtextxy(10,12,"SCORE: ");
	setcolor(BLUE);
	outintxy(120,12,scr.score);
	setcolor(YELLOW);
	outtextxy(370,12,"LIFE : ");
	outtextxy(190,12,"COINS : ");
	setcolor(RED);
	outintxy(308,12,scr.coins);
	lf_bar();
	setfillstyle(1,RED);
	setcolor(WHITE);
	settextstyle(0,0,1);
	rectangle(260,41,380,70);
	outtextxy(270,50,"AMMO : ");
	outintxy(330,50,ob.ammo,0);

}
void character::charac(int c,int x,int x1 , int y , int z)
{
	if((o==0) && (x!=0)) x = x1 = y = z = c;
	if(c==x1 && x1 == x && c == 0) z = 0;
	if(inv==0)
	{
		setcolor(x);
		circle(xh, yh,12); //HEAD
		setcolor(x1);
		line( xl, yh+15, xl, yl);
		setcolor(x);
		line( xl, yl, xl-10, yl+10);
		line( xl, yl, xl+10, yl+10);
		if(o!=0)
		{
		setfillstyle(1,y); //GUN
		bar( xb+3, yb, xb+20, yb+3);
		setfillstyle(1,z); //GUN Handle
		bar( xb+9, yb+3, xb+12, yb+7);
		}
	}
	else
	{
		setcolor(x);
		circle( xh, yh,12); //HEAD
		setcolor(x1);
		line( xl, yh+15, xl, yl);
		setcolor(x);
		line( xl, yl, xl-10, yl+10);
		line( xl, yl, xl+10, yl+10);   //LEGS
		if(o!=0)
		{
			setfillstyle(1,y); //GUN
			bar( xb-3, yb, xb-20, yb+3);
			setfillstyle(1,z); //GUN Handle
			bar( xb-9, yb+3, xb-12, yb+7);
		}
	}
}
void invert()
{
	ob.charac(0,0,0,0);
	if(inv==1) inv=0;
	else if(inv==0) inv=1;
	level();
	ob.charac();
}
void inp_name(char *p, int i, char *t)
{
	cleardevice();
	if(i==0) p = "Name : ";
	char s[50] = "\0";
	char ch;
	settextstyle(0,0,2);
	setcolor(WHITE);
	outtextxy(50,220,p);
	for(int j=0,k=0;ch!=char(13);)
	{
		ch = getch();
		if((isalnum(ch))||(ch==' '))
		{
			setcolor(BLUE);
			s[j] = ch;
			j++;
			s[j] = '\0';
			if(i==0)
				outtextxy(207,220,s);
			else
				outtextxy(307,220,s);
			k++;

		}
		else if(ch==27)
		{
			cleardevice();
			setcolor(WHITE);
			outtextxy(50,220,p);
			s[0]='\0';
			j=0;
		}
		else if((ch==8)&&(k!=0))
		{

			cleardevice();
			setcolor(WHITE);
			outtextxy(50,220,p);
			j--;
			s[j]='\0';
			k--;
			setcolor(BLUE);
			if(i==0)
				outtextxy(207,220,s);
			else
				outtextxy(307,220,s);
		}
		if(j<0) j=0;
		if((k>=500)||(k<0)) ch = char(13);
	}
	s[j]='\0';
	if(i==0)
	{
		 strcpy(scr.name,s);
	}
	else *t = s[0];
	delete p;
}
void l_bar(int a,int b,int c,int d)
{
	setcolor(YELLOW);
	setlinestyle(0,1,3);
	rectangle(a-1,b-1,c+1,d+1);
	setfillstyle(1,WHITE);
	bar(a,b,c,d);
	setcolor(WHITE);
	outtextxy(327,330,"0");
	outtextxy(335,330,"%");
	setfillstyle(7,YELLOW);
	for(int i = 110;i>1; i--)
	{
		bar(a, b, (c+1)-i, d);
		setcolor(WHITE);
		int p;
		p = ((110-i)/110.0)*100;
		if(p%10==0)
		{
			outintxy(319,330,p/10,0);
			delay(70);
			setcolor(BLACK);
			outintxy(319,330,p/10,0);
			delay(5);
		}
	}
	setcolor(BLACK);
	settextstyle(0,0,0);
	outtextxy(327,330,"0");
	outtextxy(335,330,"%");
	for(i = 0;i<2;i++)
	{
		setcolor(WHITE);
		outtextxy(310,330,"100");
		outtextxy(335,330,"%");
		delay(850);
		setcolor(BLACK);
		outtextxy(310,330,"100");
		outtextxy(335,330,"%");
		delay(300);
	}
	setcolor(WHITE);
	outtextxy(310,330,"100");
	outtextxy(335,330,"%");
	setlinestyle(0,1,1);

}
void intro()
{
	settextstyle(0,0,2);
	setcolor(9);
	delay(500);
	outtextxy(200,70,"Welcome To....");
	settextstyle(0,0,3);
	setcolor(RED);
	delay(400);
	outtextxy(170,160,"Call Of 2-D");
	delay(400);
	setcolor(YELLOW);
	delay(500);
	settextstyle(0,0,0);
	settextstyle(0,0,0);
	l_bar(265,315,375,324);
	delay(200);
	setcolor(CYAN);
	settextstyle(0,0,0);
	outtextxy(360,450,"Press any Key to Continue.....");
	getch();
	delay(300);
	setcolor(BLACK);
	outtextxy(360,450,"Press any Key to Continue.....");
	delay(500);
}
void stats()
{
	clearbuffer();
	cleardevice();
	setcolor(YELLOW);
	settextstyle(0,0,2);
	if(g_o == 2)
	{
			outtextxy(150,50,"Level Complete!");
			settextstyle(0,0,1);
			outtextxy(150,75, "Well done, ");
			setcolor(LIGHTBLUE);
			settextstyle(0,0,2);
			outtextxy(300,75,scr.name);
	}
	else if(g_o == 1)
	{
		outtextxy(150,50,"Level Failed!");
		settextstyle(0,0,1);
		outtextxy(150,75, "Hard luck, ");
		setcolor(LIGHTBLUE);
		settextstyle(0,0,2);
		outtextxy(300,75,scr.name);
	}
	settextstyle(0,0,1);
	outtextxy(50,140,"Coins Collected : ");
	outtextxy(50,220,"No: of Kills    : ");
	outtextxy(50,300,"Player Score    : ");
	setcolor(RED);
	outintxy(300,140,scr.coins);
	setcolor(GREEN);
	outintxy(300,220,scr.kills);
	setcolor(BLUE);
	outintxy(300,300,scr.score);
	setcolor(2);
	rectangle(230,355,350,375);
	setfillstyle(1,10);
	bar(240,360,240 +(pre_exp/ob.lvl),370);
	setcolor(YELLOW);
	outtextxy(370,365,"Level : ");
	setcolor(CYAN);
	outintxy(490,365,pre_lvl);
	for(int i = 240 +(pre_exp/pre_lvl),j = pre_lvl;(i<(240+(ob.exp/j))||(j < ob.lvl));i++)         // 240 to 340
	{
		setfillstyle(1,10);
		bar(240,360,i,370);
		delay(20);
		if((i == 340)&&(j<ob.lvl))
		{
			setcolor(BLACK);
			outintxy(490,365,j);
			j++;
			setcolor(CYAN);
			outintxy(490,365,j);
			i = 240;
			setfillstyle(1,0);
			bar(240,360,340,370);
			setfillstyle(1,10);
		}
	}
	getch();
}
void game_over(int jew)
{

		if(g_o == 0)
		{
			delay(600);
			settextstyle(0,0,4);
			cleardevice();
			/*for(int i=20;i<=9000;i+=50)
			{
				sound(i);
				delay(5);
			}*/
			nosound();
			for(i=1;i<2;i++)
			{
				setcolor(8);
				if(jew == 0)
					outtextxy(170,220,"GAME OVER!");
				else
					outtextxy(100,220,"LEVEL COMPLETED!");
				delay(600);
				setcolor(BLACK);
				if(jew == 0)
					outtextxy(170,220,"GAME OVER!");
				else
					outtextxy(100,220,"LEVEL COMPLETED!");
				delay(600);
			}
			file = 0;
			opt= 27;
			temp = 1;
			if(jew == 0)
			{
				g_o = 1;
				ob.glvl = 1;
			}
			else if(jew == 1)
			{
				 g_o = 2;
				 ob.glvl++;
			}
			stats();
			pre_lvl = ob.lvl;
			pre_exp = ob.exp;
		}
		writescore();
	delay(200);
}
void d_dash(int &t, int i = 100)
{
	setcolor(7);
	for(;i<=460;i+=20)
		outtextxy(i,t," - ");
	t+=50;
}
void controlUI()
{
	int t = 100;
	settextstyle(0,0,2);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Jump");
	d_dash(t);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Duck");
	d_dash(t);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Shoot");
	d_dash(t);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Move Right");
	d_dash(t,170);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Move Left");
	d_dash(t,170);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Invert Character");
	d_dash(t,270);
	setcolor(LIGHTBLUE);
	outtextxy(10,t,"Exit");
	d_dash(t);
	setcolor(YELLOW);
	line(0,40,640,40);
	setcolor(CYAN);
	settextstyle(0,0,2);
	outtextxy(280,10,"CONTROLS");
	setcolor(BLUE);
	setfillstyle(11,BLUE);
	bar(1,1,49,38);
	rectangle(0,0,50,39);
	bar(550,1,630,38);
	rectangle(549,0,631,39);
	settextstyle(0,0,1);
	setcolor(RED);
	outtextxy(10,20,"BACK");
	outtextxy(555,20,"CUSTOMIZE");
	char *p = &(ctrl.u);
	settextstyle(0,0,2);
	char s[5];
	s[1] = '\0';
	setcolor(RED);
	for(int j= 100;j<=400;j+=50,p++)
	{
		s[0] = *p;
		s[0] = toupper(s[0]);
		if(s[0]==' ') outtextxy(500,j,"Space");
		else if(s[0]==27) outtextxy(500,j,"Esc");
		else outtextxy(500,j,s);
	}
}
void customopt()
{
	cleardevice();
	int t = 39;
	settextstyle(0,0,2);
	bar3d(t,85,t+120,125,3,1);
	outtextxy(45,100,"(J)ump");
	bar3d(t,235,t+120,275,3,1);
	outtextxy(45,250,"(D)uck");
	bar3d(t,385,t+120,425,3,1);
	outtextxy(41,400,"(S)hoot");
	t = 350;
	bar3d(t,85,t+199,125,3,1);
	outtextxy(355,100,"Move (R)ight");
	bar3d(t,235,t+199,275,3,1);
	outtextxy(355,250,"Move (L)eft");
	bar3d(t,385,t+199,425,3,1);
	outtextxy(385,400,"(I)nvert");
	bar3d(235,235,305,275,3,1);
	settextstyle(0,0,1);
	outtextxy(245,250,"R(e)set");
}
void customctrl()
{
	cleardevice();
	int t = 0;
	char ch;
	do
	{
		setcolor(RED);
		if(t == 0) customopt();
		ch = getch();
		ch = tolower(ch);
		switch(ch)
		{
			case 'j' : inp_name("Key for Jump : ",1,&ctrl.u);t=0; break;
			case 'd' : inp_name("Key for Duck : ",1,&ctrl.d); t=0;break;
			case 'r' : inp_name("Key for Right : ",1,&ctrl.r);t=0;break;
			case 'l' : inp_name("Key for Left : ",1,&ctrl.l);t=0; break;
			case 'i' : inp_name("Key for Invert : ",1,&ctrl.in);t=0;break;
			case 's' : inp_name("Key for Shoot : ",1,&ctrl.sh);t=0;break;
			case 'e' : ctrl.reset_c();
			default : t = 1;
		}
	}while(ch!=27);
}
void controls()
{
	cleardevice();
	controlUI();
	char poop = 2192;
	do
	{
		m.show_mouse();
		m.get_status();
		delay(20);
		m.hide_mouse();

		if((cx<=50)&&(cy<=40)) poop='1';
		else if((cx>=550)&&(cx<=630)&&(cy>=0)&&(cy<=40))
		{
			bsound();
			customctrl();
			poop = '0';
			cleardevice();
			controlUI();
		}
		else poop='0';
	}while(poop=='0');
	save_control();
}
void highscore(int x)
{
	cleardevice();
	setcolor(YELLOW);
	line(0,40,640,40);
	setcolor(CYAN);
	settextstyle(0,0,2);
	outtextxy(250,10,"HIGHSCORES");
	setcolor(14);
	settextstyle(0,0,2);
	outtextxy(10,70,"Rank");
	line(5,89,75,89);
	outtextxy(120,70,"Name");
	line(115,89,185,89);
	outtextxy(240,70,"Kills");
	line(235,89,319,89);
	outtextxy(370,70,"Coins");
	line(365,89,450,89);
	outtextxy(520,70,"Score");
	line(515,89,605,89);
	sort_scores();
	ifstream f("highscor.dat",ios::binary);
	score ob;
	settextstyle(0,0,1);
	setcolor(random(15)+1);
	for(int i=0,k=1;(f.read((char*)&ob,sizeof(score)))&&(i<50*x);i+=50,k++)
	{
		setcolor(random(13)+1);
		if((ob.score!=0))
		{
			outintxy(40,120+i,k,2);
			settextstyle(0,0,1);
			outtextxy(130,120+i,ob.name);
			outintxy(270,120+i,ob.kills,2);
			outintxy(400,120+i,ob.coins,2);
			outintxy(550,120+i,ob.score,2);
	       }

	}
	f.close();
	setcolor(BLUE);
	setfillstyle(11,BLUE);
	rectangle(0,0,50,39);
	bar(1,1,49,38);
	setcolor(RED);
	settextstyle(0,0,1);
	outtextxy(10,20,"BACK");
	char poop;
	do
	{
		m.show_mouse();
		m.get_status();
		delay(20);
		m.hide_mouse();
		if((cx<=50)&&(cy<=40))
		{
			 poop='1';
			 bsound();
		}
		else poop='0';
	}while(poop=='0');
}
void game()
{
	rand_v();
	cn.gen();
	interface();
	ob.charac();
	level();
	do
	{
		int r,r1,t;
		r = random(20);
		r1 = random(10);
		if(r == r1)
		{
			if(e_c <= 0)
			{
				eno++;
				e[eno] = new enemy;
				e[eno]->charac();
				e[eno]->e_track();
			}
			e_c++;
		}
			e[eno]->o = 0;
		do
		{
			{
				if(eno!=-1)
				{

						if((e[eno]->coll_leg())||(e[eno]->coll_body_l())||(e[eno]->coll_h_left())) e[eno]->p = 1;
						else if((e[eno]->coll_leg())||(e[eno]->coll_body_r())||(e[eno]->coll_h_right())) e[eno]->p = 0;
						if((e[eno]->p)==0) e[eno]->m_l(2);
						else if((e[eno]->p)==1) e[eno]->m_r(2);
							if((ob.frontier())&&(g_o == 0))
							{
								up_lfbar(e[eno]->skill*-8);
								hitsound();
							}
				}
			}
			if(g_o == 0)
			{
				ob.c_track();
				delay(20);
				t = bioskey(1);
			}
			else t = 1;
		}while(!t);
		e[eno]->o = 1;
		opt = char(t);
		opt = toupper(opt);
		if(opt == ctrl.u) jump();
		 else if(opt == ctrl.d) duck();
		  else if(opt == ctrl.r) ob.m_r();
		   else if(opt == ctrl.l) ob.m_l();
		    else if(opt == ctrl.sh) shoot();
		     else if(opt == ctrl.in) invert();
		      else if(opt == 'U') up_lfbar(-5);

		if((g_o == 0)&&(opt!=27))
		{
			ob.c_track();
			if(_coin==1) cn.gen();
			int l=random(6);
			if(l==2)
				cn.coin();
		}
		clearbuffer();
	}while(opt!=27 && g_o == 0);
	save_state();
}
void save_pref()
{
	ofstream f("setting.dat" );
	f.write((char*)&set,sizeof(set));
	f.close();
}
void load_pref()
{
	ifstream f("setting.dat" );
	f.read((char*)&set,sizeof(set));
	f.close();
	strcpy(scr.name,set.s);
}
void select_color()
{
	cleardevice();
	setcolor(WHITE);
	outtextxy(150,100,"SELECT PLATFORM COLOR ");
	int a[14],k=0;
	for(int i = 0; i<14;i++)
		a[i] = i+1;
	for(i=160;i<=320;i+=160)
	{
		for(int j=50;j<=560;j+=100,k++)
		{
			setfillstyle(1,a[k]);
			bar(j,i,j+70,i+70);
		}
	}
	do
	{
		m.show_mouse();
		m.get_status();
		delay(100);
		m.hide_mouse();
		bsound();
		set.color=getpixel(cx,cy);
	}while((set.color==0)||(set.color==15));
}
void show_gun(gun *ob)
{
	char lol=0;
	do
	{
		if(lol!='1')
		{
			cleardevice();
			setcolor(WHITE);
			settextstyle(0,0,4);
			outtextxy(150,50,ob->n);
			setcolor(RED);
			setfillstyle(9,BLUE);
			rectangle(5,5,40,40);
			bar(6,6,39,39);
			settextstyle(0,0,1);
			outtextxy(7,20,"BACK");
			setcolor(WHITE);
			rectangle(350,30,500,100);
			line(250,150,250,420);
			settextstyle(0,0,2);
			setcolor(BLUE);
			setfillstyle(9,BLUE);
			bar3d(500,420,620,460,4,1);
			setcolor(8);
			outtextxy(515,435,"SELECT");
			setcolor(YELLOW);
			outtextxy(145,170,"DAMAGE");
			outtextxy(95,280,"FIRE MODE");
			outtextxy(65,390,"UNLOCKED AT");
			setcolor(RED);
			setfillstyle(9,RED);
			outtextxy(270,390,"LEVEL");
			outintxy(370,390,ob->ul);
			rectangle(270,170,425,190);
			for(int j=275,i=0;i<ob->dmg;i++,j+=15)
				bar(j,175,j+10,185);
			switch(ob->fm)
			{
				case 1 : outtextxy(270,270,"SINGLE FIRE");
					break;
				case 2 : outtextxy(270,270,"DOUBLE FIRE");
					break;
				case 3 : outtextxy(270,270,"BURST  FIRE");
					break;
			}
		}
		m.show_mouse();
		m.get_status();
		delay(30);
		m.hide_mouse();
		if((cx<=620)&&(cx>=500)&&(cy>=420)&&(cy<=460))
		{
			 bsound();
			 set.gn=(*ob);
			 lol='0';
			 delay(100);
			 cleardevice();
		}
		else if((cx>=5)&&(cx<=40)&&(cy>=5)&&(cy<=40))
		{
			 bsound();
			 lol='0';
			cleardevice();
		}
		else lol='1';
	}while(lol!='0');
}
void sort_gun(gun ob[])
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10-i-1;j++)
		{
			if(ob[j].ul>ob[j+1].ul)
			{
				gun t = ob[j];
				ob[j] = ob[j+1];
				ob[j+1] = t;
			}
		}
	}
}
void gun_selection()
{
	cleardevice();
	gun ob[10];
	char p = 0;
	ifstream f("guns.dat",ios::binary);
	for(int n=0;f.read((char*)&ob[n],sizeof(gun));n++);
	f.close();
	sort_gun(ob);
	do
	{
		if(p!='0')
		{
			cleardevice();
			setcolor(RED);
			setfillstyle(9,BLUE);
			rectangle(590,5,635,40);
			bar(591,6,634,39);
			setcolor(8);
			settextstyle(0,0,1);
			outtextxy(596,20,"BACK");
			setcolor(WHITE);
			settextstyle(0,0,3);
			outtextxy(130,40,"SELECT YOUR GUN");
			settextstyle(0,0,2);
			outtextxy(230,80,"(LEVEL  )");
			outintxy(340,80,::ob.lvl);
			setcolor(8);
			setfillstyle(9,LIGHTGRAY);
			for(int k = 150; k<=390; k+=60)
				bar3d(170,k,280,k+40,3,1);
			for(k = 150;k<=390;k+=60)
				bar3d(350,k,460,k+40,3,1);
			setcolor(BLACK);
			settextstyle(0,0,2);
			for(int i=0,j=165;i<5;i++,j+=60)
				outtextxy(187,j,ob[i].n);
			for(j=165;i<=9;i++,j+=60)
				outtextxy(367,j,ob[i].n);
		}
		m.show_mouse();
		m.get_status();
		delay(30);
		m.hide_mouse();
		int t = 170,c = 1;
		if((cx>=590)&&(cx<=635)&&(cy>=5)&&(cy<=40)) p='1';
		else p='0';
		for(int i = 0,k = 150;(i<=9)&&(c);i++,k+=60)
		{
			if(i==5)
			{
				 k = 150;
				 t = 350;
			}
			if((cx<=(t+110))&&(cx>=t)&&(cy<=k+40)&&(cy>=k))
			{
				bsound();
				if(::ob.lvl<ob[i].ul) gun_lock(ob[i].ul);
				else
					show_gun(&ob[i]);

				p = '2';
				c = 0;
			}
		}
	}while(p!='1');
}
void credits()
{
	cleardevice();
	settextstyle(0,0,1);
	outtextxy(160,220,"*Insert Credits Here*");
	getch();
}
void select_f_color()
{
	cleardevice();
	setcolor(WHITE);
	settextstyle(0,0,3);
	outtextxy(150,100,"SELECT FACE COLOR ");
	int a[14],k=0;
	for(int i = 0; i<14;i++)
	{
		if(i+1==set.color||i+1==set.b_color) a[i]=0;
		else a[i] = i+1;
	}
	a[8]=15;
	for(i=160;i<=320;i+=160)
	{
		for(int j=50;j<=560;j+=100,k++)
		{
			setfillstyle(1,a[k]);
			bar(j,i,j+70,i+70);
		}
	}
	do
	{
		m.show_mouse();
		m.get_status();
		delay(100);
		m.hide_mouse();
		set.f_color=getpixel(cx,cy);
	}while((set.f_color==0));
}
void select_b_color()
{
	cleardevice();
	setcolor(WHITE);
	settextstyle(0,0,3);
	outtextxy(150,100,"SELECT BODY COLOR ");
	int a[14],k=0;
	for(int i = 0; i<14;i++)
	{
		if(i+1==set.color||i+1==set.f_color) a[i] = 0;
		else a[i]=i+1;
	}
	a[8]=15;
	for(i=160;i<=320;i+=160)
	{
		for(int j=50;j<=560;j+=100,k++)
		{
			setfillstyle(1,a[k]);
			bar(j,i,j+70,i+70);
		}
	}
	do
	{
		m.show_mouse();
		m.get_status();
		delay(100);
		m.hide_mouse();
		bsound();
		set.b_color=getpixel(cx,cy);
	}while((set.b_color==0));
}
void customize()
{
	char lol=0;
	do
	{
		if(lol!='1')
		{
			cleardevice();
			setcolor(WHITE);
			settextstyle(0,0,3);
			outtextxy(150,30,"SELECT COLORS");
			setcolor(YELLOW);
			settextstyle(0,0,2);
			outtextxy(150,180,"FACE COLOR : ");
			setfillstyle(1,set.f_color);
			bar(350,160,410,220);
			outtextxy(150,300,"BODY COLOR : ");
			setfillstyle(1,set.b_color);
			bar(350,280,410,340);
			setcolor(RED);
			setfillstyle(9,BLUE);
			bar(1,1,39,39);
			rectangle(0,0,40,40);
			settextstyle(0,0,1);
			outtextxy(10,20,"BACK");
		}
		m.show_mouse();
		m.get_status();
		delay(100);
		m.hide_mouse();
		if((cx>=0)&&(cx<=40)&&(cy>=0)&&(cy<=40))
		{
			 lol='0';
			 bsound();
		}
		else if((cx>=350)&&(cx<=410)&&(cy>=160)&&(cy<=220))
		{
			bsound();
			select_f_color();
			lol='2';
		}
		else if((cx>=350)&&(cx<=410)&&(cy>=280)&&(cy<=340))
		{
			bsound();
			select_b_color();
			lol='2';
		}
		else lol='1';
	}while(lol!='0');
}
void settings()
{
	char lol=0;
	do
	{
		if(lol!='1')
		{
			cleardevice();
			setcolor(YELLOW);
			line(0,30,640,30);
			setcolor(CYAN);
			settextstyle(0,0,2);
			outtextxy(265,12,"SETTINGS");
			setcolor(BLUE);
			setfillstyle(11,BLUE);
			bar(1,1,39,26);
			setcolor(RED);
			rectangle(0,0,40,27);
			settextstyle(0,0,1);
			setcolor(8);
			outtextxy(5,13,"BACK");
			setcolor(LIGHTGRAY);
			outtextxy(200,100,"Platform Color : ");
			setfillstyle(1,set.color);
			bar(355,85,385,115);
			setfillstyle(9,DARKGRAY);
			bar3d(200,170,440,220,4,1);
			bar3d(180,260,460,310,4,1);
			bar3d(140,350,300,400,4,1);
			bar3d(350,350,510,400,4,1);
			bar3d(200,425,440,475,4,1);
			setcolor(WHITE);
			settextstyle(0,0,2);
			outtextxy(220,190,"GUN SELECTION");
			outtextxy(200,280,"CLEAR HIGHSCORES");
			outtextxy(168,370,"CREDITS");
			settextstyle(0,0,2);
			outtextxy(360,370,"CUSTOMIZE");
			outtextxy(230,445,"CREATE LEVEL");
		}
		m.show_mouse();
		m.get_status();
		delay(30);
		m.hide_mouse();
		{
			if((cx>=355)&&(cx<=385)&&(cy<=115)&&(cy>=85))
			{
				 bsound();
				 select_color();
				 lol='2';
			}
			else if((cx<=460)&&(cx>=180)&&(cy>=260)&&(cy<=310))
			{
				bsound();
				remove("highscor.dat");
				click(180,260,460,310,"CLEAR HIGHSCORES",200,280,2,8);
			}
			else if((cx<=440)&&(cx>=200)&&(cy>=170)&&(cy<=220))
			{
				 bsound();
				 click(200,170,440,220,"GUN SELECTION",220,190,2,8);
				 gun_selection();
				 lol='2';
			}
			else if((cx>=140)&&(cx<=300)&&(cy<=400)&&(cy>=350))
			{
				bsound();
				click(140,350,300,400,"CREDITS",168,370,2,8);
				credits();
				lol='2';
			}
			else if((cx>=350)&&(cx<=510)&&(cy<=400)&&(cy>=350))
			{
				bsound();
				click(350,350,510,400,"CUSTOMIZE",360,370,2,8);
				customize();
				lol='2';
			}
			else if((cx>=200)&&(cx<=440)&&(cy>=425)&&(cy<=475))
			{
				bsound();
				click(200,425,440,475,"CREATE LEVEL",230,445,2,8);
				cleardevice();
				l_create();
				c_lvl = 1;
				lol = '2';
				cleardevice();
			}
			else if((cx<=40)&&(cx>=0)&&(cy>=0)&&(cy<=27))
			{
				 bsound();
				 lol='0';
				 save_pref();
			}
			else lol='1';
		}
	}while(lol!='0');
}
void main_menu()
{
	cleardevice();
	setcolor(LIGHTRED);
	line(0,40,640,40);
	setcolor(CYAN);
	settextstyle(0,0,3);
	outtextxy(220,15,"MAIN MENU");
	setcolor(BLUE);
	setfillstyle(9,BLUE);
	bar3d(30,50,300,250,5,1);//NEWGAME
	bar3d(330,70,610,170,5,1);
	bar3d(30,270,300,470,5,1);//CONTINUE
	bar3d(330,200,610,300,5,1);
	bar3d(330,330,610,430,5,1);
	bar3d(580,7,630,33,3,1);
	setcolor(8);
	settextstyle(0,0,3);
	outtextxy(70,140,"NEW GAME");
	settextstyle(0,0,3);
	outtextxy(80,360,"CONTINUE");
	settextstyle(0,0,3);
	outtextxy(380,115,"CONTROLS");
	outtextxy(360,375,"HIGHSCORES");
	outtextxy(380,245,"SETTINGS");
	settextstyle(0,0,1);
	outtextxy(590,17,"EXIT");
}
void click(int l,int t,int r,int b,char s[],int sx,int sy,int sz,int c)
{
	delay(30);
	settextstyle(0,0,sz);
	setcolor(BLACK);
	bar3d(l,t,r,b,5,2);
	setcolor(8);
	outtextxy(sx,sy,s);
	setcolor(c);
	bar3d(l,t,r,b,2,2);
	setcolor(8);
	outtextxy(sx,sy,s);
	delay(250);
	setcolor(BLACK);
	bar3d(l,t,r,b,2,2);
	setcolor(8);
	outtextxy(sx,sy,s);
	setcolor(c);
	bar3d(l,t,r,b,5,2);
	setcolor(8);
	outtextxy(sx,sy,s);
	delay(150);
}
void main()
{
	clrscr();
	randomize();
	drv;
	load_pref();
	load_controls();
	randomize();
	setbkcolor(BLACK);
	char ch = 'a';
	intro();
	do
	{
		if(ch != '0') main_menu();
		m.show_mouse();
		m.get_status();
		delay(10);
		m.hide_mouse();
		{
			if((cx>=30)&&(cx<=300)&&(cy>=50)&&(cy<=250)) ch='1';
			else if((cx>=330)&&(cx<=610)&&(cy>=70)&&(cy<=170)) ch='2';
			else if((cx>=330)&&(cx<=610)&&(cy>=200)&&(cy<=300)) ch='3';
			else if((cx>=330)&&(cx<=610)&&(cy>=330)&&(cy<=430)) ch='5';
			else if((cx>=580)&&(cx<=630)&&(cy>=7)&&(cy<=33)) ch='4';
			else if((cx>=30)&&(cx<=300)&&(cy>=270)&&(cy<=470)) ch='6';
			else ch = '0';
		}
		if(ch!='0' ) bsound();
		switch(ch)
		{
			case '1' : {
					load_name();
					g_o = 0;
					eno = -1;
					e_c = 0;
					scr.score=0;
					scr.coins=0;
					scr.kills = 0;
					::c=600;
					click(30,50,300,250,"NEW GAME",70,140,3);
					cleardevice();
					delay(300);
					level_scrn();
					ob.reset();
					if((cx<=50)&&(cy<=40)) continue;
					cleardevice();
					setbkcolor(BLACK);
					delay(700);
					setcolor(CYAN);
					pre_lvl = ob.lvl;
					pre_exp = ob.exp;
					game();
					delay(300);
					break;
				    }
			case '2' :  {
					click(330,70,610,170,"CONTROLS",380,115,3);
					setbkcolor(BLACK);
					delay(100);
					controls();
					break;
				     }

			case '5' : {
					click(330,330,610,430,"HIGHSCORES",360,375,3);
					setbkcolor(BLACK);
					delay(100);
					highscore(7);
					break;
				    }
			case '4' : {
					click(580,7,630,33,"EXIT",590,17,1);
					cleardevice();
					setbkcolor(BLACK);
					delay(1000);
					setcolor(CYAN);
					settextstyle(0,0,2);
					for(int i = 0;i<2;i++)
					{
						outtextxy(170,240,"Thank You For Playing ! ");
						delay(800);
						cleardevice();
						delay(400);
					}
					closegraph();
					break;
				  }
			case '3' : {
					click(330,200,610,300,"SETTINGS",380,245,3);
					cleardevice();
					settings();
					break;
				    }
			case '6' : {

					click(30,270,300,470,"CONTINUE",80,360,3);
					cleardevice();
					g_o = 0;
					if(file == 0)
					{
						setcolor(GREEN);
						delay(500);
						settextstyle(0,0,5);
						outtextxy(100,150,"NICE");
						delay(500);
						outtextxy(300,150,"TRY!");
						delay(500);
						outtextxy(450,150," :P ");
						delay(700);
						cleardevice();
						setcolor(RED);
						settextstyle(0,0,2);
						outtextxy(150,220,"GO START A NEW GAME ");
						for(int i=0,j=455;i<5;i++,j+=5)
						{
							outtextxy(j,220,".");
							delay(300);
						 }
						 delay(1500);
					}
					else if(file == 1)
					{
						load_state();
						delay(300);
						game();
						delay(600);

					}
				   }
			}

	}while(ch!='4');
	save_pref();
	closegraph();
}


