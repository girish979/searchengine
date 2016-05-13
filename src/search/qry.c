
#include "./exp1.c"
extern long doc_count;

char q1[20][50];// reads input from php (Also Stemmed)
char qry_f[20][50];//Query formed by placing AND,OR & NOT
char pfop[50];

typedef struct q_t_struct
{
	char term[50];
	struct pFile *pf;//Pointer to Posting List
}q_t_struct;

q_t_struct *qt_ar[20];


//stack_str *top=NULL;

q_t_struct *stk_arr[100];
int stk_top=-1;
void stk_push(q_t_struct *rec)
{

	if(stk_top==-1)
	{
		stk_top++;
		stk_arr[stk_top] = rec;
	}
	else
	{
		stk_top++;
		stk_arr[stk_top] = rec;
	}
	
}
q_t_struct * stk_pop()
{
	q_t_struct *temp;
	temp = stk_arr[stk_top];
	stk_arr[stk_top]=NULL;
	stk_top--;
	return temp; 
}

sres * build_search(char term[])
{

	mdstr *a1;
	strcpy(g_key,term);
	g_key[strlen(g_key)]='\0';
	a1 = (mdstr *)do_test(g_key);//Generate MD5
	strcpy(g_hex,a1->hash);
	g_hex[32]='\0';
	free(a1);
	toBinary();
	sres *srch1;
	srch1 = tree_ssearch(g_key,g_bin);	

	return srch1;
}

void eval_pfx()
{

	char pf_exp[100];
	FILE *pf_file;
	int len,i;//len length of postfix
	char ch;
	float idf1,idf2;
	pf_file = fopen("../bin/postfix.txt","r");

	fscanf(pf_file,"%s",pf_exp);
	fclose(pf_file);

	//printf("PostFix Exp: %s",pf_exp);
	len = strlen(pf_exp);
	pf_exp[len]='\0';

	i=0;
	while(i<len)
	{
		ch = pf_exp[i];
		if(ch <= '9' && ch >= '0')
		{
			//push to stack
			stk_push(qt_ar[ch-48]);
		}
		else if (ch=='*' || ch=='+')		
		{
			q_t_struct *e2,*e1;
			e2 = stk_pop();
			e1 = stk_pop();

			sres *s1 = build_search(e1->term);
			sres *s2 = build_search(e2->term);
			pFile *n_root=NULL;
	
			pFile *pf1,*pf2,*prev;
			pf1 = e1->pf;
			pf2 = e2->pf;
			if(ch == '*')
			{
				
				if((e1->pf && e2->pf) )//(!strcmp(e1->term,"merged") && s2->found) )
				{
					//Create New PL in MM				
					//push its addr to stack
					
					
					
					while(pf1 && pf2)
					{
						if(pf1->did == pf2->did)
						{
							pFile *re_b = (pFile *)malloc(sizeof(pFile));
							re_b->did = pf1->did; 
							
							if(!strcmp(e1->term,"merged"))
							{
								idf1 = pf1->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf1 = logl(1.0f*doc_count/s1->near->df);
								idf1 = idf1*pf1->tfd; //TFIDF
							}

							if(!strcmp(e2->term,"merged"))
							{
								idf2 = pf2->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf2 = logl(1.0f*doc_count/s2->near->df);

								
								idf2 = idf2*pf2->tfd; //TFIDF
							}

							re_b->tfidf = idf1+idf2;  //Total tf-idf	
							
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;

							pf1 = pf1->next;
							pf2 = pf2->next;
						}
						else if(pf1->did < pf2->did)
							pf1 = pf1->next;
						else
							pf2 = pf2->next;
					}
					//Create record & insert into stack with root pointer
					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
					
				}
				else//New = NULL
				{
					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
				}
			}
			else //ch =='+'if()
			{
				
				while(pf1 && pf2) //Merrge Both
				{ 
						if(pf1->did == pf2->did)
						{
							pFile *re_b = (pFile *)malloc(sizeof(pFile));
							re_b->did = pf1->did; 
							
							if(!strcmp(e1->term,"merged"))
							{
								idf1 = pf1->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf1 = logl(1.0f*doc_count/s1->near->df);
								idf1 = idf1*pf1->tfd; //TFIDF
							}

							if(!strcmp(e2->term,"merged"))
							{
								idf2 = pf2->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf2 = logl(1.0f*doc_count/s2->near->df);
								idf2 = idf2*pf2->tfd; //TFIDF
							}

							re_b->tfidf = idf1+idf2;  //Total tf-idf	
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;

							pf1 = pf1->next;
							pf2 = pf2->next;
						}
						else if(pf1->did < pf2->did)
						{
							pFile *re_b = (pFile *)malloc(sizeof(pFile));
							re_b->did = pf1->did; 

							if(!strcmp(e1->term,"merged"))
							{
								idf1 = pf1->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf1 = logl(1.0f*doc_count/s1->near->df);
								idf1 = idf1*pf1->tfd; //TFIDF
							}
							re_b->tfidf = idf1;  //Total tf-idf	
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;

							
							pf1 = pf1->next;
						}
						else
						{
							pFile *re_b = (pFile *)malloc(sizeof(pFile));
							re_b->did = pf2->did; 

							if(!strcmp(e2->term,"merged"))
							{
								idf2 = pf2->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf2 = logl(1.0f*doc_count/s2->near->df);
								idf2 = idf2*pf2->tfd; //TFIDF
							}
							re_b->tfidf = idf2;  //Total tf-idf	
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;	
							pf2 = pf2->next;
						}
					
				}//while
				if(pf1) //Append pf1
				{
					while(pf1)
					{
						pFile *re_b = (pFile *)malloc(sizeof(pFile));
						re_b->did = pf1->did;
							if(!strcmp(e1->term,"merged"))
							{
								idf1 = pf1->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf1 = logl(1.0f*doc_count/s1->near->df);
								idf1 = idf1*pf1->tfd; //TFIDF
							}
							re_b->tfidf = idf1;  //Total tf-idf	
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;

							
							pf1 = pf1->next;
					}
					
				}
				else if(pf2)//Append pf2
				{
					while(pf2)
					{
							pFile *re_b = (pFile *)malloc(sizeof(pFile));
							re_b->did = pf2->did; 

							if(!strcmp(e2->term,"merged"))
							{
								idf2 = pf2->tfidf; //Already contains TF_IDF (for merged case)
							}
							else
							{
								idf2 = logl(1.0f*doc_count/s2->near->df);
								idf2 = idf2*pf2->tfd; //TFIDF
							}
							re_b->tfidf = idf2;  //Total tf-idf	
							re_b->next = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->next = re_b;
							prev = re_b;	
							pf2 = pf2->next;
					}

				}
				else//Both Not found
				{
				}

					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
				
			}

			//search for the word
			
			//pop two ele from stack & perform operation
		}
		else// ch =='!'
		{
			//Get Posting List
			//Create New Posting List with docs not in PL
			//set TF_IDF = 0,
			//Push it to stack


			pFile *n_root=NULL,*prev;
			q_t_struct *e1;
			e1 = stk_pop();

			sres *s1 = build_search(e1->term);
			long file_count=1;
			pFile *pf1;
			

		
			pf1 = e1->pf;
			for(;file_count <= doc_count;file_count++)
			{
				if(pf1 && file_count == pf1->did )
				{
					//printf("word present in docID: %ld\n",file_count);
					pf1 = pf1->next;
					continue;
				}

				pFile *re_b = (pFile *)malloc(sizeof(pFile));
				re_b->did =file_count; 
				re_b->tfidf = 1.0f*0;

				re_b->next = NULL;
				if(!n_root)
					n_root = re_b;
				else
					prev->next = re_b;
				prev = re_b;

				
			}

			q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
			strcpy(rec->term,"merged");
			rec->pf = n_root;
			stk_push(rec);
		

		}
		i++;
		

	}//End of while

	if(len==1)/* For 1word only */
	{
		//find TF_IDF for all docs
		q_t_struct *e1;
		e1 = stk_pop();
		if(e1->pf)//If word found
		{
			pFile *pf1,*root;
			root = pf1 = e1->pf;

			sres *s1 = build_search(e1->term);
		
			while(pf1)
			{
				pf1->tfidf = logl(1.0f*doc_count/s1->near->df)*pf1->tfd;
				pf1 = pf1->next;
			}
			q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
			strcpy(rec->term,"merged");
			rec->pf = root;
			stk_push(rec);
			
		}
	}

	//Sort Linked List
	q_t_struct *res;
	res = stk_pop();
	pFile *pf1,*root,*pf2;
	if( res && res->pf)
	{
		pf1 = pf2 = res->pf;
		root = res->pf;
	}
	long t1; float t2;
	while(pf1)
	{
		pf2 = pf1->next;
		while(pf2)
		{
			if(pf1->tfidf < pf2->tfidf)
			{
				//swap
				t1 = pf1->did; t2 = pf1->tfidf;

				pf1->did = pf2->did;  pf1->tfidf = pf2->tfidf;

				pf2->did = t1; pf2->tfidf = t2;
				
			}
			pf2 = pf2->next;
		}
			
		pf1 = pf1->next;
	}


	// Retrieve from stack & display file names


	FILE *r_doc_dic;
	doc_dictionay *r_dd_node = (doc_dictionay *)malloc(sizeof(doc_dictionay));

	r_doc_dic = fopen("../bin/docdic","rb");


	//printf("\n\nResults found in\n");
	while(root)
	{

		fseek(r_doc_dic,0,SEEK_SET); //GO to start
		fseek(r_doc_dic,sizeof(doc_dictionay)*(root->did - 1),SEEK_SET); //Go to req. node
		fread(r_dd_node,sizeof(doc_dictionay),1,r_doc_dic);//Read the node from file & print file name


		//printf("Doc ID: %ld\tTF_IDF: %lf\n",root->did,root->tfidf);
		//printf("%s\tTF_IDF:%f\n",r_dd_node->path,root->tfidf);
		printf("%s\n",r_dd_node->path);
		root = root->next;
		
	}
	fclose(r_doc_dic);
	


}

pFile * loadPF(char term[])
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

void build_query()
{
	FILE *qf;//,*qp,*ftemp,*sp;
	char word1[50];//,word2[80];
	int p1,p2,j;
	int qt=0,i;// qt contains, No. of query terms(from php)	

	char postfx_ip[50];//Contains the input to toPostFix() func
	char q_t_array[20][50];//Contains only the query terms for which search is to be performed
	

	qf=fopen("../bin/stemop","r");//Stemmed output file
	//ftemp=fopen("temp.txt","w+"); //NOT req
	
	p1=0;	
	while(fscanf(qf,"%s",word1)!=EOF) // check 4 stopwords in this loop
	{
		if(strcmp(word1,"and")) 
		{
				strcpy(q1[p1++],word1); 
				q1[p1-1][strlen(q1[p1-1])]='\0';

		}


	}
	qt = p1;

	fclose(qf);
	
	
	/*for(p1=0;p1<qt;p1++)
		printf("%s ",q1[p1]);
	printf("\n\nformed query\n");
	*/


	p1 = 0;
	p2 = 1;
	i=0;
	while(p2 <= qt)
	{
		if(!(strcmp(q1[p1],"or")))
		{
			strcpy(qry_f[i++],"OR");
			qry_f[i-1][strlen(qry_f[i-1])]='\0';
		}
		else if( strcmp(q1[p1],"not") && ( strcmp(q1[p1],"or") && strcmp(q1[p2],"or") ) )
		{
			strcpy(qry_f[i++],q1[p1]);
			qry_f[i-1][strlen(qry_f[i-1])]='\0';
			if(p2 +1 < qt+1)
			{
				strcpy(qry_f[i++],"AND");
				qry_f[i-1][strlen(qry_f[i-1])]='\0';
			}
			//else strcat();
		}
		else
		{
			if(strcmp(q1[p1],"not"))
			strcpy(qry_f[i++],q1[p1]);
			else strcpy(qry_f[i++],"NOT");
			qry_f[i-1][strlen(qry_f[i-1])]='\0';
		}
		p2++;
		p1++;
	}
	//strcpy(qry_f[i],q1[p2]);
	//for(p1=0;p1<i;p1++) // I contains no. of terms after inseting AND, OR in query
		//printf("%s ",qry_f[p1]);

	//printf("\n\ni = %d\n\n",i);



	strcpy(postfx_ip,"");
	j=0;qt=0;
	for(p1=0;p1<i;p1++)
	{
		if( !strcmp(qry_f[p1],"AND") )
			postfx_ip[j++]='*';
		else if( !strcmp(qry_f[p1],"OR") )
			postfx_ip[j++]='+';
		else if( !strcmp(qry_f[p1],"NOT") )
			postfx_ip[j++]='!';
		else
		{
			postfx_ip[j++]=qt+48;// postfx_ip[j]='\0';
			strcpy(q_t_array[qt],qry_f[p1]);

			qt_ar[qt] = (q_t_struct *)malloc(sizeof(q_t_struct));//Create Memory for mapping word, values in postfix
			strcpy(qt_ar[qt]->term,qry_f[p1]);
			qt_ar[qt]->term[strlen(qt_ar[qt]->term)]='\0';
			//printf("\n**copied word %s **\n",qt_ar[qt]->term);
			// Build the Posting list to MM & copy address
			
			//
			qt_ar[qt]->pf = loadPF(qt_ar[qt]->term);



			qt++;
		}
	}
	postfx_ip[j]='\0';

	//printf("\nPostfix Input string: %s\n",postfx_ip);
/*	for(p1=0;p1<qt;p1++)
	{
		printf("Word: %s\t Addr of PL: %u\n",qt_ar[p1]->term,qt_ar[p1]->pf);
		//printf("%s present in docID: %d\n",qt_ar[p1]->term,qt_ar[p1]->pf->did);
	}*/

	//pfop = infix_to_postfix(postfx_ip);
	infix_to_postfix(postfx_ip);//Postfix expression will be created in ../bin/postfix.txt file
	
	
	eval_pfx();
	
	
}
