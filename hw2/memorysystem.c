#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MEMSIZE 500


int no_of_frames = 0;
int stack_size=0;
int heap_size=0;
int no_of_buffers =0;
int d_buffer=0;

typedef struct framestatus {
	int number;               // frame number
    char name[8];             // function name representing the frame
	int functionaddress;      // address of function in code section (will be randomly generated in this case)
	int frameaddress;         // starting address of frame belonging to this header in Stack
	char used;                 // a boolean value indicating wheter the frame status entry is in use or not
}framestatus;

typedef struct  frame
{
	int size;
	char data[15];

}frame;

typedef struct freelist {
	int start;         // start address of free region
	int size;                 // size of free region
	struct freelist * next;   // pointer to next free region
} freelist;

typedef struct buffer {
	char name[6];
	int size;
	char data[];

}buffer;
void create_frame(char * memory, char* funcname, int funcadd)
{
	if (no_of_frames == 5)
	{
		printf("Cannot create another frame, maximum number of frames have reached \n");
	}
	else if(stack_size==200)
	{
		printf("Stack overflow, not enough memory available for new function \n");
	}
	else
	{
		char * stack = memory + 300;
		framestatus* framelist = (framestatus *) stack;
		int new_no=0;
		int exit =0;
		if (no_of_frames >= 0)
		{
			for (int i=0; i<5; i++)
			{
				if(strcmp(framelist[i].name,funcname)==0)
				{
					exit=1;
					break;
				}
			}
			if(exit == 1)
			{
				printf("Function already exists \n");
			}
			else if(exit ==0)
			{
				for(int i=0 ; i<5 ; i++)
				{
					if(strcmp(framelist[i].name," ")==0)
					{
						new_no=i;
						break;
					}
				}
				framelist[new_no].functionaddress= funcadd;
				strcpy(framelist[new_no].name, funcname);
				framelist[new_no].used='u';
				if (new_no !=0)
				{
					framelist[new_no -1].used = 'n';
				}
				no_of_frames+=1;
				stack_size+=21;
			}
		}
	}
}

void delete_function(char * memory)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	if (no_of_frames == 0)
	{
		printf("Stack is empty \n");
	}
	else
	{
		int no=0;
		for(int i=0; i<5 ; i++)
		{
			if(framelist[i].used == 'u')
			{
				no =i;
				framelist[i]. functionaddress = 0;
				if(i!=0)
				{
				framelist[i-1].used = 'u';
				}
				strcpy(framelist[i].name," ");
				stack_size-=21;
				no_of_frames-=1;
				break;
				
			}
		}
		stack_size-=frames[no].size;
		frames[no].size=0;
		for(int i=0; i<15; i++)
		{
			frames[no].data[i]= ' ';
		}
		
	}

}

void create_integer(char * memory, char* name, int value)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	int  no = 0;
	int yes=0;
	for(int i=0; i<5; i++)
	{
		if(framelist[i].used == 'u')
		{
			no = i; 
			yes=1;
			break;
		}
	}
	if (yes == 0)
	{
		create_frame(memory, "main", rand() % 100);
	}
	if(frames[no].size == 15 || frames[no].size + 5 > 15)
	{
		printf("The frame is full, cannot create more data on it \n");
	}
	else
	{
		int val= frames[no].size;
		frames[no].data[val]= 'i';
		val+=1;
		memcpy(&frames[no].data[val], &value, sizeof(int));
		frames[no].size+=5;
		stack_size+=5;
	}
}

void create_double(char * memory, char* name, double value)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	int  no = 0;
	int yes=0;
	for(int i=0; i<5; i++)
	{
		if(framelist[i].used == 'u')
		{
			no = i; 
			yes=1;
			break;
		}
	}
	if (yes == 0)
	{
		create_frame(memory, "main", rand() % 100);
	}
	if(frames[no].size == 15 || frames[no].size + 8 > 15)
	{
		printf("The frame is full, cannot create more data on it \n");
	}
	else
	{
		int val= frames[no].size;
		frames[no].data[val]= 'd';
		val+=1;
		memcpy(&frames[no].data[val], &value, sizeof(double));
		frames[no].size+=9;
		stack_size+=9;
	}

}

void create_char(char * memory, char* name, char value)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	int  no = 0;
	int yes=0;
	for(int i=0; i<5; i++)
	{
		if(framelist[i].used == 'u')
		{
			no = i; 
			yes=1;
			break;
		}
	}
	if (yes == 0)
	{
		create_frame(memory, "main", rand() % 100);
	}
	
	if(frames[no].size == 15 || frames[no].size + 1 > 15)
	{
		printf("The frame is full, cannot create more data on it \n");
	}
	else
	{
		int val= frames[no].size;
		frames[no].data[val]= 'c';
		val+=1;
		frames[no].data[val]= value;
		frames[no].size+=2;
		stack_size+=2;
	}
}

int create_stack(char* memory, void* add)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	int  no = 0;
	int yes=0;
	int r_val=1;
	for(int i=0; i<5; i++)
	{
		if(framelist[i].used == 'u')
		{
			no = i; 
			yes=1;
			break;
		}
	}
	if (yes == 0)
	{
		create_frame(memory, "main", rand() % 100);
	}	
	if(frames[no].size == 15 || frames[no].size + 8 > 15)
	{
		// printf("The frame is full, cannot create more data on it \n");
		r_val=0;
	}
	else
	{
		int val= frames[no].size;
		frames[no].data[val]= 'p';
		val+=1;
		memcpy(&frames[no].data[val], &add, sizeof(void*));
		frames[no].size+=9;
		stack_size+=9;
	}
	return r_val;
}
void create_buffer(char * memory, char* name, int size)
{
	freelist * free = (freelist*) memory;
	char * heap = (char *) free + 12;
	buffer * buffers = (buffer*)(heap+heap_size) ;
	if(heap_size == 288)
	{
		printf("The heap is full cannot create more data on it \n");
	}
	else
	{
		int make=0;
		int offste=0;
		for(int i=0; i<no_of_buffers; i++)
		{
			buffer* buffer1= (buffer*)(heap+offste);
			if(strcmp(buffer1->name, name)==0)
			{
				make=1;
				printf("The buffer already exists. \n");
			}
			int v= buffer1->size + 4;
			offste+= sizeof(buffer) + v;
		}
		if(make==0)
		{
			void * ptr= (heap + heap_size);
			int create= create_stack(memory, ptr);
			if (create ==1)
			{
				int val = size + 4;
				strcpy(buffers->name, name);
				buffers->size = size;
				char letter ='a';
				for(int i=0; i<val; i++)
				{
					buffers->data[i] = letter;
					letter ++;
					if(letter > 'z')
					{
						letter ='a';
					}
				}
				heap_size+= (sizeof(buffer)  + val );
				no_of_buffers+=1;
			}
			else
			{
				printf("Buffer can't be created as there is no space on stack.To create this Buffer, make a new frame. \n");
			}
		}
	}
	
	
}
void delete_buffer(char * memory, char* name)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	freelist * free = (freelist*) memory;
	char * heap = (char *) free + 12;
	buffer * buffers;
	int exit=0;
	int yes = 0;
	int no = 1;
	for(int i=0; i<no_of_buffers; i++)
	{
		buffers = (buffer*)(heap+exit) ;
		if(strcmp(buffers->name,name)==0)
		{
			if (buffers->data[0]== '0')
			{
				no = 0;
				yes = 1;
			}
			if (no ==1)
			{
				int val = buffers->size +4;
				for(int j=0; j<val; j++)
				{
					buffers->data[j]='0';
				}
				yes = 1;
			}
			break;
		}
		int val = buffers->size + 4 ;
		exit+=(sizeof(buffer) + val );
	}
	if (yes == 0)
	{
		printf("The pointer is invalid \n");
	}
	if (no == 0)
	{
		printf("The buffer is already de-allocated \n");
	}
	if(yes == 1 && no == 1)
	{
		int br=0;
		for(int i=0; i<5; i++)
		{
			if (strcmp(framelist[i].name, " ")!=0)
			{
				int size= frames[i].size;
				int no=0;
				while(no!=size)
				{
					if(frames[i].data[no] == 'i')
					{
						no+=5;
					}
					else if(frames[i].data[no] == 'd')
					{
						no+=9;
					}
					else if(frames[i].data[no] == 'c')
					{
						no+=2;
					}
					else if(frames[i].data[no] == 'p')
					{
						void* ptr;
						memcpy(&ptr, &frames[i].data[no + sizeof(char)], sizeof(void*));
						void *ptr1= (heap+exit);
						if (ptr == ptr1)
						{
							char value=' ';
							memcpy(&frames[i].data[no], &value, sizeof(void*));
							br = 1;
							break;
	
						}
						no+=9;
					}
				}
				if(br == 1)
				{
					frames[i].size-=9;
					break;
				}
			}
		}
		d_buffer+=1;

	}
}

void show_memory(char * memory)
{
	char * stack = memory + 300;
	framestatus* framelist = (framestatus *) stack;
	frame* frames = (frame*) framelist + 105;
	freelist * free = (freelist*) memory;
	char * heap = (char *) free + 12;
	if ((stack_size - 20)== 0)
	{
		printf("Stack : It is currently empty \n");
		printf("\n");
	}
	else
	{
		printf("Stack: \n");
		printf("\n");
		printf("Frames created on stack are: \n");
		printf("\n");
		for(int i=0; i<5; i++)
		{
			if (strcmp(framelist[i].name, " ")!=0)
			{
				printf("Frame # %d : %s \n", framelist[i].number,framelist[i].name);
				int size= frames[i].size;
				int no=0;
				int array1[20];
				int arr1 = 0;
				double array2[15];
				int arr2 = 0;
				char array3[15];
				int arr3 = 0;
				int* array4[20];
				int arr_4 =0;
				while(no!=size)
				{
					if(frames[i].data[no] == 'i')
					{
						int value;
						memcpy(&value, &frames[i].data[no + sizeof(char)], sizeof(int));
						no+=5;
						array1[arr1] = value;
						arr1+=1;
					}
					else if(frames[i].data[no] == 'd')
					{
						double value;
						memcpy(&value, &frames[i].data[no + sizeof(char)], sizeof(double));
						no+=9;
						array2[arr2] = value;
						arr2+=1;
					}
					else if(frames[i].data[no] == 'c')
					{
						char value;
						memcpy(&value, &frames[i].data[no + sizeof(char)], sizeof(char));
						no+=2;
						array3[arr3] = value;
						arr3+=1;
					}
					else if(frames[i].data[no] == 'p')
					{
						int* ptr;
						memcpy(&ptr, &frames[i].data[no + sizeof(char)], sizeof(int*));
						no+=9;
						array4[arr_4]= ptr;
						arr_4+=1;
					}
				}
				if (arr1 == 0)
				{
					printf("Integer values stored in this frame are: None \n");
					printf("\n");
				}
				else
				{
					printf("Integer values stored in this frame are: \n");
					printf("\n");
					for(int k=0; k< arr1; k++)
					{
						printf("value %d: %d \n", k+1, array1[k]);
					}
					printf("\n");
				}
				if (arr2 == 0)
				{
					printf("Double values stored in this frame are: None \n");
					printf("\n");
				}
				else
				{
					printf("Double values stored in this frame are: \n");
					printf("\n");
					for(int k=0; k< arr2; k++)
					{
						printf("value %d: %f \n", k+1, array2[k]);
					}
					printf("\n");
				}
				if (arr3 == 0)
				{
					printf("Character values stored in this frame are: None \n");
					printf("\n");
				}
				else
				{
					printf("Character values stored in this frame are: \n");
					printf("\n");
					for(int k=0; k< arr3; k++)
					{
						printf("value %d: %c \n", k+1, array3[k]);
					}
					printf("\n");
				}
				if (arr_4 == 0)
				{
					printf("Pointers stored in this frame are: None \n");
					printf("\n");
				}
				else
				{
					printf("Pointers stored in this frame are: \n");
					printf("\n");
					for(int k=0; k< arr_4; k++)
					{
						printf("value %d: %p \n", k+1, array4[k]);
					}
					printf("\n");
				}
			}
		}
		

	}
	if (d_buffer == no_of_buffers)
	{
		printf("Heap : It is currently empty \n");	
		printf("\n");
	}
	else
	{
		printf("Heap: \n");
		printf("\n");
		printf("Buffers created on heap are: \n");
		printf("\n");
		int exit=0;
		for (int i = 0; i < no_of_buffers; i++)
		{
    		buffer * buffers= (buffer*)(heap+exit);
			int val = buffers->size + 4;
			if(buffers->data[0]!='0')
			{
				printf("Buffer # %d : %s \n", (i + 1), buffers->name);
				for(int j=0; j<val; j++)
				{
					printf("%c",buffers->data[j]);
				}
				printf("\n");
				printf("\n");
			}
			exit+= (sizeof(buffer)+ val );
		}
	}

}
int main (int argc, char* argv[]) {
	// Buffer that will emulate stack and heap memory
    char * memory = (char *) malloc (500); // Buffer that will emulate stack and heap memory
	// creating stack
	char * stack = memory + 300;
	//frame list
	framestatus* framelist = (framestatus *) stack;
	for(int i=0; i<5 ; i++)
	{
		framelist[i].frameaddress = rand() % 100;
		framelist[i]. functionaddress = 0;
		framelist[i].number = i+1;
		framelist[i].used = 'n';
		strcpy(framelist[i].name," ");
	}

	// allocatinge frames to store variables in it (each frame is of 19 bytes)
	frame* frames = (frame*) framelist + 105;
	for (int i = 0; i < 5; i++) 
	{
		frames[i].size = 0;
		stack_size+=4;
		for(int j=0; j<15 ; j++)
		{
			frames[i].data[j]= ' ';
		}
}

	// system's free list
	freelist * free1 = (freelist*) memory;

	//creating heap
	char * heap = (char *) free1 + 12;

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
		else if(strcmp(args[0],"CF")==0)
		{
			char name[20];
			int size=0;
			strcpy(name,args[1]);
			size=atoi(args[2]);
			create_frame(memory,name,size);
		}
		else if(strcmp(args[0],"DF")==0)
		{
			delete_function(memory);
		}
		else if(strcmp(args[0],"CI")==0)
		{
			char name[20];
			int size=0;
			strcpy(name,args[1]);
			size=atoi(args[2]);
			create_integer(memory,name,size);
		}
		else if(strcmp(args[0],"CD")==0)
		{
			char name[20];
			double size=0;
			strcpy(name,args[1]);
			size=atof(args[2]);
			create_double(memory,name,size);
		}
		else if(strcmp(args[0],"CC")==0)
		{
			char name1[20];
			char name2;
			strcpy(name1,args[1]);
			name2 = args[2][0];
			create_char(memory,name1,name2);
		}
		else if(strcmp(args[0],"CH")==0)
		{
			char name[20];
			int size=0;
			strcpy(name,args[1]);
			size=atoi(args[2]);
			create_buffer(memory,name,size);
		}
		else if(strcmp(args[0],"DH")==0)
		{
			char name[20];
			strcpy(name,args[1]);
			delete_buffer(memory,name);
		}
		else if(strcmp(args[0],"SM")==0)
		{
			show_memory(memory);
		}
    }
	free(memory);
  return 0;
}
