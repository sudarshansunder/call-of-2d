#include<all.h>
struct score
{
	 char name[50];
	 int kills;
	 int coins;
	 int dodge;
	 float score;
	 void input();
	 int calc();
	 score()
	 {
		strcpy(name,"\0");
		kills=0;
		coins=0;
		dodge=0;
		score=0;
	 }
};
int score::calc()
{
	score=(kills*10)+(coins)+(5*dodge);
	return score;
}

void score::input()
{
	cout<<"\nName : ";gets(name);
	cout<<"\nKills : ";cin>>kills;
	cout<<"\nCoins : ";cin>>coins;
	cout<<"\nDodges : ";cin>>dodge;
	score=calc();
}
void create()
{
	score ob;
	ofstream f("HIGHSCOR.DAT");
	for(int i=0;i<=3;i++)
	{
		ob.input();
		f.write((char*)&ob,sizeof(score));
	}
	f.close();
}
void display()
{
	ifstream f("HIGHSCOR.DAT");
	score ob;
	while(f.read((char*)&ob,sizeof(score)))
	{
		cout<<"\n"<<ob.name<<'\t'<<ob.kills<<'\t'<<ob.coins<<'\t'<<ob.dodge<<'\t'<<ob.score;
	}
	f.close();
}
void main()
{
	clrscr();
	//create();
	clrscr();
	display();
	getch();
}



