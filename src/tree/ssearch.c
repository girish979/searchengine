extern node *root;

sres * tree_ssearch(char key[KEYSIZE],char binseq[BINLEN])
{
	sres *s = (sres *)malloc(sizeof(sres));
	node *cur, *parent;
	int n_key;
	char dir;

	cur = root;
	while(cur)//Traverse & find the nearest node
	{
		parent = cur;
		if( (binseq[cur->keyval] - 48) == 0)
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
			strcpy(s->info,cur->key);//,printf("Nearest Node key:%s\n",s->info);
		else
      			strcpy(s->info , "-");//No nearest Node found (search Ends at NULL node)
		//printf("setting key val %d\n",n_key);
	}
	//else// NOT REQUIRED??
		//s->found = 1; // NOT REQUIRED??
	return s;
}

