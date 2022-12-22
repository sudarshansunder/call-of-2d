/* Function Prototypes to prevent compilation errors due
	to bad placement of functions*/
#define UP 18432
#define DWN 20480
#define RT 19712
#define LT 19200
#define ST 14624
#define INV 11875
#define EX 11640
void target();
void lf_bar();
void click(int,int,int,int,char s[],int,int,int,int c=1);
void up_lfbar(int);
void health();
void game_over(int);
void coinsound();\
void invert();
void updatecoin();
void fallsound();
void platform();
void outintxy(int ,int, int, int sz = 2);
void heartxy(int , int);
void heartxy_half(int , int);
void updatescore();
void shoot();
void pt(int,int,int,int,int);
void level();
void jump();
void duck();
void interface();
void outcharxy(int,int,char);
void inp_name(char *p = NULL, int i = 0, char *t = NULL);
void sort_scores();
void l_bar();
void intro();
void controls();
void highscore(int);
void game();
/*

	END OF PROTOTYPES!!!

*/
