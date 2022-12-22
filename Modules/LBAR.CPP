void l_bar()
{
	setfillstyle(1,WHITE);
	bar(390,470,638,480);
	setfillstyle(7,YELLOW);
	for(int i = 195; i>2; i--)
	{
		bar(395, 473, 638-i, 477);
		delay(8);
	}
}