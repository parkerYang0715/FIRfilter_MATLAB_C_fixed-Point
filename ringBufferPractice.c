// ref: https://www.youtube.com/watch?v=uNNNj9AZisM
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define input_Len 9000
#define FIR_FILTER_LENGTH 15
static float coefficients[FIR_FILTER_LENGTH] = {-0.0035,-0.0039,0.,0.0205,0.0648,0.1251,0.1786,0.2000,0.1786,0.1251,0.0648,0.0205,0.0,-0.0039,-0.0035};
typedef struct{
	float buf[FIR_FILTER_LENGTH];
	uint8_t bufIndex;
	float out;	 //current filter output
} FIRFilter;  
void FIRFilter_Init(FIRFilter *fir){
	// reset everything to 0
	/* clear filter buffer */
	for (uint8_t n=0;n<FIR_FILTER_LENGTH;n++){
		fir->buf[n]=0.0f;
	}
	
	/* Reset buffer index*/
	fir->bufIndex =0;
	
	/* clear filter output*/
	fir->out =0.0f;
}
float FIRFilter_Update(FIRFilter *fir,float inp){
	//inp: current input
	//return: current output
	
	/* store the lastest sample in buffer */
	fir->buf[fir->bufIndex] = inp;
	
	/* increment the buffer index and wrap around if necessary */
	fir->bufIndex++;
	if (fir->bufIndex == FIR_FILTER_LENGTH){
		fir->bufIndex = 0;
	}
	
	/* compute output*/
	fir->out = 0.0f;
	uint8_t sumIndex = fir->bufIndex;
	for(uint8_t n=0; n< FIR_FILTER_LENGTH;n++){
		if(sumIndex>0)
			sumIndex--;
		else
			sumIndex = FIR_FILTER_LENGTH - 1;
		
		fir->out += coefficients[n] * fir->buf[sumIndex];
	}
	return fir->out;
}
int main(){
	FIRFilter *fir = NULL;
	fir = (FIRFilter*) malloc(sizeof(FIRFilter) );
	FIRFilter_Init(fir);
	float *sig = calloc(input_Len,sizeof(float));
	float *filter_sig = calloc(input_Len,sizeof(float));
	printf("\n input\n");
	for (uint16_t k=0;k<input_Len;k++){
		sig[k]=0.8*sin(0.15*M_PI*k)+0.2*sin(0.93*M_PI*k)+0.1*sin(0.77*M_PI*k);
		//printf("sig[%d]=%f\n",k,*(sig+k));
		//printf("%f\n",*(sig+k));
	}
	printf(" output\n");
	clock_t start_t, end_t, total_t;
	uint16_t k;
	start_t = clock();
	for (int m=0;m<1000;m++){
	for (k = 0;k<input_Len;k++){
		filter_sig[k] = FIRFilter_Update(fir,sig[k]);
		//printf("filter_sig[%d]=%f\n",k,*(filter_sig+k));
		//printf("%f\n",*(filter_sig+k));
	}
	}
	end_t = clock();
	total_t = (double)(end_t - start_t) ;
	printf("\n ring Buffer Total time taken by CPU: %f \n", (float)total_t/ CLOCKS_PER_SEC  );
	printf("filter_sig[%d]=%f\n",k-1,*(filter_sig+k-1));
	free(fir);
	return 0;
}