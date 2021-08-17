#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
//Variable Declaration
#define MxN 502	
#define MyN 502	
#define NC 999	
using namespace std;
double f[MxN][MyN],w[MxN][MyN],shadow[MxN][MyN],S[MxN][MyN],N[MxN][MyN];
double Pn[MxN][MyN],Ps[MxN][MyN],Pw[MxN][MyN],Pe[MxN][MyN],P[MxN][MyN];
double kp=0.1,ks=1.5,kw=0.3,maxS;
int MaxTime=300;
int Mx,My,x,y;
int r=5;
double gam=0.9;
int Sat=1;
int bek=4;
int MR=2;
int WD=1,ED=2,ND=3,SD=4;
struct dc {
	int x;
	int y;
};
struct coord {
	int fx;
	int fy;
	int dx;
	int dy;
} movement[NC+1];
int count=0;
// Border Declaration
int InitBorder()
{
	int i,j;
	for(i=0;i<=Mx+1;i++)
	for(j=0;j<=My+1;j++)
	{
		w[i][j]=0;
	}
	for(i=0;i<=Mx+1;i++)
	{
		w[i][0]=1;
		w[i][Mx+1]=1;
		
	}
	for(j=0;j<=My+1;j++)
	{
		w[0][j]=1;
		w[Mx+1][j]=1;
	}
	for(i=61;i<=63;i++)
	{
		w[1][i]=0;
		w[0][i]=0;
		w[Mx][i]=0;
		w[Mx+1][i]=0;

	}
	return 0;
}
// Every step initilization
int InitStep()
{
	int i,j;
 	 for(i=1;i<=Mx;i++)
	 for(j=1;j<=My;j++)
	 {
		Pw[i][j]=0.0;
		Pn[i][j]=0.0;
		Pe[i][j]=0.0;
		Ps[i][j]=0.0;
		if ((f[i][j]==1) || (w[i][j]==1))
		shadow[i][j]=1.0;
		else
		shadow[i][j]=0.0;
	 }
	 for(i=0;i<=NC;i++)
	 {
	 movement[i].fx=0;
	 movement[i].fy=0;
	 movement[i].dx=0;
	 movement[i].dy=0;
	 }
	for(i=61;i<=63;i++)
	{
		f[1][i]=0;
		f[Mx][i]=0;
	}
	return 0;
}
//N Definition
int locateN()
{
	int side=0;
	for(int i=0;i<Mx;i++)
		for(int j=0;j<My;j++)
		{
			side=0;
			if (w[i][j]==0)
		{
			if ((w[i+1][j]==0) && (i+1<Mx))
				side++;
			if ((w[i-1][j]==0) && (i-1>0))
				side++;
			if ((w[i][j+1]==0) && (j+1<My))
				side++;
			if ((w[i][j-1]==0) && (j-1>0))
				side++;
			N[i][j]=side;

		}
		}
		return 0;
}
//S Definition
int locateS()
{
	int step;
	int NS,NS1;
	int flag,i;
	dc stack[NC];
	dc stack1[NC];
	step=1;
	NS=0;
	for(i=61;i<=63;i++)
	{
	 NS++;
	 stack[NS].x=1;
	 stack[NS].y=i;
	 S[stack[NS].x][stack[NS].y]=step;
	}

	for(i=61;i<=63;i++)
	{
	 NS++;
	 stack[NS].x=Mx;
	 stack[NS].y=i;
	 S[stack[NS].x][stack[NS].y]=step;
	}

	flag=0;
	while (!flag)
	{
	 NS1=0;
	 step++;
	 for (i=1;i<=NS;i++)
	{
	if ((stack[i].x+1<=Mx) &&
		((S[stack[i].x+1][stack[i].y]==0)||
		(S[stack[i].x+1][stack[i].y]>step))&&
		(w[stack[i].x+1][stack[i].y]<=0))
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x+1;
         stack1[NS1].y=stack[i].y;
		 S[stack[i].x+1][stack[i].y]=step;
		}
	if ((stack[i].x-1>=0) &&
		((S[stack[i].x-1][stack[i].y]==0)||
		(S[stack[i].x-1][stack[i].y]>step))&&
		(w[stack[i].x-1][stack[i].y]<=0))
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x-1;
         stack1[NS1].y=stack[i].y;
		 S[stack[i].x-1][stack[i].y]=step;
		}
	if ((stack[i].y+1<=My) &&
		((S[stack[i].x][stack[i].y+1]==0)||
		(S[stack[i].x][stack[i].y+1]>step))&&
		(w[stack[i].x][stack[i].y+1]<=0))
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x;
         stack1[NS1].y=stack[i].y+1;
		 S[stack[i].x][stack[i].y+1]=step;
		}
	if ((stack[i].y-1>=0) &&
		((S[stack[i].x][stack[i].y-1]==0)||
		(S[stack[i].x][stack[i].y-1]>step))&&
		(w[stack[i].x][stack[i].y-1]<=0))
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x;
         stack1[NS1].y=stack[i].y-1;
		 S[stack[i].x][stack[i].y-1]=step;
		}
	}
	if (NS1==0) 
	   flag=1;
	for (i=1;i<=NS1;i++)
		stack[i]=stack1[i];
	NS=NS1;
	NS1=0;
	}

return 0; 		
}
//Distance definition
double incr(int step)
{
	return 1.0/(step*32);
}
double DWinc(int step)
{
	return 1.0/(step);
}
int Wave(int i,int j,int MaxR)
{
	int step;
	int NS,NS1;
	int flag;
	dc stack[NC];
	dc stack1[NC];
	step=0;
	NS=1;
	stack[NS].x=i;
	stack[NS].y=j;
	P[stack[NS].x][stack[NS].y]=1;
	flag=0;
	while ((!flag) && (step<MaxR))
	{
	 NS1=0;
	 step++;
	 for (i=1;i<=NS;i++)
	 {
	 if (stack[i].x+1<=Mx)
		{
		if (stack[i].y+1<=My) 
			{
			 NS1++;
			 stack1[NS1].x=stack[i].x+1;
		  	 stack1[NS1].y=stack[i].y+1;
			 P[stack[i].x+1][stack[i].y+1]+=incr(step);
			}
		if (stack[i].y-1>=0) 
			{
			 NS1++;
			 stack1[NS1].x=stack[i].x+1;
			 stack1[NS1].y=stack[i].y-1;
			 P[stack[i].x+1][stack[i].y-1]+=incr(step);
			}

		 NS1++;
		 stack1[NS1].x=stack[i].x+1;
         stack1[NS1].y=stack[i].y;
		 P[stack[i].x+1][stack[i].y]+=incr(step);
		}
	 if (stack[i].x-1>=0)
		{
 		 if (stack[i].y+1<=My) 
			{
			 NS1++;
			 stack1[NS1].x=stack[i].x-1;
		  	 stack1[NS1].y=stack[i].y+1;
			 P[stack[i].x-1][stack[i].y+1]+=incr(step);
			}
		 if (stack[i].y-1>=0) 
			{
			 NS1++;
			 stack1[NS1].x=stack[i].x-1;
			 stack1[NS1].y=stack[i].y-1;
			 P[stack[i].x-1][stack[i].y-1]+=incr(step);
			}
		 NS1++;
		 stack1[NS1].x=stack[i].x-1;
         stack1[NS1].y=stack[i].y;
		 P[stack[i].x-1][stack[i].y]+=incr(step);
		}
	 if (stack[i].y+1<=My) 
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x;
         stack1[NS1].y=stack[i].y+1;
		 P[stack[i].x][stack[i].y+1]+=incr(step);
		}
	 if (stack[i].y-1>=0) 
		{
		 NS1++;
		 stack1[NS1].x=stack[i].x;
         stack1[NS1].y=stack[i].y-1;
		 P[stack[i].x][stack[i].y-1]+=incr(step);
		}
	}
	if (NS1==0) 
	   flag=1;
	for (i=1;i<=NS1;i++)
		stack[i]=stack1[i];
	NS=NS1;
	NS1=0;
	}
return 0; 		

}
//Presure find
int InitPres()
{
	int i,j;
    for(i=0;i<=Mx;i++)
	for(j=0;j<=My;j++)
	P[i][j]=0;

	for(i=1;i<Mx;i++)
	 for(j=1;j<My;j++)
	 {
	   if (f[i][j]==1)
	   Wave(i,j,MR);

	 }
	 return 0;
}
//Radius toward obstacle
double ro(int i,int j,int dir,int k)
	{
		double temp=-1;
		int l=j;
		if (dir==ND) while ((l<j+k) && (temp<0))
		{
			l++;
			if (((f[i][l]>0) || (w[i][l]>0)) && l<My)
				temp=l-j-1;
		}
		else
		if (dir==SD) while ((l>j-k) && (temp<0))
		{
			l--;
			if (((f[i][l]>0) || (w[i][l]>0)) && l>0)
				temp=j-l-1;
		}
		else
		{
			l=i;
			if (dir==ED) while ((l<i+k) && (temp<0))
			{
				l++;
				if (((f[l][j]>0) || (w[l][j]>0)) && l<Mx)
					temp=l-i-1;
			}
			else
			if (dir==WD) while ((l>i-k) && (temp<0))
			{
				l--;
				if (((f[l][j]>0) || (w[l][j]>0)) && l>0)
					temp=i-l-1;
			}
		}
		if (temp<0)	temp=r;
		return temp;
}
// Function D, Distance toward obstacle
double D(int i,int j,int dir,double radius)
{	
	double P=1;
	double Sum=0.0;
	double rl=ro(i,j,dir,radius);
	double m;
	for(m=1;m<=rl;m++)
	{
		Sum+=m/radius;
	}
	return Sum;
}
double deltaP(int i1,int j1,int i2,int j2)
{
	return (P[i1][j1]-P[i2][j2]);
}
double I(double fi)
{
		if (fi<0.0) 
			return 0.0;
		else 
			return 1.0;
}

double delta(int i1,int j1,int i2,int j2)
{
	double q;
	if (S[i1][j1]>0){
	q=S[i2][j2]-S[i1][j1];
	}
	else
	{
	q=0;
	}
	return q;
}
int main()
{
	int i,j,l,tx,ty;
	int timestep,temp;
	char prefix[4]="out";
	char ext[5]=".dat";
	char name[20]="";
	char tmp[20]="";
	FILE *file;  

 		ifstream fin("Input5.txt",ios::in);
        fin>>Mx;
        fin>>My;
        InitBorder();	
		for(j=My;j>0;j--)
         for(i=1;i<=Mx;i++)
		 {
	      fin>>temp;
	       if (temp==1)
		   {
		    f[i][j]=1;
//			shadow[i][j]==1;
		   }
	      else if (temp==2)
		   w[i][j]=1;
   }
   fin.close();
     locateS();
/*	 for(i=1;i<Mx;i++)
		 for(j=1;j<My;j++)
			 S[i][j]=S[i][j]/10.0;
			 */
	 locateN();


	for (timestep=1; timestep<=MaxTime;timestep++)
{
	 InitStep();
	 InitPres();
	 count=0;
//	 double e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12;
	 for(i=1;i<=Mx;i++)
	 for(j=1;j<=My;j++)
		 if (f[i][j]==1)
	 {
	 f[i][j]=5;
//	 e1=ks*delta(i,j+1,i,j);
//	 e2=kp*D(i,j+1);
//	 e3=delta(i+1,j,i,j);
//	 e4=delta(i,j+1,i,j);
//	 e5=delta(i-1,j,i,j);
//	 e6=delta(i,j-1,i,j);
//	 e7=kw*Dwall(i,j+1);
//	 e8=delta(i+1,j,i,j);
//	 e9=D(i,j);
//	 e10=f[1][1];
	 if (w[i][j+1]==0)
	 {
		Pn[i][j]=(1.0/N[i][j])*exp(ks*I(delta(i,j+1,i,j))-kp*deltaP(i,j+1,i,j)+kw*D(i,j,ND,r))*(1-w[i][j+1]);
		
	 }
	 else
	 {
		Pn[i][j]=0;
	 }
	 if (w[i-1][j]==0)
	 {
		Pw[i][j]=(1.0/N[i][j])*exp(ks*I(delta(i-1,j,i,j))-kp*deltaP(i-1,j,i,j)+kw*D(i,j,WD,r))*(1-w[i-1][j]);
	 }
	 else
	 {
		 Pw[i][j]=0;
	 }
	 if (w[i+1][j]==0)
	 {
		Pe[i][j]=(1.0/N[i][j])*exp(ks*I(delta(i+1,j,i,j))-kp*deltaP(i+1,j,i,j)+kw*D(i,j,ED,r))*(1-w[i+1][j]);
	 }
	 else
	 {
		 Pe[i][j]=0;
	 }
	 if (w[i][j-1]==0)
	 {
		Ps[i][j]=(1.0/N[i][j])*exp(ks*I(delta(i,j-1,i,j))-kp*deltaP(i,j-1,i,j)+kw*D(i,j,SD,r))*(1-w[i][j-1]);
	 }
	 else
	 {
		Ps[i][j]=0;
	 }
	 f[i][j]=1;
 }
//	if ((timestep%5==0) || (timestep<=40))
//	if ((timestep%20==0))
//	 {
	 strcpy(name,"out");
 	 if (timestep<10) 
	 strcpy(tmp,"00");
	 else
	 if (timestep<100) 
	 strcpy(tmp,"0");
	 else
	 strcpy(tmp,"");
	 strcat(name,tmp);
	 strcpy(tmp,"");
	 itoa(timestep,tmp,10);
	 strcat(name,tmp);
	 strcat(name,ext);
	 cout<<"Time -  "<<timestep<<endl;

	 file = fopen (name, "wt");

/*	 stringstream ss(stringstream::in | stringstream::out);
	 ss<<"Out"<<timestep<<".dat";
	 ss>>name;*/
 //	 ofstream fout(name, ios::out);
	 fprintf(file,"TITLE=\"Celluar Automat\"\n");
	 fprintf(file,"VARIABLES=\"X\", \"Y\", \"D\"\n");
	 fprintf(file,"ZONE I=%d, J=%d, F=POINT\n",Mx,My);

	for(j=1;j<=My;j++)
		for(i=1;i<=Mx;i++)
		if (f[i][j]>0)
		{
		 fprintf(file,"%d\t%d\t 1\n", i, j);
//		 fout<<i<<" "<<j<<" "<<Pn[i][j]<<" "<<Pe[i][j]<<" "<<Pw[i][j]<<" "<<Ps[i][j]<<endl;
		}
		else if (w[i][j]>0)
		{
		 fprintf(file,"%d\t%d\t 2\n", i, j);
		}
		else
		{
		 fprintf(file,"%d\t%d\t 0\n", i, j);
		}

		fclose(file);

	 strcpy(name,"Pv");
 	 if (timestep<10) 
	 strcpy(tmp,"00");
	 else
	 if (timestep<100) 
	 strcpy(tmp,"0");
	 else
	 strcpy(tmp,"");
	 strcat(name,tmp);
	 strcpy(tmp,"");
	 itoa(timestep,tmp,10);
	 strcat(name,tmp);
	 strcat(name,ext);
	 file = fopen (name, "wt");

	 fprintf(file,"TITLE=\"Celluar Automat\"\n");
	 fprintf(file,"VARIABLES=\"X\", \"Y\", \"D\"\n");
	 fprintf(file,"ZONE I=%d, J=%d, F=POINT\n",Mx,My);

 	 for(j=1;j<=My;j++)
 		for(i=1;i<=Mx;i++)
		if (w[i][j]>0)
		fprintf(file,"%d\t%d\t10\n", i, j);
		else
		fprintf(file,"%d\t%d\t%f\n", i, j, P[i][j]);

		fclose(file);
/*
	 strcpy(name,"Sh");
 	 if (timestep<10) 
	 strcpy(tmp,"00");
	 else
	 if (timestep<100) 
	 strcpy(tmp,"0");
	 else
	 strcpy(tmp,"");
	 strcat(name,tmp);
	 strcpy(tmp,"");
	 itoa(timestep,tmp,10);
	 strcat(name,tmp);
	 strcat(name,ext);
	 file = fopen (name, "wt");

	 fprintf(file,"TITLE=\"Celluar Automat\"\n");
	 fprintf(file,"VARIABLES=\"X\", \"Y\", \"Pn\", \"Ps\", \"Pw\", \"Pe\"\n");
	 fprintf(file,"ZONE I=%d, J=%d, F=POINT\n",Mx,My);

	for(j=1;j<=My;j++)
		for(i=1;i<=Mx;i++)
		 fprintf(file,"%d\t%d\t%f\t%f\t%f\t%f\n", i, j, Pn[i][j],Ps[i][j],Pw[i][j],Pe[i][j]);

		fclose(file);
*/
//	}


	 for(i=2;i<Mx;i++)
	 for(j=2;j<My;j++)
		 if (f[i][j]==1)
		 {
			 //TORMOZ!!!!!
			if ((Pn[i][j]>=Pw[i][j]) && (Pn[i][j]>=Pe[i][j]) && (Pn[i][j]>=Ps[i][j]) && (shadow[i][j+1]==0))
			{
				count++;
			movement[count].fx=i;
			movement[count].fy=j;
			movement[count].dx=i;
			movement[count].dy=j+1;
			shadow[i][j+1]=1;
			}
			else
			if ((Pe[i][j]>=Pw[i][j]) && (Pe[i][j]>=Pn[i][j]) && (Pe[i][j]>=Ps[i][j]) && (shadow[i+1][j]==0))
			{
				count++;
			movement[count].fx=i;
			movement[count].fy=j;
			movement[count].dx=i+1;
			movement[count].dy=j;
			shadow[i+1][j]=1;
			}
			else
			if ((Pw[i][j]>=Pe[i][j]) && (Pw[i][j]>=Pn[i][j]) && (Pw[i][j]>=Ps[i][j]) && (shadow[i-1][j]==0))
			{
				count++;
			movement[count].fx=i;
			movement[count].fy=j;
			movement[count].dx=i-1;
			movement[count].dy=j;
			shadow[i-1][j]=1;
			}
			else
			if ((Ps[i][j]>=Pw[i][j]) && (Ps[i][j]>=Pn[i][j]) && (Ps[i][j]>=Pe[i][j]) && (shadow[i][j-1]==0))
			{
				count++;
			movement[count].fx=i;
			movement[count].fy=j;
			movement[count].dx=i;
			movement[count].dy=j-1;
			shadow[i][j-1]=1;
			}
			
		 }
		 for (l=1;l<=count;l++)
		 {
			ty=movement[l].fy;
			tx=movement[l].fx;
//			shadow[tx][ty]=0;
	      	f[tx][ty]=0;
			ty=movement[l].dy;
			tx=movement[l].dx;
			if (ty<=My)
			f[tx][ty]=1;
		 }
}

	return 0;
}

