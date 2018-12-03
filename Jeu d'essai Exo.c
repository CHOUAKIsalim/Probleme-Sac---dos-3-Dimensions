#include <stdio.h>
#include<stdlib.h>
#define nbObject 20
#define maxWeight 520
#define maxVolume 300
#define volumeScale 100

//Result Table
int result2D [nbObject][maxWeight+1];
int result3D [nbObject][maxWeight+1][maxVolume+1];

//Example Data
int weights[]={20,30,50,20,40,60,30,10,14,36,72,86,5,3,7,23,49,57,69,12};
int gains[]={6,8,14,6,13,17,10,4,5,11,26,35,2,1,2,7,15,17,30,3};
float volumes[]={0.10,0.15, 0.25, 0.10,0.20, 0.30, 0.15, 0.05, 0.07, 0.18 ,0.36, 0.43, 0.03 ,0.02 ,0.04, 0.12, 0.25, 0.29, 0.35 ,0.06};


// Structure for the Objects
typedef struct  {
		int  weight;
		int  volume;
		int  gain;
	}object;
object data[nbObject];

//Max Function
int max(int a , int b ) {
	if(a>b) return a;
	return b;
}


//Recursive function
int f (int n , int m, float v ){
	if((m<0)||(v<0))  	    return -data[n+1].gain;
	if((m==0)||(v==0)||(n==-1)) return 0;
	return max(
		f(n-1,m,v),
		f(n-1,m-data[n].weight,v-data[n].volume) + data[n].gain
		);
}


//Print the objects
void printData(){
	int i;
	for(i=0;i<nbObject;i++)
		printf("%d  Weight : %d, Volume %d Gain : %d \n",i,data[i].weight,data[i].volume,data[i].gain);
}

//feel Table with random data
void feelData (){
	int i ;
	for( i =0;i<nbObject;i++){
		data[i].weight=weights[i];
		data[i].volume=volumes[i]*volumeScale;
		data[i].gain=gains[i];
	}

	printf("\nObjects:\n");
	printData();
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
int printToCSV(){
    FILE *f = fopen("result3D.csv", "w");
    if (f == NULL) return -1;
    int i,j,k;
    for(i=0; i<nbObject ; i++){
    fprintf(f,"i== %d\n",i);
        for(j=0; j<maxWeight; j++){
            fprintf(f,"\n");
            for(k=0; k<maxVolume; k++)
                fprintf(f,"%d ;",result3D[i][j][k]);
        }
    }


  fclose(f);
    return 0;

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
int main(){
	feelData();
	arrayResolution3D();
	getChoosenObjects();
    return 0;
}
