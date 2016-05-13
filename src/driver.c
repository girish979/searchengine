/*//////////////////////////////////////////////////////////////////////////////////////////////////

	Main File where program begins


K REDDIGIRISH
10410131
M.Tech 1st Year
k.reddigirish@iitg.ernet.in
/*//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <dirent.h>
#include <math.h>

#include "../include/hash2bin.c"
#include "../src/tree/tree.c"
#include "../src/tree/ssearch.c"




#include "../include/stopwords/sw_driver.c"

long doc_count=0;//Used to keep track of no of docs indexed//Mke it unique no of docs indexed
long doc_len = 0;

double avg_doc_len = (double)0;

//Global key & binary
char g_key[140], g_bin[129],g_hex[33];

		typedef struct mdstr
		{
			char hash[33];
		}mdstr;

struct sw_bkt *bkt[MAX_SW];


char g_ifile[500];//path to Current  file being indexed
long g_idocid;// Current DocID being Indexed

typedef struct doc_dictionay
{
	long docID;
	char path[500];
	long doc_len;
	struct doc_dictionay *next;
}doc_dictionay;
doc_dictionay *doc_dic_root = NULL,*doc_dic_last=NULL;


#include "../src/commit.c"
#include "../src/search/qry.c"
#include "../src/search/bm25.c"

void genHash(char file[])
{
	mdstr *a;
	char term[KEYSIZE]; // max chars in word
	FILE *fp;
	
	fp = fopen(file,"r+");
		
	if(!fp)
		printf("--Unable to open file: %s--\n\n",file);
	else
	{
		//printf("opened");
		doc_len = 0;
		while(fscanf(fp,"%s",term) != EOF) //Read word by word in file	
		{
			/*
				Do Stemming Optimization
			*/
			
			if(isSW(term)) continue;

			term[strlen(term)]='\0';
			doc_len++;

			a = (mdstr *)do_test(term);//finding md5 hash
			strcpy(g_hex,"\0");
			strcpy(g_hex,a->hash);
			g_hex[32]='\0';
			free(a);

			//printf("Hex value: %s",g_hex);
			strcpy(g_key,"\0");
			strcpy(g_key,term);
			g_key[strlen(g_key)]='\0';
			toBinary();//hash2Bin();
			
			buildTree();
			
		}
		printf("Finished Indexing %s\n",g_ifile);
		fclose(fp);

	}
}


int dir_filter(const struct direct *entry) 
{
	if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
		return 0;
 
	return 1;
}



 
void dir_read(const char dir[]) 
{
	struct direct **files;
	long nfiles, x;
	int dir_filter(const struct direct *entry);
	char path[500];
	char fnames[500];

//
	//FILE *doc_dic;
//
 
	nfiles = scandir(dir, &files, dir_filter, alphasort);
  
  
	for(x=0;x<nfiles;x++)
	{
		if( files[x]->d_type == DT_DIR )
	  	{
	  	  	strcpy(path,"\0");
	  	  	strcpy(path,dir);
			strcat(path,"/");
		        strcat(path,files[x]->d_name);
		        dir_read(path);
	  	  	
	 	 }
	}
 
  	if(nfiles <= 0);
    //return 0;
 
	for(x=0;x<nfiles;x++)
		if(files[x]->d_type != DT_DIR)
		{
			strcpy(fnames,"\0");
	    		sprintf(fnames,"%s/%s", dir, files[x]->d_name);
			strcat(fnames,"\0");
	    		//printf("\nIndexing File : %s",fnames);
//////////////Doc Dic start
/*
			doc_dic = fopen("../bin/docdic","ab+");
		
			doc_dictionay *dd_node = (doc_dictionay *)malloc(sizeof(doc_dictionay));

			dd_node->docID = ++doc_count;
			strcpy(dd_node->path,fnames);
			dd_node->path[strlen(dd_node->path)]='\0';
			fwrite(dd_node,sizeof(doc_dictionay),1,doc_dic);
			
			fclose(doc_dic); 
			free(dd_node);
*/

//Doc Dic


			++doc_count;


			
			
			
			



			g_idocid = doc_count;

			//v2.1
			strcpy(g_ifile,"\0");//Remove previous contents
			strcpy(g_ifile,fnames); //copying current indexing file to global variable g_ifile
			g_ifile[strlen(g_ifile)]='\0';
			//printf("Indexing File gfile name %s\n",g_ifile);
			//End of v2.1

			/* */
		
			fflush(stdin);
			do_stem(fnames);// stemmed o/p will be in ../bin/stemop
			genHash("../bin/stemop");  //For every File read Start building Tree

			//Write Doc_Dic
			doc_dictionay *dd_node = (doc_dictionay *)malloc(sizeof(doc_dictionay));
			dd_node->docID = doc_count;
			dd_node->doc_len = doc_len;
			strcpy(dd_node->path,fnames);
			dd_node->path[strlen(dd_node->path)]='\0';
			dd_node->next = NULL;
			if(doc_dic_root)
			{
				doc_dic_last->next = dd_node;
				doc_dic_last = dd_node;

			}
			else
			{
				doc_dic_root = dd_node;
				doc_dic_last = doc_dic_root;
			}

			//
			
			avg_doc_len = (double) ( ((  avg_doc_len*(doc_count-1) ) + doc_len )/doc_count);
	



			
		}
}

void indexer(int argc,char *argv[])
{
	
	char command[500];
	char dir[500];

	FILE *conf;

	//FILE *doc_dic;

	conf = fopen(argv[2],"r");//conf = fopen("../src/idx.conf","r");
	strcpy(dir,"\0");	
	while(fscanf(conf,"%s",dir)!=EOF)
	{
		
		dir[strlen(dir)]='\0';
		dir_read(dir);//Send dir path to input

		
		strcpy(dir,"\0");
	}
	fclose(conf);
	//sleep(1);
	printf("\nStoring Tree & Posting Files to Disk\n");
	commit();
}

void process_query(int argc,char *argv[])
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

	build_query();
/*
	q_file = fopen("../bin/stemop","r+");
	qt_count = -1;
	while(fscanf(q_file,"%s",query_term)!=EOF)
	{
		query_term[strlen(query_term)]='\0';
		if(isSW(query_term)) 
		{
			printf("%s is a stop word\n",query_term);
			continue;
			
		}
		else
		{
			//apply md5
			//search for word in posting file
			//get the node , get the posting list
			//perform query [just display all files containing the word now]

			mdstr *a1;
			strcpy(g_key,query_term);
			g_key[strlen(g_key)]='\0';
			a1 = (mdstr *)do_test(g_key);//Generate MD5
			strcpy(g_hex,a1->hash);
			g_hex[32]='\0';
			free(a1);
			toBinary();
			sres *srch ;
			srch = tree_ssearch(g_key,g_bin);
			if(srch->found == 0)
				printf("\nkey '%s' is NOT found in the tree\n",g_key);
			else if(srch->found ==1)
			{
				printf("\nKey '%s' found in the tree with keyvalue %d, %d\n\n",g_key,srch->keyval,srch->near->keyval);
				qt_count++;//Incriment Query Terms
				qt_pfs[qt_count] = srch->near->pf;
				dfs[qt_count] = srch->near->df;


///Test Search				printf("%s is present in the Docs with ID & Freq\n");
				printf("DOC ID\tTerm Freq\nPath\n");
			
				FILE *qt_pf;  
				pFile *ret_pf_node;
				strcpy(pf_name,"../bin/ifiles/");	pf_name[strlen(pf_name)]='\0';
				strcat(pf_name,g_bin);		pf_name[strlen(pf_name)]='\0';

				qt_pf = fopen(pf_name,"rb"); // Opened the Posting File
				ret_pf_node = (pFile *)malloc(sizeof(pFile));

				while(fread(ret_pf_node, sizeof(struct pFile), 1, qt_pf))
				{
					
					// Get the actual file name r_doc_dic
					fseek(r_doc_dic,0,SEEK_SET); //GO to start
					fseek(r_doc_dic,sizeof(doc_dictionay)*(ret_pf_node->did - 1),SEEK_SET); //Go to req. node
					fread(r_dd_node,sizeof(doc_dictionay),1,r_doc_dic);//Read the node from file & print file name
		
					printf("%ld\t%ld\n",ret_pf_node->did,ret_pf_node->tfd);
					printf("from bfile: ID: %ld path:%s\n\n",r_dd_node->docID,r_dd_node->path);
					
					
				}

				fclose(qt_pf);
///End o Test Search
			}//else if(srch->found ==1)
			
		}//else if !stopword
	}// End of While	
*/
	
	
}

void main(int argc,char *argv[])
{

	FILE *hdd_load;
	buildSW();
	if(argc < 3)
		printf("No input specified / Invalid Command\n");
	
	else if(argv[1][0] =='-' && argv[1][1]=='i')
	{
		hdd_load = fopen("../bin/tree","rb");
		
		if(hdd_load)
		{
			fclose(hdd_load);
			load2mm();
			//buildSW();
		
			printf("Tree Built sucessfully\n");
			printf("retrieved vlaue:%ld",doc_count);
			indexer(argc,argv);	
		}
		else
			indexer(argc,argv);	
	}

	else if(argv[1][0] =='-' && argv[1][1]=='s')
	{
		hdd_load = fopen("../bin/tree","rb");
		if(hdd_load)
		{
			fclose(hdd_load);
			load2mm();
		
			//printf("Tree Built sucessfully\n");
			//printf("retrieved vlaue:%ld\n",doc_count);
			
		}

		if(argv[2][0] =='-' && argv[2][1]=='b')
			process_query_bm25(argc,argv);
		else
			process_query(argc,argv);							

	
	}// -s

}
