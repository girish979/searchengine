/*/////////////////////////////////////

Used to Store Tree to HDD

////////////////////////////////////*/


extern node *root;
pFile *pf_hdd_root=NULL;
long pf_hdd_count;
extern long doc_count;
extern double avg_doc_len;

typedef struct node_q
{
	struct node *node_ptr;
	struct node_q *next;
	int bit;
}node_q;

node_q *front=NULL, *rear=NULL;



void smq_insert(node *n1,int bit) //smq send mem q
{
	node_q *nq = (node_q *)malloc(sizeof(node_q));
	nq->next = NULL;
	nq->node_ptr = n1;
	nq->bit = bit;

	if(!rear) rear = nq;
	else
	{
		rear->next = nq;
		rear = nq;
	}
	if(!front)
		front = nq;
}

node_q * smq_delete()
{
	node_q *tmp;
	tmp = front;
	if(front)
		front = front ->next;
	if(!front)
		rear = NULL;

	return tmp;		
		
}


void commit()
{
	FILE *hdd;
	FILE *pf_hdd;

	char pf_name[200];

	pFile *pf_temp;
	
	hdd = fopen("../bin/tree","wb+");
	
	smq_insert(root,1);


	while(front)//There is an element in Q
	{
		
		node_q *pro;
		node *tmp;

		pro = smq_delete();
		tmp = pro->node_ptr;

		//Write to File
		if(tmp)//Write all except NULL node
		{
			fwrite(tmp, sizeof(struct node), 1, hdd);
			//printf("%s\t",tmp->key);
		}

		// Insert left child to Q //pro->node_ptr->lptr; //Left child of processing Node
		if(pro->bit == 1)
		{
			if(tmp)
			{
				if(tmp->lptr)
				{
					if(tmp->keyval > tmp->lptr->keyval)
						smq_insert(tmp->lptr,1);
					else if(tmp->keyval <= tmp->lptr->keyval)
						smq_insert(tmp->lptr,0);
				}
				if(tmp->rptr)
				{
					if(tmp->keyval > tmp->rptr->keyval)
						smq_insert(tmp->rptr,1);
					else if(tmp->keyval <= tmp->rptr->keyval)
						smq_insert(tmp->rptr,0);
				}
//Write Posting List to HDD here
					strcpy(pf_name,"../bin/ifiles/");
					pf_name[strlen(pf_name)]='\0';
					strcat(pf_name,tmp->key);//strcat(pf_name,tmp->bin);
					pf_name[strlen(pf_name)]='\0';

/*
Code to load posting list to MM
					pf_hdd = fopen(pf_name,"rb");
					
					pFile *prev;
					pf_hdd_count=0;
					pf_hdd_root=NULL;
					pf_temp = (pFile *)malloc(sizeof(pFile));
					while(fread(pf_temp, sizeof(struct node), 1, pf_hdd))
					{
						//pf_temp = (pFile *)malloc(sizeof(pFile));
						//fread(pf_temp, sizeof(struct node), 1, pf_hdd);
			

						if(!pf_hdd_root)
							pf_hdd_root = pf_temp;
						else
							prev->next = pf_temp;
						prev = pf_temp;
						pf_hdd_count++;
						pf_temp = (pFile *)malloc(sizeof(pFile));
						
					}
					fclose(pf_hdd);
End of code to load posting list to MM		
*/			
///////////////////////////// Write PL to file in Binary Mod

					pf_hdd = fopen(pf_name,"ab+");
					pf_temp = tmp->pf;
					while(pf_temp)
					{
						//printf("DocID:%d\ttf:%ld--",temp->did,temp->tfd);
						fwrite(pf_temp, sizeof(pFile), 1, pf_hdd);
						//fprintf(pf_hdd,"%ld\t%ld\n",pf_temp->did,pf_temp->tfd);
						pf_temp = pf_temp->next;
					}
		
					fclose(pf_hdd);

///////////////////////////// End of Write PL to file in Binary Mod

					
				// End of writing Posting List to HDD

			}
		}
		
		// Insert right child to Q //pro->node_ptr->rptr; //right child of processing Node
		
		
		free(pro);//Remove Node from Queue
	}

	fclose(hdd);

	hdd = fopen("../bin/hdd_config","w+");
	fprintf(hdd,"%ld\n",doc_count);
	fprintf(hdd,"%f\n",avg_doc_len);
	
	fclose(hdd);

	printf("Writing Doc Dic to HDD\n");
	FILE *doc_dic;
	doc_dictionay *dd_temp;
	doc_dic = fopen("../bin/docdic","ab+");
	dd_temp = doc_dic_root;
	while(dd_temp)
	{
		fwrite(dd_temp,sizeof(doc_dictionay),1,doc_dic);
		dd_temp = dd_temp->next;
	}
	fclose(doc_dic); 

	
}


void load2mm()
{

	front = rear = NULL;

	FILE *hdd;
	node *tmp,*lchild,*rchild;
	node_q *pro;
	hdd = fopen("../bin/tree","rb");

	tmp = (node *)malloc(sizeof(node));
	fread(tmp, sizeof(struct node), 1, hdd);

	if(tmp)//check for necessty	
	root = tmp;


	smq_insert(tmp,1);

	while(front)
	{
		pro = smq_delete();


		tmp = pro->node_ptr;

		tmp->pf_last = NULL; //Setting Posting List to Null
		tmp->pf = NULL;
		if(tmp->lptr)
		{

			lchild = (node *)malloc(sizeof(node));
			fread(lchild, sizeof(struct node), 1, hdd);
			if(tmp->keyval > lchild->keyval)
			{
				tmp->lptr = lchild;
				smq_insert(lchild,1);
			}
			else if(tmp->keyval == lchild->keyval)
			{
				tmp->lptr = tmp;
				free(lchild);
			}
			else
			{
				//search in tree for req node
				node *x;
				x = root;

				while(x)
				{
					if(x->keyval == lchild->keyval)
						break;
					else if( (lchild->bin[x->keyval]) - 48 == 0 )
						x = x->lptr;
					else x = x->rptr;
					
				}
				tmp->lptr = x;

				//end of search
				free(lchild);
			}
		}
		if(tmp->rptr)
		{
			
			rchild = (node *)malloc(sizeof(node));
			fread(rchild, sizeof(struct node), 1, hdd);
			if(tmp->keyval > rchild->keyval)
			{
				tmp->rptr = rchild;
				smq_insert(rchild,1);
			}
			else if(tmp->keyval == rchild->keyval)
			{
				tmp->rptr = tmp;
				free(rchild);
			}
			else
			{
				//search in tree for req node
				node *x;
				x = root;
				
				while(x->keyval != rchild->keyval)//(x)
				{
					//if(x->keyval == rchild->keyval)
						//break; 
					//else 
					if( (rchild->bin[x->keyval]) - 48 == 0 )
						x = x->lptr;
					else x = x->rptr;
					
				}
				tmp->rptr = x;

				//end of search
				free(rchild);
			}
		}
	free(pro);	
	}

	fclose(hdd);
	hdd = fopen("../bin/hdd_config","r");
	fscanf(hdd,"%ld",&doc_count);
	fscanf(hdd,"%f",&avg_doc_len);
	fclose(hdd);

	
}
