/*//////////////////////////////////////////////////////////////////////////////////////////////////

	Converts MD5 Hash to Binary String


K REDDIGIRISH
10410131
M.Tech 1st Year
k.reddigirish@iitg.ernet.in
/*//////////////////////////////////////////////////////////////////////////////////////////////////
extern char g_key[140], g_bin[129],g_hex[33];


void toBinary()
{
	int i;
	int j=0;
	for(i=0;i<32;i++)
	{
		switch(g_hex[i])
		{
			case '0': g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='0';	break;
			case '1': g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='1';	break;
			case '2': g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='0';	break;
			case '3': g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='1';	break;
			case '4': g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='0';	break;
			case '5': g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='1';	break;
			case '6': g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='0';	break;
			case '7': g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='1';	break;
			case '8': g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='0';	break;
			case '9': g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='0';g_bin[j++]='1';	break;
			case 'a': g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='0';	break;
			case 'b': g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='1';g_bin[j++]='1';	break;
			case 'c': g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='0';	break;
			case 'd': g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='0';g_bin[j++]='1';	break;
			case 'e': g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='0';	break;
			case 'f': g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='1';g_bin[j++]='1';	break;		
		}
	}
	g_bin[128]='\0';
/*	FILE *h2b;
	h2b = fopen("../bin/hashvalues","a+");
	fprintf(h2b,"%s %s\n",g_key,g_bin);
	fclose(h2b);
*/
}
