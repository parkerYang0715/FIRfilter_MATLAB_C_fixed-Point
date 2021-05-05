#include<stdio.h>
// precomputed value:

int8_t Q = 5;  // Q refers to the fractional part's number of bits
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

int main(){
	
	float a = 15.5;
	float b = 31.7;
	int16_t fixed_a = a*32;
	int16_t fixed_b = b*32;
	int16_t fixed_c = q_mul(fixed_a, fixed_b);
	printf("fixed ans = %d\n",fixed_c);
	printf("float ans = %f\n",(float)fixed_c/32);
	
	
	return 0;
}

// ref: https://en.wikipedia.org/wiki/Q_(number_format)