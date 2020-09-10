#include<stdio.h>
#include<stdlib.h>
struct nd
{
	int x;
	int y;
	int id;
	int flag;
	struct nd *left;
	struct nd *down;
	struct nd *right;
	struct nd *up;	
};
typedef struct nd node;
node **nodelist;
int *faultlist,*suspectedfaultlist;
int test_time=0,diagnosis_time=0;
int activate(int m, int n, int i);

void displayVisitedNodeBiochip(int m,int n)
{
	printf("\nBiochip info:\n");
	int i,j,k=0;
	for(i=0;i<m;i++)
	{
		printf("\n");
		for(j=0;j<n;j++,k++)
			printf("%5d",nodelist[k]->flag);
	}
}
void displayfaultylist(int *a,int n)
{
	int i;
	for( i=1;i<=n;i++)
		printf("\t%d ",a[i]);
}  
int left(int j)
{
	j=j-1;
	if(nodelist[j]->left)
		return nodelist[j]->left->id;
	else
		return 0;
}
int right(int j)
{
	j=j-1;
	if(nodelist[j]->right)
		return nodelist[j]->right->id;
	else
		return 0;
}
int up(int j)
{
	j=j-1;
	if(nodelist[j]->up)
		return nodelist[j]->up->id;
	else
		return 0;
}
int down(int j)
{
	j=j-1;
	if(nodelist[j]->down)
		return nodelist[j]->down->id;
	else
		return 0;
}

//resets an array
int reset(int* a, int k)
{
	int i;
	for(i=0;i<k;i++)
	{
		if(nodelist[a[i]-1]->flag!=2)
		nodelist[a[i]-1]->flag=0;
		a[i]=0;
	}
}

//PRDTest to test the upper boundary of the chip
int PRDTest(int m,int n)
{
    //array to keep the record of activated electrodes
    int *a=(int*)malloc((m+n)*sizeof(int));
	int k=0,flag=0;
	nodelist[0]->flag=1;
	a[k++]=1;
    int i,id=1;
	for(i=1;i<n;i++)
	{
		int t=right(id);
		if(nodelist[t-1]->flag==2){
			flag=1;
			nodelist[id-1]->flag=3;
			k--;//k-- so that it doesnot resets the value 3 added 
		}
		else if(nodelist[t-1]->flag==0){
			a[k++]=t;
			nodelist[t-1]->flag=1;
		}
		id=t;
	 		
	}
	for(i=1;i<m;i++)
	{
		int t=down(id);
		if(nodelist[t-1]->flag==2){
			flag=1;
			nodelist[id-1]->flag=3;
			k--;
		}
		else if(nodelist[t-1]->flag==0){
			a[k++]=t;
			nodelist[t-1]->flag=1;
		}
		id=t;
		
	}
	if(flag==1){
		reset(a,k);
		free(a);
		return -1;
	}
	free(a);
	return 1;
}

//PDRTest to check the lower boundary of the biochip
int PDRTest(int m,int n)
{
    //array to keep the record of activated electrodes
    int *a=(int*)malloc((m+n)*sizeof(int));
	int k=0,flag=0;
	nodelist[0]->flag=1;
	a[k++]=1;
    int i,id=1;
    for(i=1;i<m;i++)
	{
		int t=down(id);
		if(nodelist[t-1]->flag==2){
			flag=1;
			nodelist[id-1]->flag=3;
			k--;
		}
		else if(nodelist[t-1]->flag==0){
			a[k++]=t;
			nodelist[t-1]->flag=1;
		}
		id=t;
		
	}
	for(i=1;i<n;i++)
	{
		int t=right(id);
		if(nodelist[t-1]->flag==2){
			flag=1;
			nodelist[id-1]->flag=3;
			k--;//k-- so that it doesnot resets the value 3 added 
		}
		else if(nodelist[t-1]->flag==0){
			a[k++]=t;
			nodelist[t-1]->flag=1;
		}
		id=t;
	 		
	}
	if(flag==1){
		reset(a,k);
		free(a);
		return -1;
	}
	free(a);
	return 1;
}

int* DRDTest(int m,int n,int i)
{
	int *d=(int*)malloc(n*sizeof(int));
	d[0]=0;
	int j;
    if(i==2)
    {
        if(m%2==0)
            j=m-2;
        else
            j=m-1;
    }
    else
    {
        if(m%2==0)
            j=m-1;
        else
            j=m-2;
    }
	for(;j>=2;j-=2){
		int *a=(int*)malloc((m+n)*sizeof(int));
		int k=0,fg=0;
    	int i,id=1;
    	for(i=1;i<j;i++)
    		id=down(id);

		for(i=1;i<n;i++)
		{
			int t=right(id);		
			if(nodelist[t-1]->flag==2){
				fg=1;
				nodelist[id-1]->flag=3;
				if(k>0)
                    k--;
			}
			else if(nodelist[t-1]->flag==0){
				a[k++]=t;
				nodelist[t-1]->flag=1;
			}
			id=t; 		
		}
		for(i=1;i<=m-j;i++)
			id=down(id);		
	
		if(fg==1){
			reset(a,k);
            // int b = activate(m,n,j);
			d[++d[0]]= j;
		}
		free(a);
	}
	return d;
}
int DRDactivate(int m, int n, int i)
{
	int j,k=0,rtn;
	int *id_list = (int*)malloc((n+i-2)*sizeof(int));
    id_list[k++] = n+i-3;
    for(j=1;j<i;j++)
        id_list[k++]= (j-1)*n + 1;

    for(j=1;j<n-1;j++)
        id_list[k++] = (i-1)*n + j;

	for(k=0; k<n-1; k++) {
		for(j=k+i-1; j>0; j--) {
			int id_r = id_list[j];
			int id_a = id_list[j+1];

			if(nodelist[id_a-1]->flag == 3) {
				nodelist[id_a-1]->flag = 1;
				nodelist[id_r-1]->flag = 3;
			}
		}

		if(nodelist[0]->flag == 3) {
			rtn = id_list[k+i+1];
			nodelist[0]->flag = 1;
			break;
		}
	}
	
    diagnosis_time+=(i-1)+k*3;
	if(i==2 && k>0)
		diagnosis_time-=1;
   // printf("%d --",(i-1)+k*3);
    free(id_list);
	return rtn;
}


int *RDRTest(int m, int n, int i)
{
	int *d=(int*)malloc(n*sizeof(int));
	d[0]=0;
	int j;
    if(i==2) {
        if(n%2 == 0)
            j=n-2;
        else
            j=n-1;
    }
    else {
        if(n%2 == 0)
            j=n-1;
        else
            j=n-2;
    }
	for(;j>=2;j-=2){
		int *a=(int*)malloc((m+n)*sizeof(int));
		int k=0,flag=0;
    	int i,id=1;
    	for(i=1;i<j;i++)
    		id=right(id);

		for(i=1;i<m;i++)
		{
			int t=down(id);		
			if(nodelist[t-1]->flag==2){
				flag=1;
				nodelist[id-1]->flag=3;
                if(k>0)
				    k--;
			}
			else if(nodelist[t-1]->flag==0){
				a[k++]=t;
				nodelist[t-1]->flag=1;
			}
			id=t; 		
		}
		for(i=1;i<=n-j;i++)
			id=right(id);
		
		if(flag==1){
			reset(a,k);
			d[++d[0]]= j;
			}
		free(a);
	}

	return d;
}


int RDRactivate(int m, int n, int i)
{
	int j,k=0,rtn;
	int *id_list = (int*)malloc((m+i-2)*sizeof(int));
    id_list[k++] = m+i-3;
    for(j=1;j<=i;j++)
        id_list[k++]= j;

    for(j=1;j<m-2;j++)
        id_list[k++] = j*n + i;


	for(k=0; k<m-1; k++) {
		for(j=k+i-1; j>0; j--) {
			int id_r = id_list[j];
			int id_a = id_list[j+1];

			if(nodelist[id_a-1]->flag == 3) {
				nodelist[id_a-1]->flag = 1;
				nodelist[id_r-1]->flag = 3;
			}
		}

		if(nodelist[0]->flag == 3) {
			rtn = id_list[k+i+1];
			nodelist[0]->flag = 1;
			break;
		}
	}
   // diagnosis_time+=(i-1)+(k-1)*3;
   // printf("%d --",diagnosis_time);
    free(id_list);
	return rtn;
}

int search(int *faultlist, int f)
{
	for(int i=1; i<=faultlist[0]; i++) {
		if(faultlist[i] == f) return 1;
	}
	return 0;
}


int main()
{
    FILE *fp;
	char s[30];
	int row[8],m,n;
	int i,j,k;
    printf("Enter your input file name with .txt:");
    scanf("%s",s);
	fp=fopen(s,"r");
	fscanf(fp,"%d ",&m);
	fscanf(fp,"%d ",&n);
	printf("Biochip size is %d*%d",m,n);
	printf("\nTotal number of nodes:%d\n",m*n);
    nodelist=(node**)malloc(m*n*sizeof(node*));
	for(i=0;i<m*n;i++)
		nodelist[i]=(node*)malloc(sizeof(node));
	for(i=0;i<m*n;i++)
	{
		for(j=0;j<8;j++)    //Input file's each row contains 8 values  
		{
			fscanf(fp,"%d ",&row[j]);
		}
		nodelist[i]->x=row[0];
		nodelist[i]->y=row[1];
		nodelist[i]->id=row[2];
		nodelist[i]->flag=row[3];	
		nodelist[i]->left=(row[4]==0)?NULL:nodelist[row[4]-1];
		nodelist[i]->down=((row[5]==0)?NULL:nodelist[row[5]-1]);
		nodelist[i]->right=((row[6]==0)?NULL:nodelist[row[6]-1]);
		nodelist[i]->up=((row[7]==0)?NULL:nodelist[row[7]-1]);
	}
	faultlist=(int *)malloc((m*n)*sizeof(int));
    faultlist[0] = 0;
    int p1=PRDTest(m,n);
    int p2=PDRTest(m,n);
	if(p1==-1 ||p2 ==-1)
        printf("Biochip Testing not proceeded further due to peripheral faults : ");
    else{
        int *a,*b,*c,*d;
        a = DRDTest(m,n,1);
	    for(i=a[0]; i>=1; i--) {
	    	int f = DRDactivate(m,n,a[i]);
	    	faultlist[++faultlist[0]] = f;
	    }
        b = DRDTest(m,n,2);
        for(i=b[0]; i>=1; i--) {
	    	int f = DRDactivate(m,n,b[i]);
	    	faultlist[++faultlist[0]] = f;
	    }

        // Right Down Right
	    c = RDRTest(m,n,2);
	    for(i=c[0]; i>=1; i--) {
	    	int f = RDRactivate(m,n,c[i]);
	    	if(!search(faultlist, f))
	    		faultlist[++faultlist[0]] = f;
	    }
	    d = RDRTest(m,n,3);
	    for(i=d[0]; i>=1; i--) {
	    	int f = RDRactivate(m,n,d[i]);
	    	if(!search(faultlist, f))
	    		faultlist[++faultlist[0]] = f;	
	    }
        
        free(a);
        free(b);
        free(c);
        free(d);

    }
	displayVisitedNodeBiochip(m,n);
    printf("\nThe faulty nodes are : ");
	for(i=1;i<=faultlist[0];i++)
    	printf("%d  ",faultlist[i]);
	printf("\n");
	//total_of_five
    test_time= 8*m + 8*n -9;
	printf("\nThe test time for open faults only is : %d",(6*m+3*n-3));
    printf("\nThe test time required is : %d\n",test_time);
    printf("The diagnosis time required is : %d\n",diagnosis_time);
    free(faultlist);
    return 0;

}