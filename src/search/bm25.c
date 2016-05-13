
char q1[20][50];


pFile *pf_bm25[100];

float idf_bm25[100];
int n_qterms_bm25;//No. of qery terms 




pFile * loadPF_bm25(char term[],int i)
{
	char pf_name[200];
	FILE *pf_hdd;

	 pFile *pf_temp;
	pFile *pf_hdd_root,*prev;


	strcpy(pf_name,"../bin/ifiles/");
	pf_name[strlen(pf_name)]='\0';
	strcat(pf_name,term);//strcat(pf_name,tmp->bin);
	pf_name[strlen(pf_name)]='\0';

	//printf("opening file %s\t",pf_name);
	pf_hdd = fopen(pf_name,"rb");
	pf_hdd_root=NULL;
	if(pf_hdd)
	{	//printf("  opened file");
		pf_temp = (pFile *)malloc(sizeof(pFile));
		while(fread(pf_temp,sizeof(pFile),1, pf_hdd))
		{

			//cal value
			pf_temp->tfidf = idf_bm25[i]* (pf_temp->tfd * 3.0)/(pf_temp->tfd + (2.0*(0.25+(.75*doc_len/avg_doc_len))));

			pf_temp->next = NULL;
			if(!pf_hdd_root)
				pf_hdd_root = pf_temp;//,printf("root Not set\n");
			else
				prev->next = pf_temp;//,printf("exec else\n");;
			prev = pf_temp;
		
			pf_temp = (pFile *)malloc(sizeof(pFile));
		
		}
		fclose(pf_hdd);

		
		return pf_hdd_root;
	}
	else
	{

		return NULL;
	}

}

void build_query_bm25()
{

	FILE *qf;
	char word1[50];
	int p1,i;
	long df_temp;
	
	qf=fopen("../bin/stemop","r");
	
	p1=0;	
	while(fscanf(qf,"%s",word1)!=EOF)
	{

		strcpy(q1[p1++],word1); 
		q1[p1-1][strlen(q1[p1-1])]='\0';
		strcpy(word1,"\0");	
	}
	fclose(qf);
	n_qterms_bm25 = p1;

	//Get Single Document List
	i=0;
	while(i<p1)
	{
		sres *s1 = build_search(q1[i]);
		//pf_bm25[i] = (pf_ptrs_bm25 *)malloc(sizeof(pf_ptrs_bm25));
		if(s1->found==1)
		{
			df_temp = s1->near->df;
			idf_bm25[i] = logl ((1.0f*doc_count - df_temp + 0.5) / (1.0f*df_temp+0.5) );//IDF for Q i

			//printf("N = %ld \t df = %ld\n",doc_count,df_temp);
			//printf("*********IDF Value calulated:%f*************\n",idf_bm25[i]);
			pf_bm25[i] = loadPF_bm25(q1[i],i);//Posting List for qery term using function from qry.c
		}
		else
		{
			df_temp = 0;
			idf_bm25[i] = logl ( (doc_count - df_temp + 0.5) / (df_temp+0.5) );//IDF for Q i
			pf_bm25[i] = NULL;
		}

		
		
		
		i++;
	}
	pFile *f_link,*tmp1,*tmp2,*prev;
	int j;
	if(p1==1)
		f_link = pf_bm25[0];
	
	for(i=0;i<p1-1;i++)
	{

		j=i+1;
		tmp1 = pf_bm25[i];//check again
		tmp2 = pf_bm25[j];

		if(tmp1 && tmp2)		
		{
			while(tmp1 && tmp2)
			{
				if(tmp1->did == tmp2->did)
				{		
					pFile *x1 = (pFile *)malloc(sizeof(pFile));
					x1->did = tmp1->did;
					x1->tfidf = tmp1->tfidf + tmp2->tfidf;
					x1->next = NULL;

					if(!f_link)
						f_link = x1;
					else
						prev->next = NULL;

					prev = x1;

					tmp1 = tmp1->next;
					tmp2 = tmp2->next;
					free(tmp1);free(tmp2);
				}
			}
			
		}
		else if(tmp1)
		{
			while(tmp1)
			{
				pFile *x1 = (pFile *)malloc(sizeof(pFile));
				x1->did = tmp1->did;
				x1->tfidf = tmp1->tfidf ;
				x1->next = NULL;
				if(!f_link)
					f_link = x1;
				else
					prev->next = NULL;
				prev = x1;
				tmp1 = tmp1->next;
				free(tmp1);
			}
		}
		else
		{
			while(tmp2)
			{
				pFile *x1 = (pFile *)malloc(sizeof(pFile));
				x1->did = tmp2->did;
				x1->tfidf = tmp2->tfidf ;
				x1->next = NULL;
				if(!f_link)
					f_link = x1;
				else
					prev->next = NULL;
				prev = x1;
				tmp2 = tmp2->next;
				free(tmp2);
			}		
		}
		
		
		
	}

	//Sort
	if(f_link)
	{
		tmp1 = f_link;
		long t1; float t2;
		while(tmp1)
		{
			tmp2 = tmp1->next;
			while(tmp2)
			{
				if(tmp1->tfidf < tmp2->tfidf)
				{
					//swap
					t1 = tmp1->did; t2 = tmp1->tfidf;

					tmp1->did = tmp2->did;  tmp1->tfidf = tmp2->tfidf;

					tmp2->did = t1; tmp2->tfidf = t2;
				
				}
				tmp2 = tmp2->next;
			}
			
			tmp1 = tmp1->next;
		}

		FILE *r_doc_dic;
		doc_dictionay *r_dd_node = (doc_dictionay *)malloc(sizeof(doc_dictionay));

		r_doc_dic = fopen("../bin/docdic","rb");


		//printf("\n\nResults found in\n");
		while(f_link)
		{

			fseek(r_doc_dic,0,SEEK_SET); //GO to start
			fseek(r_doc_dic,sizeof(doc_dictionay)*(f_link->did - 1),SEEK_SET); //Go to req. node
			fread(r_dd_node,sizeof(doc_dictionay),1,r_doc_dic);//Read the node from file & print file name


			//printf("Doc ID: %ld\tTF_IDF: %lf\n",root->did,root->tfidf);
			//printf("%s\tTF_IDF:%f\n",r_dd_node->path,f_link->tfidf);
			printf("%s\n",r_dd_node->path);
			f_link = f_link->next;
		
		}
		fclose(r_doc_dic);
	
				
	}

	//getDocList_BM25();
	
}

void process_query_bm25(int argc,char *argv[])
{
	FILE *q_file;//,*r_doc_dic;

	//doc_dictionay *r_dd_node = (doc_dictionay *)malloc(sizeof(doc_dictionay));
	
	int q_terms,i;
	int qt_count;//Actual count of the query terms found in the tree
	
	char query_term[50];
	char pf_name[200];

	pFile *qt_pfs[argc-3];// Max of argc -2 query terms, one link to each
	long dfs[argc-3];// same as above for Doc Freq
	
	//r_doc_dic = fopen("../bin/docdic","rb");
	q_file = fopen("../bin/query","w+");
	q_terms = argc-3;// 1st two r filename & -s

	for(i=3;i<argc;i++)
	{
		fprintf(q_file,"%s",argv[i]);
		if(i+1 != argc)			
			fprintf(q_file,"%c",' ');
	}
	fclose(q_file);
	
	do_stem("../bin/query");

	build_query_bm25();

}

