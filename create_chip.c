#include<stdio.h>
#include<stdlib.h>
int main()
{
	char s[30];
	FILE *fp;
	int **a;
	int i,j,m,n,k;
	printf("Enter the output file name with .txt:");
	scanf("%s",s);
	printf("Enter the biochip size(M*N)");
	scanf("%d%d",&m,&n);

	fp=fopen(s,"w");
	fprintf(fp,"%d %d\n",m,n);
	a=(int**)malloc(m*n*sizeof(int*));
	for(i=0;i<m*n;i++)
		a[i]=(int*)malloc(8*sizeof(int));
	for(i=0,k=0;i<m;i++)
	{	
		for(j=0;j<n;j++,k++)
		{
			a[k][0]=i+1;
			a[k][1]=j+1;
			a[k][2]=k+1;
			a[k][3]=0;
			a[k][4]=(j==0)?0:k;
		//	printf("\nk=%d",k);
			a[k][5]=(k+1+n>m*n)?0:(k+1+n);
			a[k][6]=(j==n-1)?0:k+2;
			a[k][7]=(k>n-1)?(k+1-n):0;
			fprintf(fp,"%d %d %d %d %d %d %d %d",a[k][0],a[k][1],a[k][2],a[k][3],a[k][4],a[k][5],a[k][6],a[k][7]);
			fprintf(fp,"\n");
		//	printf("\n%d %d %d",i+1,j+1,k+1);
		}	
		fprintf(fp,"\n");	
	}
	for(i=0;i<m*n;i++)
	{	
		printf("\n");
		for(j=0;j<8;j++)
		{
			printf("%d ",a[i][j]);
		}
	}
/*	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			fprintf(fp,"%d ",a[i][j]);
			if(j==n)
				fprintf(fp,"\n");
		}
	//	fprintf(fp,"\n");
	}*/
	fclose(fp);
	return 0;			
}