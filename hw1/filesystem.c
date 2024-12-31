#include <stdio.h>
#include <stdlib.h>
#include<string.h>


/*
 *   ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ 
 *  |   |   |   |   |                       |   |
 *  | 0 | 1 | 2 | 3 |     .....             |127|
 *  |___|___|___|___|_______________________|___|
 *  |   \    <-----  data blocks ------>
 *  |     \
 *  |       \
 *  |         \
 *  |           \
 *  |             \
 *  |               \
 *  |                 \
 *  |                   \
 *  |                     \
 *  |                       \
 *  |                         \
 *  |                           \
 *  |                             \
 *  |                               \
 *  |                                 \
 *  |                                   \
 *  |                                     \
 *  |                                       \
 *  |                                         \
 *  |                                           \
 *  |     <--- super block --->                   \
 *  |______________________________________________|
 *  |               |      |      |        |       |
 *  |        free   |      |      |        |       |
 *  |       block   |inode0|inode1|   .... |inode15|
 *  |        list   |      |      |        |       |
 *  |_______________|______|______|________|_______|
 *
 *
 */


#define FILENAME_MAXLEN 8  // including the NULL char

/* 
 * inode 
 */

typedef struct inode {
  int  dir;  // boolean value. 1 if it's a directory.
  char name[FILENAME_MAXLEN];
  int  size;  // actual file/directory size in bytes.
  int  blockptrs [8];  // direct pointers to blocks containing file's content.
  int  used;  // boolean value. 1 if the entry is in use.
  int  rsvd;  // reserved for future use
} inode;

/* 
 * directory entry
 */

typedef struct dirent {
  char name[FILENAME_MAXLEN];
  int  namelen;  // length of entry name
  int  inode;  // this entry inode index
} dirent;

// file entry
typedef struct file{
  char name[FILENAME_MAXLEN];
  int namelen;
  int inode;
} file;
// datablock structure
typedef struct datablk{
  dirent dir[5];
  file file1[25];
  char data[544];
} datablk;


/*
 * functions
 */
// create file
void create_file(void ** filesystem,char* filename, int file_size){
    printf("Name:  %s\n",filename);
    printf("Size:  %d \n",file_size);
    char *token=strtok(filename,"/");
    char *args[100];
    int count=0;
    int arg=0;
    while(token!=NULL)
    {
	args[count]=strdup(token);
	count+=1;
	token=strtok(NULL,"/");
    }
    printf("Count : %d \n",count);
    for(int i=0; i<count; ++i)
    {
        printf("%s \n",args[i]);
    }
    void *superblock=filesystem;
    datablk *datablock=(datablk*)((char*)filesystem+1024);
    char *freelist=(char *)superblock;
    inode *inode_table=(inode*)superblock+128;
    int exit1=1;
    int check=-1;
    int in_n=0;
    while(arg != count-1)
    {
        int eq=0;
        int frloop=0;
        if (count >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args[arg])==0)                                {
                                        eq=1;
                                        in_n=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args[arg]); 
       			check=frloop;
                        break;
                }
        }
        arg+=1;
    }
    if(check!=0)
    {
	int in_no=-1;
	for(int i=0; i<16; i++)
	{
		if(strcmp(inode_table[i].name," ")==0)
		{
			in_no=i;
			break;
		}
	}
	int inode_root=in_n;
	int index=-1;
	int dirindex=-1;
	int whlcount=-1;
	for(int i=0; i<8; i++)
	{
		index=inode_table[inode_root].blockptrs[i];
		for(int j=0; j<25; j++)
		{
			if(strcmp(datablock[index].file1[j].name,args[arg])==0)
			{
				printf("The file already exists \n");
				whlcount=0;
				break;
			}
		}
		if(whlcount==0)
		{
			exit1=0;
			break;
		}
		if (exit1==0)
		{
			break;
		}
		for(int j=0; j<25; j++)
		{
			if(strcmp(datablock[index].file1[j].name," ")==0)
			{
				dirindex=j;
				break;
			}
		}
		if(dirindex>-1)
		{
			break;
		}
	}
	if(whlcount!=0)
	{
		file newfile;
        	strcpy(newfile.name,args[arg]);
       		newfile.namelen=strlen(args[arg]);
        	newfile.inode=in_no;
        	inode_table[in_no].size=file_size;
        	strcpy(inode_table[in_no].name,newfile.name);
        	int inode_root=in_n;
        	int freeb=0;
        	for(int i=0; i<128; i++)
        	{
                	if(freelist[i]=='0')
                	{
                        	freeb=i;
                        	break;
                	}
        	}
        	printf("Root dir: %s \n",inode_table[inode_root].name);
        	int con=freeb;
        	for(int i=0; i<8; i++)
        	{
                	inode_table[in_no].blockptrs[i]=con;
                	con+=1;
        	}
        	for(int i=0; i<8; i++)
        	{
                	freelist[freeb]='1';
                	freeb+=1;
        	}
		int val=inode_table[inode_root].size - file_size;
        	inode_table[inode_root].size=val;
		printf("Index no: %d \n",index);
		printf("Dir index: %d \n",dirindex);
		char datastring[50];
		sprintf(datastring, "I am a dummy file%d", dirindex);
		if(index>-1 && dirindex>-1)
		{
			strcpy(datablock[index].file1[dirindex].name,newfile.name);
			datablock[index].file1[dirindex].namelen=newfile.namelen;
			datablock[index].file1[dirindex].inode=newfile.inode;
			strcpy(datablock[inode_table[in_no].blockptrs[0]].data,datastring);
		}
		else
		{
			printf("The directory doesn't have enough space. \n");
		}
	}
    }
}

//delete file
void delete_file(void ** filesystem,char* filename){
    printf("Name:  %s\n",filename);
    char *token=strtok(filename,"/");
    char *args[100];
    int count=0;
    int arg=0;
    while(token!=NULL)
    {
	args[count]=strdup(token);
	count+=1;
	token=strtok(NULL,"/");
    }
    for(int i=0; i<count; ++i)
    {
	printf("%s \n",args[i]);
    }
    void *superblock=filesystem;
    datablk *datablock=(datablk*)((char*)filesystem+1024);
    char *freelist=(char*)superblock;
    inode *inode_table=(inode*)superblock+128;
    int in_n=0;
    int check=-1;
    while(arg != count-1)
    {
        int eq=0;
        int frloop=0;
        if (count >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args[arg])==0)
				{
                                        eq=1;
                                        in_n=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args[arg]);
                        check=frloop;
                        break;
                }
        }
        arg+=1;
    }
    if(check!=0)
    {
	int data_list[8];
	int inode=0;
	int frloop=0;
	int exit1=0;
	int index=0;
        for(int i=0; i<8; i++)
	{
		index=inode_table[in_n].blockptrs[i];
		for(int j=0; j<25; j++)
		{
			if(strcmp(datablock[index].file1[j].name,args[arg])==0)
			{
				inode=datablock[index].file1[j].inode;
				strcpy(datablock[index].data," ");
				datablock[index].file1[j].namelen=0;
				datablock[index].file1[j].inode=-1;
				strcpy(datablock[index].file1[j].name," ");
				frloop=1;
				break;
			}
		}
		if(frloop==1)
		{
			exit1=1;
			break;
		}
	}
	if(exit1!=1)
	{
	printf("The file doesn't exist in this directory \n");
	}
	else if(exit1==1)
	{
		inode_table[in_n].size+=inode_table[inode].size;
		for(int i=0; i<8; i++)
		{
			data_list[i]=inode_table[inode].blockptrs[i];
		}
		inode_table[inode].dir=0;
		strcpy(inode_table[inode].name," ");
		inode_table[inode].size=0;
		for(int i=0; i<8; i++)
		{
			inode_table[inode].blockptrs[i]=128;
		}
		inode_table[inode].used=0;
		inode_table[inode].rsvd=0;
		for(int i=0; i<128; i++)
		{
			for(int j=0; j<8; j++)
			{
				if(data_list[j]==i)
				{
					freelist[i]='0';
				}
			}
		}
	}
    }
}

//copy file
void copy_file(void ** filesystem, char* src, char* dest)
{
    printf("Name:  %s\n",src);
    printf("Size:  %s \n",dest);
    char *token=strtok(src,"/");
    char *args[100];
    int count=0;
    int arg=0;
    while(token!=NULL)
    {
        args[count]=strdup(token);
        count+=1;
        token=strtok(NULL,"/");
    }
    char *token1=strtok(dest,"/");
    char *args1[100];
    int count1=0;
    int arg1=0;
    while(token1!=NULL)
    {
        args1[count1]=strdup(token1);
        count1+=1;
        token1=strtok(NULL,"/");
    }
    void *superblock=filesystem;
    datablk *datablock=(datablk*)((char*)filesystem+1024);
    inode *inode_table=(inode*)superblock+128;
    int check=-1;
    int in_n=0;
    while(arg != count-1)
    {
        int eq=0;
        int frloop=0;
        if (count >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args[arg])==0) 
                               {
                                        eq=1;
                                        in_n=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args[arg]);
                        check=frloop;
                        break;
                }
        }
        arg+=1;
    }
    int check1=-1;
    int in_n1=0;
    while(arg1 != count1-1)
    {
        int eq=0;
        int frloop=0;
        if (count1 >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n1].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args1[arg1])==0)                                {
                                        eq=1;
                                        in_n1=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args1[arg1]);
                        check1=frloop;
                        break;
                }
        }
        arg1+=1;
    }
    if(check!=0 && check1!=0)
    {
        int inode=0;
	int inode1=0;
        int frloop=0;
	int frloop1=0;
        int exit1=0;
	int exit2=0;
        int index=0;
	int index1=0;
	int is_file=0;
	int space=0;
        for(int i=0; i<8; i++)
        {
                index=inode_table[in_n].blockptrs[i];
                for(int j=0; j<25; j++)
                {
                        if(strcmp(datablock[index].file1[j].name,args[arg])==0)
                        {
                                inode=datablock[index].file1[j].inode;
                                frloop=1;
                                break;
                        }
                }
                if(frloop==1)
                {
                        exit1=1;
                        break;
                }
        }
        if(exit1!=1)
      	{
        	printf("%s doesn't exist in this directory \n",args[arg]);
        }
	for(int i=0; i<8; i++)
        {
                index1=inode_table[in_n1].blockptrs[i];
                for(int j=0; j<25; j++)
                {
                        if(strcmp(datablock[index].file1[j].name,args1[arg1])==0)
                        {
                                inode1=datablock[index1].file1[j].inode;
                                frloop1=1;
                                break;
                        }
                }
                if(frloop1==1)
                {
                        exit2=1;
                        break;
                }
        }
        if(exit2!=1)
        {
                printf("%s doesn't exist in this directory \n",args1[arg1]);
        }
	else if(exit1==1 && exit2==1)
	{
		if(inode_table[inode].dir==0 && inode_table[inode1].dir==0)
		{
			is_file=1;
		}
		else
		{
			is_file=0;
			printf("Can't handle directories \n");
		}
	}
	if(is_file==1)
	{
		int size_file1=inode_table[inode].size;
		int val2=inode_table[in_n1].size-size_file1;
		if (val2<0)
		{
		printf("Not enough space \n");
		space=1;
		}
	}
	if(space==0)
	{
		for(int i=0; i<8; i++)
		{
			int blkpts=inode_table[inode].blockptrs[i];
			int file2=inode_table[inode1].blockptrs[i];
			strcpy(datablock[file2].data,datablock[blkpts].data);
		}
	}
     }
}

//move file
void move_file(void ** filesystem,char* src, char* dest){
    printf("Name:  %s\n",src);
    printf("Name2:  %s \n",dest);
    char to_delete[20];
    strcpy(to_delete,src);
    char *token=strtok(src,"/");
    char *args[100];
    int count=0;
    int arg=0;
    while(token!=NULL)
    {
        args[count]=strdup(token);
        count+=1;
        token=strtok(NULL,"/");
    }
    char *token1=strtok(dest,"/");
    char *args1[100];
    int count1=0;
    int arg1=0;
    while(token1!=NULL)
    {
        args1[count1]=strdup(token1);
        count1+=1;
        token1=strtok(NULL,"/");
    }
    void *superblock=filesystem;
    datablk *datablock=(datablk*)((char*)filesystem+1024);
    inode *inode_table=(inode*)superblock+128;
    int check=-1;
    int in_n=0;
    while(arg != count-1)
    {
        int eq=0;
        int frloop=0;
        if (count >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args[arg])==0)
                               {
                                        eq=1;
                                        in_n=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args[arg]);
                        check=frloop;
                        break;
                }
        }
        arg+=1;
    }
    int check1=-1;
    int in_n1=0;
    while(arg1 != count1-1)
    {
        int eq=0;
        int frloop=0;
        if (count1 >1)
        {
                for (int i=0; i<8; i++)
                {
                        int val1=inode_table[in_n1].blockptrs[i];
                        for(int j=0; j<5; j++)
                        {
                                if (strcmp(datablock[val1].dir[j].name,args1[arg1])==0)                                {
                                        eq=1;
                                        in_n1=datablock[val1].dir[j].inode;
                                        break;
                                }
                        }
                        if (eq==1)
                        {
                                frloop=1;
                                break;
                        }
                }
                if (frloop!=1)
                {
                        printf("The directory %s does not exist in the path. \n",args1[arg1]);
                        check1=frloop;
                        break;
                }
        }
        arg1+=1;
    }
    if(check!=0 && check1!=0)
    {
        int inode=0;
        int inode1=0;
        int frloop=0;
        int frloop1=0;
        int exit1=0;
        int exit2=0;
        int index=0;
        int index1=0;
        int is_file=0;
        for(int i=0; i<8; i++)
        {
                index=inode_table[in_n].blockptrs[i];
                for(int j=0; j<25; j++)
                {
                        if(strcmp(datablock[index].file1[j].name,args[arg])==0)
                        {
                                inode=datablock[index].file1[j].inode;
                                frloop=1;
                                break;
                        }
                }
                if(frloop==1)
                {
                        exit1=1;
                        break;
                }
        }
        if(exit1!=1)
        {
                printf("%s doesn't exist in this directory \n",args[arg]);
        }
	for(int i=0; i<8; i++)
        {
                index1=inode_table[in_n1].blockptrs[i];
                for(int j=0; j<25; j++)
                {
                        if(strcmp(datablock[index].file1[j].name,args1[arg1])==0)
                        {
                                inode1=datablock[index1].file1[j].inode;
                                frloop1=1;
                                break;
                        }
                }
                if(frloop1==1)
                {
                        exit2=1;
                        break;
                }
        }
        if(exit2!=1)
        {
                printf("%s doesn't exist in this directory \n",args1[arg1]);
        }
	else if(exit1==1 && exit2==1)
        {
                if(inode_table[inode].dir==0 && inode_table[inode1].dir==0)
                {
                        is_file=1;
                }
                else
                {
                        is_file=0;
                        printf("Can't handle directories \n");
                }
        }
	if(is_file==1)
        {
                for(int i=0; i<8; i++)
                {
                        int blkpts=inode_table[inode].blockptrs[i];
                        int file2=inode_table[inode1].blockptrs[i];
                        strcpy(datablock[file2].data,datablock[blkpts].data);
                }
		delete_file(filesystem,to_delete);
        }
     }
}

//create a directory
void create_dir(char* name, void ** filesystem){
    printf("Name:  %s\n",name);
    char *token=strtok(name,"/");
    char *args[100];
    int count=0;
    int arg=0;
    while(token!=NULL)
    {
	args[count]=strdup(token);
	count+=1;
	token=strtok(NULL,"/");
    }
    printf("Count : %d \n",count);
    for(int i=0; i<count; ++i)
    {
	printf("%s \n",args[i]);
    }
    void *superblock=filesystem;
    datablk *datablock=(datablk*)((char*)filesystem+1024);
    char *freelist=(char *)superblock;
    inode *inode_table=(inode*)superblock+128;
    int check=-1;
    int in_n=0;
    while(arg != count-1)
    {
	int eq=0;
	int frloop=0;
	if (count >1)
	{
		for (int i=0; i<8; i++)
        	{
			int val1=inode_table[in_n].blockptrs[i];
                	for(int j=0; j<5; j++)
                	{
                        	if (strcmp(datablock[val1].dir[j].name,args[arg])==0)
                        	{
                                	eq=1;
					in_n=datablock[val1].dir[j].inode;
                                	break;
                        	}
			}
        		if (eq==1)
                	{
                        	frloop=1;
                        	break;
                	}
		}
        	if (frloop!=1)
        	{
                	printf("The directory %s in the given path doesnot exist. \n",args[arg]);
			check=frloop;
                	break;
        	}
	}
	arg+=1;
    }
   if (check!=0)
   {
   	int in_no=-1;
   	for(int i=0; i<16; i++)
   	{
        	if(strcmp(inode_table[i].name," ")==0)
        	{
                	in_no=i;
                	break;
        	}
  	}
  	int inode_root=in_n;
  	int index=-1;
  	int dirindex=-1;
  	int whlcount=-1;
  	for(int i=0; i<8; i++)
  	{
		index=inode_table[inode_root].blockptrs[i];
		printf("Index: %d \n",index);
		for(int j=0; j<5;j++)
		{
			if(strcmp(datablock[index].dir[j].name,args[arg])==0)
			{
				printf("The directory already exists \n");
				whlcount=0;
				break;
			}
		}
		if(whlcount==0)
		{
			break;
		}
		for(int j=0; j<5; j++)
		{
			if(strcmp(datablock[index].dir[j].name," ")==0)
			{
				dirindex=j;
				//printf("Inside dir %d \n",index);
				break;
			}
		}
		if (dirindex>-1)
		{
			//printf("Inside %d \n",index);
			break;
		}
   	}
   	if(whlcount!=0)
   	{
		dirent newdir;
        	strcpy(newdir.name,args[arg]);
        	newdir.namelen=strlen(args[arg]);
        	newdir.inode=in_no;
        	inode_table[in_no].dir=1;
        	strcpy(inode_table[in_no].name,newdir.name);
        	inode_table[in_no].size=8192;
        	int freeb=0;
        	for(int i=0; i<128; i++)
        	{
                	if(freelist[i]=='0')
                	{
                        	freeb=i;
                        	break;
                	}
        	}
		int con=freeb;
        	for(int i=0; i<8; i++)
        	{
                	inode_table[in_no].blockptrs[i]=con;
                	con+=1;
        	}
		for(int i=0; i<8; i++)
        	{
                	freelist[freeb]='1';
                	freeb+=1;
        	}
		printf("Index no: %d \n",index);
		printf("Dir index no: %d \n",dirindex);
		if (index>-1 && dirindex>-1)
		{
			strcpy(datablock[index].dir[dirindex].name,newdir.name);
			datablock[index].dir[dirindex].namelen=newdir.namelen;
			datablock[index].dir[dirindex].inode=newdir.inode;
		}
		else
		{
			printf("The directory doesn't have enough space \n");
		}
		//printf("Name of 2nd dir: %s \n",datablock[8].dir[0].name);
   	}
   }

}

//delete a directory
void delete_dir(char* name, void ** filesystem){
    printf("Name:  %s\n",name);
    char *token=strtok(name,"/");
    char *args[100];
    int count1=0;
    int arg=0;
    while(token!=NULL)
    {
	args[count1]=strdup(token);
	count1+=1;
	token=strtok(NULL,"/");
    }
    for(int i=0; i<count1; i++)
     {
	printf("%s \n",args[i]);
     }
     printf("Count: %d \n",count1);
     printf("arg: %d \n",arg);
     void *superblock=filesystem;
     datablk *datablock=(datablk*)((char*)filesystem+1024);
     char *freelist=(char*)superblock;
     inode *inode_table=(inode*)superblock+128;
     int in_n=0;
     int frloop=0;
     while(arg!=count1-1)
     {
	int eq=0;
	if(count1>=1)
	{
		for(int i=0; i<8; i++)
		{
			int val1=inode_table[in_n].blockptrs[i];
			for(int j=0; j<5; j++)
			{
				if(strcmp(datablock[val1].dir[j].name,args[arg])==0)
				{
					eq=1;
					in_n=datablock[val1].dir[j].inode;
					break;
				}
			}
			if (eq==1)
			{
				frloop=1;
				break;
			}
		}
		if(frloop==0)
		{
			printf("The directory %s in the given path does not exist. \n",args[arg]);
			break;
		}
	}
	arg+=1;
     }
     if(arg==0)
     {
	frloop=1;
     }
     if (frloop==1)
     {
	int in_no=-1;
	if(arg==0)
	{
		in_no=0;
	}
	else
	{
     		in_no=in_n;
	}
     	int index1=-1;
	int exist=0;
	int final=0;
	int empty=0;
	int inode=0;
	for(int i=0; i<8; i++)
	{
		index1=inode_table[in_no].blockptrs[i];
		for(int j=0; j<5;j++)
		{
			if(strcmp(datablock[index1].dir[j].name,args[arg])==0)
			{
				exist=1;
				inode=datablock[index1].dir[j].inode;
				break;
			}
		}
		if (exist==1)
		{
			final=1;
			break;
		}
     	}
	if (exist==0)
	{
		printf("The directory does not exist \n");
	}
	if (final==1)
	{
		char *stackdir[16];
		for(int i=0; i<16; i++)
		{
			stackdir[i] = (char *)malloc(8);
			strcpy(stackdir[i]," ");
		}
		char *stackfile[16];
		for(int i=0; i<16; i++)
                {
			stackfile[i] = (char *)malloc(8);
                        strcpy(stackfile[i]," ");
                }
		int countstack=0;
		int countfile=0;
		char *stackdir2[16];
		for(int i=0; i<16; i++)
                {
                        stackdir2[i] = (char *)malloc(8);
                        strcpy(stackdir2[i]," ");
                }
                int count=0;
		int countval=0;
		while(empty!=1)
		{
			for(int i=0;i<8;i++)
			{
				int val2= inode_table[inode].blockptrs[i];
				for(int j=0; j<5; j++)
				{
					if(strcmp(datablock[val2].dir[j].name," ")!=0)
					{
						strcpy(stackdir[countstack],datablock[val2].dir[j].name);
						countstack+=1;
						strcpy(datablock[val2].dir[j].name," ");
						datablock[val2].dir[j].namelen=0;
						datablock[val2].dir[j].inode=-1;
					}
				}
				for(int k=0; k<25; k++)
				{
					if(strcmp(datablock[val2].file1[k].name," ")!=0)
					{
						strcpy(stackfile[countfile],datablock[val2].file1[k].name);
						countfile+=1;
						strcpy(datablock[val2].file1[k].name," ");
						datablock[val2].file1[k].namelen=0;
						datablock[val2].file1[k].inode=-1;
						strcpy(datablock[val2].data," ");
					}
				}
			}
			countval=countstack;
			while(countval!=0)
			{
				for(int i=0; i<16; i++)
				{
					for(int m=0; m<16; m++)
					{
						if (strcmp(stackdir[m]," ")!=0)
						{
							int equal=0;
							for(int i=0; i<16; i++)
							{
								if (strcmp(stackdir2[i],stackdir[m])==0)
								{
									equal=1;
								}
							}
							if (equal==0)
							{
								strcpy(stackdir2[count],stackdir[m]);
                                                		count+=1;
							}
							if(strcmp(inode_table[i].name,stackdir[m])==0)
							{
								for(int j=0; j<8; j++)
								{
									int val=inode_table[i].blockptrs[j];
									for(int k=0; k<5; k++)
									{
										 if(strcmp(datablock[val].dir[k].name," ")!=0)
										{
											strcpy(stackdir[m],datablock[val].dir[k].name);
											countval+=1;
											strcpy(datablock[val].dir[k].name," ");
                                                					datablock[val].dir[k].namelen=0;
                                                					datablock[val].dir[k].inode=-1;
										}
									}
									for(int l=0; l<25; l++)
									{
                                                                         	if(strcmp(datablock[val].file1[l].name," ")!=0)
                                                                        	{
                                                                                	strcpy(stackfile[countfile],datablock[val].file1[l].name);
                                                                                	countfile+=1;
                                                                                	strcpy(datablock[val].file1[l].name," ");
                                                                                	datablock[val].file1[l].namelen=0;
                                                                                	datablock[val].file1[l].inode=-1;
											strcpy(datablock[val].data," ");
                                                                        	}
									}
								}
								countval-=1;
							}
						}
					}
				}
			}
			if(countval==0)
			{
				//for(int i=0; i<16; i++)
				//{
				//	if(strcmp(stackdir2[i]," ")!=0)
				//	{
				//		printf("%s \n",stackdir2[i]);
				//	}
				//}
				//for(int i=0; i<16; i++)
				//{
				//	if(strcmp(stackfile[i]," ")!=0)
				//	{
				//		printf("%s \n",stackfile[i]);
				//	}
				//}
				break;
			}
		}
		for(int i=0; i<16; i++)
		{
			int list_ptr[8];
			for(int j=0; j<16; j++)
			{
				if(strcmp(inode_table[i].name,stackdir2[j])==0)
				{
					strcpy(inode_table[i].name," ");
					inode_table[i].dir=0;
					inode_table[i].size=0;
					for(int k=0; k<8; k++)
					{
						list_ptr[k]=inode_table[i].blockptrs[k];
						inode_table[i].blockptrs[k]=128;
					}
				}
				for(int l=0; l<128; l++)
				{
					for(int n=0; n<8; n++)
					{
						if(list_ptr[n]==l)
						{
							freelist[l]='0';
						}
					}
				}
			}
		}
		for(int i=0; i<16; i++)
                {
                        int list_ptr[8];
                        for(int j=0; j<16; j++)
                        {
                                if(strcmp(inode_table[i].name,stackfile[j])==0)
                                {
                                        strcpy(inode_table[i].name," ");
                                        inode_table[i].dir=0;
                                        inode_table[i].size=0;
                                        for(int k=0; k<8; k++)
                                        {
                                                list_ptr[k]=inode_table[i].blockptrs[k];
                                                inode_table[i].blockptrs[k]=128;
                                        }
                                }
                                for(int l=0; l<128; l++)
                                {
                                        for(int n=0; n<8; n++)
                                        {
                                                if(list_ptr[n]==l)
                                                {
                                                        freelist[l]='0';
                                                }
                                        }
                                }
			}
		}
		inode_table[inode].size=0;
		inode_table[inode].dir=0;
		strcpy(inode_table[inode].name," ");
		for(int i=0; i<8; i++)
		{
			inode_table[inode].blockptrs[i]=128;
		}
        }
     }
}
//List all files
void list_all(void ** filesystem){
    void *superblock=filesystem;
    inode *inode_table=(inode*)superblock+128;
    for(int i=0; i<16; i++)
    {
	if(strcmp(inode_table[i].name," ")!=0)
	{
		printf("%s \t %d \n ",inode_table[i].name,inode_table[i].size);
	}
    }
}



/*
 * main
 */

int main (int argc, char* argv[]){
  //filesystem
  size_t sz_file= 128 * 1024;
  void *filesystem=malloc(sz_file);
  //superblock
  size_t superblock_size=1024;
  void *superblock=filesystem;
  //datablock
  datablk *datablock=(datablk*)((char*)filesystem+1024);
  // creating freeblock in superblock
  char *freelist=(char *)(superblock);
  for (int i=0; i<128 ; ++i){
    freelist[i]='0';;
  }
  // creating datablocks
  for(int i=0; i<127;i++)
  {
    strcpy(datablock[i].data," ");
    for(int j=0; j<5; j++)
    {
	datablock[i].dir[j].namelen=0;
	datablock[i].dir[j].inode=-1;
        strcpy(datablock[i].dir[j].name," ");
   }
   for(int k=0; k<25; k++)
   {
	datablock[i].file1[k].namelen=0;
	datablock[i].file1[k].inode=-1;
	strcpy(datablock[i].file1[k].name," ");
    }
  }
 // for (int i=0; i<128; i++)
  //{
//	printf("%s",freelist[i].ptr->data);
  //}
   datablk *adr=(datablk*)((char*)filesystem+1024);
  // creating inode table in superblock
  inode *inode_table=(inode*)superblock+128;
  for(int i=0 ; i<16 ;++i)
  {
  	inode_table[i].dir=0;
	strcpy(inode_table[i].name," ");
	inode_table[i].size=0;
	for(int j=0; j<8; ++j)
	{
		inode_table[i].blockptrs[j]=128; //I have stored 128 because there are only 127 datablocks in total
	}

	inode_table[i].used=0;
	inode_table[i].rsvd=0;
 }
// creating root directory
  int count1=0;
  for(int i=0; i<16; i++)
  {
	if (strcmp(inode_table[i].name," ")==0)
	{
		count1=i;
		break;
	}
  }
  printf("Value of inode: %d\n",count1);
  dirent root;
  strcpy(root.name," /");
  root.namelen=2;
  root.inode=count1;
  inode_table[count1].dir=1;
  strcpy(inode_table[count1].name,"/");
  inode_table[count1].size=8192;
  inode_table[count1].used=1;
  for(int i=0; i<8; i++)
  {
	inode_table[count1].blockptrs[i]=i;
  }
  for (int i=0; i<8 ; i++)
  {
	freelist[i]='1';
  }
  for(int i=0; i<8; i++)
  {
	printf("%c",freelist[i]);
	printf("\n");
  }
  printf("Name: %s\n",inode_table[0].name);
  printf("Size: %d \n",inode_table[0].size);
  printf("Directory 0 Name: %s \n",datablock[0].dir[0].name);
  //for (int i=0; i<8; i++)
  //{
    // printf("%d",inode_table[0].blockptrs[i]);
  //}
  //for (int i=0; i<4; i++)
  //{
//	int val=inode_table[0].blockptrs[i];
//	printf("%s \n",datablock[val].dir[0].name);
	//strcpy(datablock[val].dir[0].name,"/");
	//datablock[val].dir[0].namelen=2;
	//datablock[val].dir[0].inode=count1;
   //}
   int exit1=1;
   while(exit1)
   {
	char input[50];
	printf("prompt->");
	fgets(input,50,stdin);
	input[strcspn(input,"\n")]='\0';
	int i=0;
	char *token=strtok(input," ");
	char * args[100];
	while(token!=NULL)
	{
		args[i]=strdup(token);
		i+=1;
		token=strtok(NULL," ");
	}
	if (strcmp(args[0],"exit")==0)
	{
		break;
	}
	else if(strcmp(args[0],"CD")==0)
	{
		char name[20];
		strcpy(name,args[1]);
		create_dir(name,filesystem);
		printf("Name: %s \n",datablock[0].dir[0].name);
		printf("Name of dir2 %s \n",datablock[8].dir[0].name);
		printf("Name of dir3 %s \n",datablock[16].dir[0].name);
		printf("Name of dir4: %s \n",datablock[0].dir[1].name);
	}
	else if(strcmp(args[0],"DD")==0)
	{
		char name[20];
		strcpy(name,args[1]);
		delete_dir(name,filesystem);
	        printf("Name: %s \n",datablock[0].dir[0].name);
                printf("Name of dir2 %s \n",datablock[8].dir[0].name);
	}
	else if(strcmp(args[0],"CR")==0)
	{
		char name[20];
		int size=0;
		strcpy(name,args[1]);
		size=atoi(args[2]);
		create_file(filesystem,name,size);
		printf("File created %s \n",datablock[8].file1[0].name);
		printf("File size %d \n",inode_table[1].size);
		printf("Data file1: %s \n",datablock[16].data);
		printf("Data file2: %s \n",datablock[24].data);
	}
	else if(strcmp(args[0],"DL")==0)
	{
		char name[20];
		strcpy(name,args[1]);
		delete_file(filesystem,name);
		printf("File created %s \n",datablock[8].file1[0].name);
                printf("Directory size %d \n",inode_table[1].size);
	}
	 else if(strcmp(args[0],"LL")==0)
        {
                list_all(filesystem);
        }
	else if(strcmp(args[0],"CP")==0)
	{
		char name1[20];
		char name2[20];
		strcpy(name1,args[1]);
		strcpy(name2,args[2]);
		copy_file(filesystem,name1,name2);
		printf("Data copied: %s \n",datablock[24].data);
	}
        else if(strcmp(args[0],"MV")==0)
        {
                char name1[20];
                char name2[20];
                strcpy(name1,args[1]);
                strcpy(name2,args[2]);
                move_file(filesystem,name1,name2);
                printf("Moved copied: %s \n",datablock[24].data);
        }
     }
  return 0;
}
