
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

double r2(){
	return (double)rand() / (double)RAND_MAX ;
}


int main(int argc, char** argv){
		
	int k,l,m=8,state,numstates,flag=0,j,crit=0,flag1=0,p=0.5;

	FILE *f = fopen("trace.txt","w"),*fout = fopen(argv[1],"w"),*fin = fopen("input.txt","r"),*fout1=fopen("result.txt","a+");	
	fscanf(fin,"%d%d",&k,&l);
	//scanf("%d",&p);	
	numstates = (k + 9);		
	
	unsigned long long *arrivals = (unsigned long long *) malloc(numstates*sizeof(unsigned long long));
	double *prob = (double*) malloc(numstates*sizeof(double)), coin,barr1,barr2,average=0,oldaverage=0,totalav=0,totalav2=0,g,g1,g2,go;
	unsigned long long tot_arrivals=0,i,time1=0,time2=0;
	
	srand(time(NULL));
	state = 0;

	for(i=0;i<numstates;i++){
		arrivals[i]=0;
	}
	
	i=0;
	

	while(flag1!=1){
					
		if (state>=1 && state<=k) {
			barr1 = (double)(l)/(double)(l+m);
			coin = r2();
			if (coin<=barr1) flag = 0;
			else flag = 1;
		}
		else if (state == 0) flag = 0;
		else if (state == k+1){
			barr1 = (double)(l)/(double)(l+m+m);
			barr2 = (double)(l+m+(p*m))/(double)(l+m+m);
			coin = r2();
			if (coin<=barr1) flag = 0;
			else if (coin<=barr2) flag = 1;
			else flag =2;
			}
		else if (state==9){
			barr1 = (double)(l)/(double)(l+m);
			coin = r2();
			if (coin<=barr1) flag = 0;
			else flag = 2;
			}
		else {
			barr1= (double)(l)/(double)(l+m+m);
			barr2 = (double)(l+m)/(double)(l+m+m);
			coin = r2();
			if (coin<=barr1) flag = 0;
			else if (coin<=barr2) flag = 1;
			else flag = 2;
		}
		
	
		if (flag == 0){ 
			tot_arrivals++;
			arrivals[state]++;

			if (state==k+8) {
				state=k+1;
			}
			else if (state == 8) state = 8;
			else if (state == 2) { 
				if(r2()<p){
				state=k+8;
				}
				else{
				state++;
				}
			}
			else{
				state++;
			}
			
		}

		else  if (flag == 1){ 
			if (state == k+1){
				state=k+8;
			}
			else {
				state--;
			}
		}

		else if (flag == 2){ 
			if (state >=9){ 
				state=state - 9;
			}
			else {
				state--;
			}
		}
		
		if((i%1000) == 0 && i>0){
			average = 0;

			for(j=0;j<numstates;j++) {
			
				prob[j] = (double)arrivals[j]/(double)tot_arrivals;
				
			}
			
			g= (double)(l)*(1-prob[8]);
			g1=(double)(m)*(1-prob[0]-prob[9]);
			g2=(m)*(1-prob[0]);

			for(j=1;j<=k;j++) g2-=m*prob[j];
			for(j=0;j<=8;j++) average+=(double)(j)*prob[j];
			for(j=1;j<=k;j++) average+=(double)(j)*prob[8+j];
			

			if (fabs(average-oldaverage)<0.0000001 && flag1==0) { 
				flag1=1;
				time1=i;
				totalav=average;
			}
					
			oldaverage=average;
		}
		i++;
	}

	printf("Average 1:%f\n",totalav);
	printf("time1: %llu\n",time1);

	go=g1/g2;

	fprintf(fout1,"%f\n",g1);
	fprintf(fout,"time: %llu",time1);
	fprintf(fout1,"%f\n",g2);
	printf("%lf",go);

	free(prob);
	free(arrivals);

	fclose(fin);
	fclose(fout);
	fclose(f);
	fclose(fout1);

	return 0;
}
