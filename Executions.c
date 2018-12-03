#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#define max1 56
#define max2 1000
#define max3 1000

//Variables
//Example Data
int weights[]={20,30,50,20,40,60,30,10,14,36,72,86,5,3,7,23,49,57,69,12};
int gains[]={6,8,14,6,13,17,10,4,5,11,26,35,2,1,2,7,15,17,30,3};
float volumes[]={0.10,0.15, 0.25, 0.10,0.20, 0.30, 0.15, 0.05, 0.07, 0.18 ,0.36, 0.43, 0.03 ,0.02 ,0.04, 0.12, 0.25, 0.29, 0.35 ,0.06};


//Values
int nbObject,maxWeight,maxVolume;
int result3D [max1][max2][max3];
// Structure for the Objects
typedef struct  {
		int  weight;
		int  volume;
		int  gain;
	}object;
object data[max1];


//Max Function
int max(int a , int b ) {
	if(a>b) return a;
	return b;
}

//Recursive function
int f (int n , int m, float v ){
	if((m<0)||(v<0)) return -data[n+1].gain;
	if((m==0)||(v==0)||(n==-1)) return 0;
	return max(
		f(n-1,m,v),
		f(n-1,m-data[n].weight,v-data[n].volume) + data[n].gain
		);
}


//table accesor for the resolution with 3D Array
int value(int i , int j , int k ){
	if((j<0)||(k<0)) return -data[i+1].gain;
	if((i<0)) return 0;
	return result3D[i][j][k];

}
//Resolution with a 3D array
void arrayResolution3D(){
	int i,j,k;
	for(i=0; i<nbObject ; i++)
	for(j=0; j<maxWeight; j++)
	for(k=0; k<maxVolume; k++)
		result3D[i][j][k]= max(value(i-1,j,k),
				      value(i-1,j-data[i].weight,k-data[i].volume)+data[i].gain);

}

//Show the 3D result table
void printResultTable(){
	int i,j,k ;
	printf("Result Table : \n");
	for(i=0;i<nbObject ;i++){
	printf(" i= %d \n",i);
		for(j=0;j<maxWeight;j++){
		for(k=0;k<maxVolume;k++)
			printf("| %2d |",result3D[i][j][k]);
		printf("\n");
		}
	}

}

void getChoosenObjects(){
    int values[nbObject];
    int i,j,k;
    j=maxWeight;
    k=maxVolume;
    for(i=nbObject-1; i>=0;i--){
        values[i]= (value(i-1,j,k)<value(i-1,j-data[i].weight,k-data[i].volume)+data[i].gain);
        j=j-values[i]*data[i].weight;
        k=k-values[i]*data[i].volume;
        }

    printf("Volume is scaled*100\nChoosenValues : \n i=  |yes/no | gain[i] |Total Gain| Volume   |Weight | \n");
        int totalGain=0;
        int totalWeight=0;
        int totalVolume=0;
    for(i=0;i<nbObject;i++)
        printf("%4d | %4d  |  %4d   | %3d      |   %3d   |   %3d  |\n",i,values[i],data[i].gain,totalGain+=values[i]*data[i].gain,totalVolume+=values[i]*data[i].volume,totalWeight+=values[i]*data[i].weight);

}

void feelRandomData(){
	int i;
	for(i=0;i<nbObject;i++){
		data[i].gain = rand()%10;
		data[i].weight=rand()%10;
		data[i].volume=rand()%10;
	}

}


void runTests2(){

    time_t t1,t2;
    float time_taken;
    int sizes[]={5,10,15,20,25,30,32,34,35,36,37,38,39,40,45,50,55};
    int i;

    //Data Array with the maximum of Objects
    nbObject=max1;
    feelRandomData();


    //File for saving records
    FILE *file = fopen("results.txt", "w");

    for(i=0;i<17;i++){

        nbObject=sizes[i];
        maxWeight=(5)*sizes[i];
        maxVolume=(5)*sizes[i];

        printf("\nSize= %d, Weight = %d , Volume = %d ",nbObject,maxWeight,maxVolume);
        fprintf(file,"\nSize= %d, Weight = %d , Volume = %d ",nbObject,maxWeight,maxVolume);

        //Array Resolution
        t1=clock();
		arrayResolution3D();
		t2=clock();

		time_taken = ((double)t2-t1)/CLOCKS_PER_SEC;
		printf("\nArrayResolution : = %f",time_taken );
		fprintf(file,"\nArrayResolution %f",time_taken);

		//Recursive Resolution
		t1=clock();
		f(nbObject,maxWeight,maxVolume);
		t2=clock();


    }

fclose(file);
}
int main(){
	runTests2();
    return 0;
}
