
//int g_swcount; //No of Stop words
int djb2(char *str,int g_swcount)
{
	unsigned long hash = 5381;
        int c;

        while (c = *str++)
          	hash = ((hash << 5) + hash) + c; 
	     // hash = hash * 33 + c ;
	

        return hash % g_swcount;
}
