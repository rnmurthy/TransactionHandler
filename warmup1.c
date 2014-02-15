
#include "my402list.h"
#include "cs402.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct tagdataObj
{
	char t_type;
	unsigned long time; /*time in int after epoch*/
	int cents;
	char t_details[1024];
}t_dataObj;

static FILE* fout; /*SET FILE PTR FOR ERRORS*/ 

void freeObjData(My402List* anchorElem)
{
	My402ListElem* node = NULL;
	if(anchorElem == NULL)
	{
		return;
	}
	for(node = My402ListFirst(anchorElem); node != NULL; node = My402ListNext(anchorElem,node))
	{
		if(node->obj != NULL)
		free(node->obj);
	}
	return;
}
void format_number(int num, char* buf, char*pt )
{
/*	char buf[15];*/

	sprintf(buf,"%d",num);
	int c = (int)(strlen(buf)-2);
	if(c > 0)
	{
/*		char pt[3];*/
		pt[2]='\0';
		pt[1]=buf[c+1];
		pt[0]=buf[c];
		buf[c] = '\0';
	}
	else if(c == 0)
	{
		pt[2] = '\0';
		pt[1] = buf[1];
		pt[0] = buf[0];
		strcpy(buf,"0");
	}
	else if(c ==-1)
	{
		strcpy(pt,"0");
		pt[2] = '\0';
		pt[1] = buf[0];
		/*pt[0] = '0';*/
		strcpy(buf,"0");

	}
}

void add_commas(int num,char* buf)
{
	int mod1 = 0;
	int mod2 = 0;

	if(num<1000)
	{
		sprintf(buf,"%d",num);
	}
	else if(num < 1000000)
	{	mod1 = num%1000;	
		sprintf(buf,"%3d,%03d",num/1000, mod1);
	}
	else if(num < 10000000)
	{
		mod1 =num%1000;
		num = num/1000;
		mod2=num%1000;
		num = num/1000;
	sprintf(buf,"%d,%03d,%03d",num, mod2,mod1);	
	}

}

void formatOutput(char * heading[])
{
int i,j,cnt=0;
for(i=0,j=0;i<80;i++)
	{
		
		if(i==0||i==2||i==3||i==4)
		{
			
			if(j<4)
			{
				printf("%c%c%s",'|',' ',(char *)heading[j]);
				if(i == 0)
				{	
					cnt =13 - strlen(heading[0]);
					while(cnt>0)
					{printf("%c",' ');cnt--;}
					
				}		
				else if(i==2)
				{
					cnt = 24- strlen(heading[1]);
					while(cnt>=0)
					{printf("%c", ' ');cnt--;}
				}
				else if(i==3)
				{
					cnt = 14- strlen(heading[2]);
					while(cnt>=0)
					{
						printf("%c", ' ');
						cnt--;
					}
				}
				else if(i==4)
				{
					cnt = 15- strlen(heading[3]);
					while(cnt>0)
					{printf("%c", ' ');cnt--;
						}
					if(cnt==0)
					printf("%c", '|');
					break;
				}

			 j++;	
			}/* if j<4*/
						
		}
		else
			printf("%c",' ');
	}/*end of for*/
	printf("\n");
 return;
}

void formatBoundary()
{
	int i;
		for(i=0;i<=79;i++)
	{	
		if(i==0||i==18||i==45||i==62||i==79)
		{	
			printf("%c",'+');
						
		}
		else
			printf("%c",'-');
	}
  printf("\n");	
}

int checkFormat(char * dataBuf )
{
	char * charPtr = dataBuf;
	int cnt = 0;
	while(*charPtr !='\0')
	{
		if(*charPtr == '\t')
		{	
			cnt++;
		}
		
		charPtr++;
	}
	if(cnt == 3)
		return TRUE;
	else
		return FALSE;

}




void balance(int amt, char sign, int i, int* balptr)
{
	if(i == 0)
	{
		return;
	}
	switch(sign)
	{
	case '+': *balptr = (*balptr + amt);	
			break;
	case '-': *balptr = (*balptr - amt);
				break;
	default: fprintf(fout,"Invalid transaction type in str while calculating balance\n");
				/*TODO:free memory*/
				exit(1);
				break;	/*Have ensured wrong input will not be taken.
					If mem corruption, then print condition should help in debug*/				
	}

}

int printTransaction(My402List* txnList)
{
	
	My402ListElem* elem =My402ListFirst(txnList);
	int i=0;
	time_t t;
	/*float amt=0;*/
	int j;
	/*array of ptrs*/
	char* heading[4]={"      Date     ","Description     ","        Amount ","       Balance "};

   	formatBoundary();	
	formatOutput(heading);
	formatBoundary();

	t_dataObj* temp = (t_dataObj*)(elem->obj);
	int bal = ((t_dataObj*)temp)->cents;/*amt brought abt in 1st trxn*/
	if(temp->t_type == '-')
		bal = bal * (-1);
	/*bal = bal/100;/ *convert back to dollars*/
	for(i=0;elem!=NULL;elem=My402ListNext(txnList,elem),i++)
	{
		t_dataObj* temp = (t_dataObj*)(elem->obj);
		/*Get the time into timeBuf*/
		t= (time_t)temp->time;
		char timeBuf[50];
		int rep = 0;
		strncpy(timeBuf,ctime(&t),50);
		timeBuf[strlen(ctime(&t))-1] = '\0';
		/*Format time stamp as per spec*/
		for(j=strlen(timeBuf)-4, rep =strlen(timeBuf)-13;j<=strlen(timeBuf);j++,rep++)
		{
			timeBuf[rep] = timeBuf[j];
		}
		timeBuf[15] = '\0';
		char * pt0 = (char*)malloc(15);
                strncpy(pt0, timeBuf,15);
                heading[0]=pt0;

		/*heading[0] = timeBuf;*/

		/*Get the descriptionBuf*/
		char descrpBuf[25];
	/*	if(strlen(temp->t_details)>23){
			printf("Keeping only 4 bytes of Description field\n");
		}*/	
		strncpy(descrpBuf,temp->t_details,25);	
		descrpBuf[24] ='\0';
		char * pt1 = (char*)malloc(sizeof(descrpBuf));
                strncpy(pt1, descrpBuf,25);
                heading[1]=pt1;
 
		/*heading[1]=descrpBuf;*/

		/*Get amount field*/
		
		/*amt = (float)temp->cents/100.00;*/
		char amtBuf[15];
		if((temp->cents/100) < 10000000)
		{
		char a[15];
		char p[3];
		format_number(temp->cents,a,p);
		int whole,point;
		sscanf(a,"%d",&whole);
		sscanf(p,"%d",&point);/*
		int whole = (int)amt;
		int point = (amt - whole)*100;*/
		char cmsep[10];
		add_commas(whole,cmsep);
		if((temp->t_type) == '+' )
			sprintf(amtBuf," %9s.%02d ",cmsep,point);
		else
			 sprintf(amtBuf,"%c%9s.%02d%c",'(',cmsep,point,')');
		}
		else
		{
		if((temp->t_type) == '+' )
			strncpy(amtBuf,"  ?,???,???.??",14);
		else
			strncpy(amtBuf,"(?,???,???.??"")",14);
		
		}
		/*sprintf(amtBuf,"%10.2f",amt);*/
		amtBuf[14] = '\0';
		char * pt2 = (char*)malloc(sizeof(amtBuf));
                strncpy(pt2, amtBuf,15);
                heading[2]=pt2;

		

		/*Calculate balance*/
		balance(temp->cents,((t_dataObj*)temp)->t_type,i,&bal);	
		float bd = (float)bal/100.00;
		char balBuf[15];
		int ck = FALSE;
		if(bal < 0){
		 ck=TRUE;
		bal = bal*(-1);
		}
		if(bd < 10000000)
		{
		char a[15];
		char p[3];
		format_number(bal,a,p);
		int whole,point;
		sscanf(a,"%d",&whole);
		sscanf(p,"%d",&point);/*
		int whole = (int)bd;
		int point = (bd - whole)*100;*/
		char cmsep[10];
		add_commas(whole,cmsep);
		if(ck==FALSE)
			sprintf(balBuf," %9s.%02d ",cmsep,point);
		else
		{
			 sprintf(balBuf,"%c%9s.%02d%c",'(',cmsep,point,')');
			bal = bal *(-1);
		}	
		}
		else
		{
		if(ck == FALSE )
			strncpy(balBuf," ?,???,???.?? ",14);
		else{
			strncpy(balBuf,"(?,???,???.??"")",14);
			bal = bal *(-1);
			}
		}
		balBuf[14] = '\0';
		char * pt3 = (char*)malloc(sizeof(balBuf));
                strncpy(pt3, balBuf,15);
                heading[3]=pt3;

		
   formatOutput(heading);
	free(pt0);
	free(pt1);
	free(pt2);
	free(pt3);			
	}/*end of processing for*/ 
	/*printf("\n");*/
	formatBoundary();
 return TRUE;
}



int HandleData(char* buf, t_dataObj* newNode)
{
	int retVal = FALSE;
	/*making a local copy of buffer for strtok use*/
	char data[1024];
	char* dptr = NULL;
	strncpy(data,buf,1024);
	
	/*Get type */
	dptr = strtok(data,"\t");
	if(dptr == NULL)
	{
		return retVal;
	}
	/*validating type field*/
	if((int)strlen(dptr)>=2)
	{
		fprintf(fout,"Error while retrieving the type field: Invalid type\n");
		return retVal;
	}
			
	if(*dptr == '+'|| *dptr == '-')
		newNode->t_type = *dptr;
	else 
	{
		fprintf(fout,"Error while retrieving the type field: Invalid type\n");
		return retVal;
	}
	/*Get time stamp*/
	dptr = strtok(NULL,"\t");
	if(dptr == NULL)
	{	
		fprintf(fout,"Error while retrieving the time stamp\n");
		return retVal;
	}
	if(strlen(dptr)>=11 )
	{
		fprintf(fout,"Error while retrieving the time stamp: size overflow\n");
		return retVal;	
	}
	if(NULL != strchr(dptr,'-'))
	{
		fprintf(fout,"Error while retrieving the time stamp: negative time stamp\n");
		return retVal;		
	}
	
	char * ckp = dptr;
	int p =(int)strlen(dptr);
	while(p>0)
	{
		if((*ckp <48) || (*ckp > 57))
		{
		fprintf(fout,"Error while retrieving the time stamp: alpha numeric stamp\n");
		return retVal;	
		}
		p--;
		ckp++;
	}
	newNode->time =strtoul(dptr,0,10);

	if(newNode->time >= 4294967295u)
	{
		fprintf(fout,"Error while retrieving the time stamp: size overflow\n");
		return retVal;
	}
	time_t currtime;
	time(&currtime);
	if((unsigned long)currtime < newNode->time)
	{
		fprintf(fout,"Invalid time stamp:Beyond current time \n");
		return retVal;
	}

 	
	dptr = strtok(NULL,"\t");
	if(dptr == NULL)
	{
		return retVal;
	}
	{
	char * ckp = dptr;
	int p =(int)strlen(dptr);
	while(p>0)
	{
		if((*ckp <48) || (*ckp > 57))
		{
		if((*ckp != 46))
			{
			fprintf(fout,"Error while retrieving the time stamp: alpha numeric stamp\n");
			return retVal;	
			}
		}
		p--;
		ckp++;
	}
	}


	float temp=0;
 	sscanf(dptr,"%f",&temp);	
/*	double temp = atof(dptr);*/	
	/*transaction amount cannot be negative*/
	if(temp < 0)
	{	
		fprintf(fout,"Error while retrieving the amount: negative txn amt\n");
		return retVal;
	}
	if((int)temp >= 10000000)
	{
		fprintf(fout,"Error while retrieving the amount: size overflow\n");
		return retVal;
	}
	if(*dptr == '.')
	{
		fprintf(fout,"Invalid Input: minimum 1 number before decimal point\n");
			return retVal;

	}

	char* pptr = strchr(dptr,'.');
	if(pptr !=NULL)
	{	
		
		pptr+=2;
		if(*pptr == '\0')
		{
			fprintf(fout,"Invalid Input: 2 decimal places necessary\n");
			return retVal;
		}

		pptr++;
		if(*pptr != '\0')
		{
			fprintf(fout,"Invalid Input: more than 3 decimal places not allowed\n");
			return retVal;
		}
		if(((int)strlen(dptr)-3 )> 7)
		{
		fprintf(fout,"Invalid Input: more than 10 million not allowed\n");
			return retVal;

		}

	}
	else
	{
		fprintf(fout,"Invalid Input: provide amt in decimal notation to 2 places\n");
			return retVal;	
	}
	
/*	sprintf(buf,"%d",num);
*/	int c = (int)(strlen(dptr)-2);
	char pt[3];
	pt[2]='\0';
	pt[1]=dptr[c+1];
	pt[0]=dptr[c];
	dptr[c] = '\0';
	int whole;
	int point;
	sscanf(dptr,"%d", &whole);
	sscanf(pt,"%d",&point);
	 	if(whole == 0 && point ==0)
		{
			fprintf(fout,"Invalid Input: provide non-zero positive amount\n");
			return retVal;	
		}				
		newNode->cents = whole*100+point;
	
	 	
	
	/*convert to int with 2 digit precision got by (* 100)* /
	if(temp < 10000000)
	{
	
		/ *int pr1 = (int)(temp *1000);	
		if((temp*1000 -pr1)*10 > 9)
		newNode->cents = (temp * 100) +1;
		else* /
		newNode->cents = (temp * 100);
	}
	else/ *to indicate >10million case* /
     	 newNode->cents = -1;*/
	dptr = strtok(NULL,"\0");
	if(dptr == NULL)
	{
		return retVal;
	}
	
	/*removing white spaces*/
	while(*dptr == ' ')
	{	
		dptr++;	
	}
	if(*dptr == '\0')
	{
		fprintf(fout,"Empty description field\n");
		return retVal;
	}
	/*copying the entire field of description[1024 max] here to have a ref to it in str. While formating o/p, it will be truncated to 23bits*/
	strncpy(newNode->t_details,dptr,strlen(dptr));
	newNode->t_details[strlen(dptr)+1]='\0';
	
	return (retVal=TRUE); 	
	
}
void timeConv(t_dataObj * temp, char * timeBuf)
{
			time_t t= (time_t)temp->time;
			int rep = 0;
			int j;
			strcpy(timeBuf,ctime(&t));
			timeBuf[strlen(ctime(&t))-1] = '\0';
		/*Format time stamp as per spec*/
			for(j=strlen(timeBuf)-4, rep =strlen(timeBuf)-13;j<=strlen(timeBuf);j++,rep++)
			{
				timeBuf[rep] = timeBuf[j];
			}
			timeBuf[15] = '\0';
}

int sort_addList(My402List* txnList, t_dataObj* newNode)
{
	int retVal = FALSE;
	My402ListElem* currElem = NULL;
	if(TRUE == My402ListEmpty(txnList))
	{
		/*adding the 1st element. So just insert*/
		if(FALSE == (retVal =My402ListAppend(txnList,(void*)newNode)))
		{
			fprintf(fout,"Could not add the transaction with details:\n %s \nto the list as the 1st txn\n",(char *)newNode->t_details);
		
		return FALSE;
		}
	}
	/*List is already populated. Find the right place to insert the node*/
	else
	{
		
		for(currElem = My402ListLast(txnList); currElem !=NULL; currElem = My402ListPrev(txnList, currElem))
		{	/*greater the time, more recent is the node*/
		/*Get the time into timeBuf*/
			t_dataObj* ctemp = (t_dataObj*)currElem->obj;
			/*char ctimeBuf[50];
			char newTimeBuf[50];
			timeConv(ctemp, ctimeBuf);
			timeConv(newNode, newTimeBuf);
			if( 0 ==strcmp(ctimeBuf,newTimeBuf))
			{
				fprintf(fout,"Error: Reporting another transaction for the same time stamp\n ");
				return FALSE;

			}*/
			if((unsigned int*)ctemp->time < (unsigned int*)newNode->time)
			{
				My402ListInsertAfter(txnList,(void*)newNode,currElem);
				return TRUE;;
			}
			else if((unsigned int*)ctemp->time == (unsigned int*)newNode->time)
			{
				fprintf(fout,"Error: Reporting another transaction for the same time stamp\n ");
				return FALSE;
			}
		}
		if(currElem == NULL)
		{
			/*oldest transaction*/
			My402ListPrepend(txnList,(void*)newNode);
		}	
	return TRUE;

	}
return retVal;
}


/*main() is allocating memory for the node to be added in the list which is to be freed at exit*/

int main(int argc, char* argv[])
{
	My402List txnList;
	My402ListInit(&txnList);
	FILE* fp =NULL;
	int retVal = TRUE;
	char dataBuf[1026];
	memset((void *)dataBuf,0,sizeof(dataBuf));
	t_dataObj* newNode = NULL;
	fout = stderr;	

	if(argc == 3)
	{
		if(0 == strcmp(argv[1],"sort"))
		{	
			/*printf("Reading from file %s\n",argv[2]);*/
			fp = fopen(argv[2], "r");
			if(fp == NULL)
			{
				fprintf(fout,"Cannot open the file mentioned in the argument: %s\n", strerror(errno));
				exit(FALSE);
			}
			struct stat s;
			if(stat(argv[2], &s) == 0)
			{
				if(S_ISDIR(s.st_mode))
				{
				fprintf(fout,"Cannot open : Directory path\n");
				exit(FALSE);
	
				}
 			}
			
		}
		else
		{
			fprintf(fout,"Invalid command %s\n", argv[1]);
			exit(FALSE);
		}
	}
	else if(argc == 2)
	{
		if(0 == strcmp(argv[1],"sort"))
		{
			/*printf("Reading from stdin\n");*/
			fp = stdin;
			if(fp == NULL)
			{
				fprintf(fout,"Cannot open the stdin\n");
				exit(1);
			}
		}
		else
		{
				fprintf(fout,"Invalid command %s\n", argv[1]);
				exit(FALSE);
		}

		
	}
	else
	{
		fprintf(fout,"Invalid command line arguments\nPlease enter:\nwarmup sort <tfile>  :[to read from tfile] or\nwarmup sort :[to read from stdin]\n");
		exit(FALSE);
	
	}

	while(NULL != fgets(dataBuf,sizeof(dataBuf),fp))/*reads until new line*/
	{
		/*remove new line char from fgets input*/
		if(dataBuf[strlen(dataBuf)-1] == '\n')
		{
			dataBuf[strlen(dataBuf)-1] = '\0';
		}
		/*check if size of line is greater than 1024*/
		if(strlen(dataBuf)>1024)
		{
			fprintf(fout,"Line Error: Number of char in the line(>%d) exceeds limit of 1024\n", (int)strlen(dataBuf));	
			exit(FALSE);
		}
		/*check the line for format compliance*/
		if(FALSE == (retVal = checkFormat(dataBuf)))
		{
			fprintf(fout,"Malformed line: Line entered not compliant to specification \n");
			exit(FALSE);	
		}
		
		newNode = (t_dataObj*)malloc(sizeof(t_dataObj));
		if(newNode == NULL)
		{
			fprintf(fout,"Malloc failed: Creating Object data structure\n");
			exit(1);
		}
	
		if(FALSE == (retVal=HandleData(dataBuf,newNode)))
		{
			fprintf(fout,"Malformed data: Error while populating the data obj structure\n");
			if(txnList.num_members > 0)
			{
				free(newNode);
				freeObjData(&txnList);
				My402ListUnlinkAll(&txnList);
	
			}
			else
				free(newNode);
			exit(FALSE);
		}
		if(FALSE == (retVal = sort_addList(&txnList,newNode)))
		{
			fprintf(fout,"Error in adding to and sorting in the list\n");
			free(newNode);
			freeObjData(&txnList);
			My402ListUnlinkAll(&txnList);
			exit(FALSE);
						
		}
		/*if(fp == stdin)
			break;*/
	}
	if(fp != stdin)
		fclose(fp);
	if(txnList.num_members > 0)
	{ 
		if( retVal == TRUE)
		{
			printTransaction(&txnList);
		}	
	
		freeObjData(&txnList);
		My402ListUnlinkAll(&txnList);
	}
	return TRUE;

}






