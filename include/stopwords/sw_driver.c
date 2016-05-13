
#include "./sw_hash.c"
#define SW_LEN 200
#define MAX_SW 300

char stopw[MAX_SW][SW_LEN];
int g_swcount;

typedef struct sw_bkt
{
	char w[SW_LEN];
	struct sw_bkt *next;
}sw_bkt;

extern struct sw_bkt  *bkt[MAX_SW];

void buildSW()
{

	FILE *s_fp;
	char temp[SW_LEN];
	int i;
	int bid;

	s_fp = fopen("../include/stopwords/swlist","r");

	i=0;
	while( fscanf(s_fp,"%s",temp) != EOF )
	{
		temp[strlen(temp)] = '\0';
		strcpy(stopw[i++],temp);
	}
	g_swcount = i;
	fclose(s_fp);



	
	for(i=0;i<g_swcount;i++)
	{
		bkt[i] = 0;
	}

	for(i=0;i<g_swcount;i++)
	{
		bid = djb2(stopw[i],g_swcount);	
		sw_bkt *b = (sw_bkt *)malloc(sizeof(sw_bkt));
		
		b->next=NULL;
		strcpy(b->w,stopw[i]);
		b->w[strlen(b->w)]='\0';
		
		if( !bkt[bid] )
		{
			bkt[bid] = b;
			//printf("storing in bid:%d w:%s\n",bid,bkt[bid]->link->w);
		}
		else
		{
			sw_bkt *tmp,*prev;
			tmp = prev = bkt[bid];
			while(tmp)
			{
				prev = tmp;
				tmp = tmp->next;
			}
			prev->next = b;
		}
	}

}

int isSW(char ip[])
{
	int bid;

	bid = djb2(ip,g_swcount);
	
	if( bkt[bid] )
	{
		sw_bkt *t2;
		t2 = bkt[bid];

		while(t2)
		{
			if( !strcmp(ip,t2->w) )
				return 1;
			else
				t2 = t2->next;
		}
		return 0;
	}	
	else
		return 0;
}

