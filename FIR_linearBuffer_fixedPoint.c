// ref: https://www.youtube.com/watch?v=uNNNj9AZisM
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define input_Len 30
#define FIR_FILTER_LENGTH 15
static int16_t coefficients[FIR_FILTER_LENGTH] = {-57,-64,0,335,1062,2050,2926,3277,2926,2050,1062,335,0,-64,-57};
// these coefficients were generated from MATLAB file (FIRfilterDesign_plusWindowDemo.m)

int8_t Q = 14;  // Q refers to the fractional part's number of bits
#define K   (1 << (Q - 1))
// saturate to range of int16_t
int16_t sat16(int32_t x)
{
	if (x > 0x7FFF) return 0x7FFF;
	else if (x < -0x8000) return -0x8000;
	else return (int16_t)x;
}
int16_t q_add_sat(int16_t a, int16_t b)
{
    int16_t result;
    int32_t tmp;

    tmp = (int32_t)a + (int32_t)b;
    if (tmp > 0x7FFF)
        tmp = 0x7FFF;
    if (tmp < -1 * 0x8000)
        tmp = -1 * 0x8000;
    result = (int16_t)tmp;

    return result;
}
int16_t q_mul(int16_t a, int16_t b)
{
    int16_t result;
    int32_t temp;

    temp = (int32_t)a * (int32_t)b; // result type is operand's type
    // Rounding; mid values are rounded up
    temp += K;
    // Correct by dividing by base and saturate result
    result = sat16(temp >> Q);

    return result;
}


typedef struct{
	int16_t buf[FIR_FILTER_LENGTH];
	int16_t out;	 //current filter output
} FIRFilter;  
void FIRFilter_Init(FIRFilter *fir){
	// reset everything to 0
	/* clear filter buffer */
	for (uint8_t n=0;n<FIR_FILTER_LENGTH;n++){
		fir->buf[n]=0;
	}
	
	/* clear filter output*/
	fir->out =0;
}
int16_t FIRFilter_Update_fixed(FIRFilter *fir,int16_t inp){
	//inp: current input
	//return: current output
	
	/* move memory */
	for(uint8_t n=FIR_FILTER_LENGTH-1;n>0;n--)
		fir->buf[n] = fir->buf[n-1];
	/* store the lastest sample in buffer */
	fir->buf[0] = inp;
	
	/* compute output*/
	fir->out = 0;

	for(uint8_t n=0; n< FIR_FILTER_LENGTH;n++){
		fir->out = q_add_sat( fir->out , q_mul(coefficients[n], fir->buf[n]) )  ; 
	}
	return fir->out;
}
int main(){
	FIRFilter *fir = NULL;
	fir = (FIRFilter*) malloc(sizeof(FIRFilter) );
	FIRFilter_Init(fir);
	int16_t *sig = calloc(input_Len,sizeof(int16_t));
	int16_t *filter_sig = calloc(input_Len,sizeof(int16_t));
	printf("\n input\n");
	for (uint16_t k=0;k<input_Len;k++){
		sig[k]=(0.8*sin(0.15*M_PI*k)+0.2*sin(0.93*M_PI*k)+0.1*sin(0.77*M_PI*k))*16384;
		printf("%d\n",*(sig+k));
	}
	
	printf(" output\n");
	clock_t start_t, end_t, total_t;
	uint16_t k;
	start_t = clock();
	//for (int m=0;m<1000;m++){
	for (k = 0;k<input_Len;k++){
		filter_sig[k] = FIRFilter_Update_fixed(fir,sig[k]);
		printf("%f\n",(float)*(filter_sig+k)/16384.);
	}
	//}
	end_t = clock();
	total_t = (double)(end_t - start_t) ;
	printf("\n linear Buffer Total time taken by CPU: %f \n",(float)total_t/ CLOCKS_PER_SEC  );
	
	free(fir);
	
	
	return 0;
}
