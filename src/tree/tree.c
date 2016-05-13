/*//////////////////////////////////////////////////////////////////////////////////////////////////

	PATRICIA Tree Construction & Search Functions


K REDDIGIRISH
10410131
M.Tech 1st Year
k.reddigirish@iitg.ernet.in
/*//////////////////////////////////////////////////////////////////////////////////////////////////

#define BINLEN 128 //length of the binary code
#define KEYSIZE 140//lenght of key is 30
extern char g_key[140], g_bin[129],g_hex[33]; //Accessing External variables
extern char g_ifile[500];//v2.1
extern long g_idocid;

typedef struct dnode // Duplicate Node Structure
{
	char key[KEYSIZE];
	struct dnode *link;
}dnode;

//v2.1b
/*	Create linked list structure for Posting flie	*/
typedef struct pFile
{
	//char doc[200];
	long did;
	long tfd;	//term feq in a document
	float tfidf;
	struct pFile *next;
}pFile;

//End of v2.1b

typedef struct node // Structure of Node
{
	char key[KEYSIZE];//KEY
	int keyval; //KEY INDEX
	char bin[129]; //BINARY Value
	struct node *lptr;
	struct node *rptr;
	struct dnode *next; //Duplicate Node Pointer
	//mar 1 char ifile[150];// Stores the path of Index File v2.1
	long df;//term freq in the collection //v2.1b Mar 1
	struct pFile *pf;//v2.1b //link to first Node of Posting File
	struct pFile *pf_last;//v2.1b points to last Node of Posting File
}node;

typedef struct sres //Local Data Used in Searching..
{
	int found;
	int keyval;
	char info[KEYSIZE];
	int rd; // 0->redundant Node(same key& bitstring)  1-> duplicate Node(same bitstring, diff keys)
	struct node *near;
	//mar 1  char ifile[150];//path of index file v2.1
}sres;




node *root=NULL; //Root Node


///////////////////////////CALCULATE KEY BEGIN///////////////////////////////////////////////////

int cal_key(char par[129],char cur[129],int ncase)
{
	//int l = KEYSIZE;//strlen(cur);
	int i=127;

	if(ncase)	par[128] = '\0'; 
	cur[128]='\0';
	if(ncase)
	{
		if(strcmp(par,cur)==0) return -1;
		else
		for(i=127;i>=0;i--)
		{
			if(par[i] == cur[i])	continue;
			else	break;
		}
	}
	else
		while( !(cur[i]-48)  ) i--;      //i<1 just to prevent -ve value key

	//i = BINLEN -(i+1);	
	return i;
}
///////////////////////////CALCULATE KEY ENDS////////////////////////////////////////////////////


///////////////////////////SEARCH BEGIN//////////////////////////////////////////////////////////

sres * tree_search(char key[KEYSIZE],char binseq[129])
{
	sres *s = (sres *)malloc(sizeof(sres));
	node *cur, *parent;
	int n_key;
	char dir;
	binseq[128]='\0';
	key[strlen(key)]='\0';
	cur = root;
	while(cur)//Traverse & find the nearest node
	{
		parent = cur;
		if( (binseq[cur->keyval ] - 48) == 0)  //BINLEN - cur->keyval -1
			cur = cur->lptr, dir = 'l';
		else
			cur = cur->rptr,dir = 'r';
		if( cur==NULL || cur->keyval >= parent->keyval)//(parent == cur) || cur->keyval>parent->keyval)//|| cur->visited)//node pointing to itself
			break;
	}

	if(cur)
		n_key = cal_key(cur->bin,binseq,1); //for Insertion only to calculate key of new term
	else if(!cur)
		n_key = cal_key("",binseq,0);
	if(cur && n_key ==-1)  //Found either redundant node or a duplicate bitstring
	{
		s->keyval = cur->keyval;

		if( !strcmp(cur->key,key) )//Redundant
		{	//skip doing nothing
			s->found = 1;
			s->rd = 0;


			s->near = cur;

			// Modified on Mar 5
			if(cur->pf)
			{
				if( (g_idocid == cur->pf_last->did) )// Found in list //( !strcmp(g_ifile,cur->pf_last->doc) )//Same
				{
					cur->pf_last->tfd = cur->pf_last->tfd + 1;
				}
				else
				{
					pFile *pf2 = (pFile *)malloc(sizeof(pFile));
					pf2->did = g_idocid;
					pf2->tfd = 1;
					pf2->next = NULL;
					cur->pf_last->next = pf2;//Appending to last node
					cur->pf_last = pf2;
					cur->df = cur->df + 1;
				}
			}	
			else //if(!cur->pf) //Create New node to posting file
			{
				pFile *pf2 = (pFile *)malloc(sizeof(pFile));
			
				
				pf2->did = g_idocid;
				pf2->tfd = 1;
				pf2->next = NULL;
				//perform search & append
				//binarySearch(cur->pf,cur->pf_last,cur->df); //Address of 1st Node,last Node, No. of Nodes
				cur->df = cur->df + 1;

				cur->pf = pf2;// 
				cur->pf_last = pf2;
				

				
			}
	
			//End of v2.1b

		}
		else //Duplicate case practically impossible
		{
			s->found = 1;//0
			s->rd = 1;
			s->near = cur;
         		//printf("duplicate key found for bitstring:%s\n",binseq);
		}
   	}
	//else ??
	else if(cur==NULL || (cur->keyval >= parent->keyval) ) //cur==NULL || parent==cur || cur->keyval>parent->keyval) // Notfound
	{ //            found self loop
		 s->keyval = n_key;
		 if(cur && (strcmp(key,cur->key)==0)) // for search operation
		 {	 s->found = 1; }// NOT REQUIRED??  
		 else
			s->found = 0;
		if(cur)
			{ strcpy(s->info,cur->key); s->info[sizeof(s->info)]='\0';}//,printf("Nearest Node key:%s\n",s->info);
		else
      			strcpy(s->info , "-");//No nearest Node found (search Ends at NULL node)
		//printf("setting key val %d\n",n_key);
	}
	//else// NOT REQUIRED??
		//s->found = 1; // NOT REQUIRED??
	return s;
}

///////////////////////////SEARCH ENDS///////////////////////////////////////////////////////////


///////////////////////////////INSERTION BEGINS//////////////////////////////////////////////////

void insert(int k,char term[KEYSIZE],char binseq[129])
{
	node *cur, *parent;
	char dir;//direcition

	
	
	node *n1 = (node *)malloc(sizeof(node));//New Node created

	pFile *pfnode = (pFile *)malloc(sizeof(pFile));//v2.1b Creating PostingFile Node for every new node in patricial Tree
	//FILE *res;
	//res = fopen("../bin/debug","a+");

	// Copy info to newly created node (Keyword, key, binary, Inverted File path, duplicate link)
	strcpy(n1->key,term);	n1->key[strlen(n1->key)]='\0';
	n1->keyval = k;
	strcpy(n1->bin,binseq); n1->bin[128]='\0';
	n1->next = NULL;// Setting Duplicate pointer to Null


		//start of v2.1b
		
		n1->pf = pfnode;//Node Points to its linked list	
		///n1->tfc = 1;//updating term freq in the collection to 1
		
		n1->df = 1;
		n1->pf_last = pfnode;// pf_last points to last node in posting file linked list


		//strcpy(pfnode->doc,g_ifile);// v2.1b path of currently indexing document;
		pfnode->did = g_idocid;
		pfnode->tfd = 1;//Setting the term freq to 1 initially while creating the node
		pfnode->next = NULL;
		
		//End of v2.1b


	// End of copy info to newly Created Node

	cur = root;
	if(!root)
	{
		if(n1->bin[n1->keyval ] - 48)	n1->lptr = NULL,n1->rptr = n1;
		else	n1->lptr = n1,n1->rptr = NULL;
		root = n1;
	}
	else
	{
		parent = cur;//doubt
		while(k < cur->keyval)
		{
			parent = cur;
			if(n1->bin[cur->keyval ] - 48)       // Check here BINLEN - cur->keyval -1
			{
				cur = cur->rptr;
				dir = 'r';
			}
			else
			{
				cur = cur->lptr;
				dir = 'l';
			}
			if(!cur || parent == cur || cur->keyval > parent->keyval)
				break;
		}

		//setting Pointers
		if(n1->bin[n1->keyval ] - 48)
			n1->lptr = cur,n1->rptr = n1;
		else
			n1->lptr = n1,n1->rptr = cur;
		if(parent->keyval > n1->keyval)
			if(dir=='r')	parent->rptr = n1;
			else	parent->lptr = n1;
		if(cur && cur->keyval > parent->keyval)      //TODAY
			if(dir =='r')	parent->rptr = n1;
			else	parent->lptr = n1;

		if(root->keyval < n1->keyval )
		{
			if(cur != root)     //Exceptional case, eg inserting s in notes
				if(n1->lptr)
					n1->lptr = root;
				else
					n1->rptr = root;
			root = n1;
		}
		//End of setting pointers
	}
/*	if(cur)	fprintf(res,"Nearest-%s\t",cur->key);
	else	fprintf(res,"Nearest-NULL\t",cur->key);
	if(!n1->lptr)	fprintf(res,"lptr-NULL\t");
	else	fprintf(res,"lptr-%s\t",n1->lptr->key); 
	if(!n1->rptr)	fprintf(res,"rptr-NULL\t");
	else	fprintf(res,"rptr-%s\t",n1->rptr->key); 
	fprintf(res,"\n");	fclose(res);
*/
}
///////////////////////////////INSERTION ENDS////////////////////////////////////////////////////


// BUILD TREE (FROM MAIN MEM)

void buildTree()//from main mem
{

	char key[KEYSIZE]; //atmost 20 char key
	char binseq[129]; // 5 Bit binary
	char k2[KEYSIZE];
	int t_key;
	sres *s1;
	//FILE *res;
	
	//res = fopen("../bin/debug","a+");
	strcpy(k2,g_key);  k2[strlen(k2)]='\0';
	strcpy(binseq,g_bin); binseq[128]='\0';
	s1 = tree_search(k2,binseq); // Calculate KeyIndex & other Details
	//fprintf(res,"%s\t%d\t",g_key,s1->keyval);fclose(res);
	if(s1->found == 0)//Need to insert
	{
		//printf("setting key val %d\n",s1->keyval);  
		insert(s1->keyval,k2,binseq); //insert key to the tree
		//printf("Node created\n\n");
	}
	else // Key already present in Tree
	{
		if(s1->rd==0)//Redundant Node
		{
			//res = fopen("../bin/debug","a+");
			//fprintf(res,"Key Already Exists\n"); fclose(res);
			//printf("Key '%s' already exists with keyvalue %d\n\n",k2,s1->keyval);
			//Handling Inverted File v2.1
				/*
					I know the current file of indexing,
					if [same file] then update count for the current indexing file
					else open the inverted file and append the current indexing file to it
				*/
			//End Handling Inverted File v2.1

		}
		else //Handling Duplicates (Practically Impossible case)
		{

			dnode *dtmp,*dtmp2;
			printf("Duplicate key found for bitstring:%s\n",binseq);
			dtmp = s1->near->next;
			dtmp2 = dtmp;
			if(dtmp)
			{
				while(dtmp)
				{	dtmp2=dtmp;
					if( strcmp(dtmp->key,k2)==0 ) break;
					dtmp = dtmp->link;
				}
			}

			if(dtmp==NULL)//Insert if Duplicate is NOT Present
			{
				dnode *d1 = (dnode *)malloc(sizeof(dnode));
				strcpy(d1->key,s1->info);
				d1->link = NULL;
				if(dtmp2)
					dtmp2->link = d1,printf("Duplicate key found & appended to :%s\n",dtmp2->key);
				else s1->near->next =d1,printf("Duplicate key found & appended to :%s\n",s1->near->next->key);
			}


		}

	}		
}
