
#include <stdio.h>

int first_arr[] = {2,2,8};
int second_arr[] = {1,3,7};

int scalar(int* first_arr, int* second_arr, size_t length)
{
	int sum = 0;
	int i;

	for(i = 0; i < length; i++)
		sum += first_arr[i] * second_arr[i];
	return sum;
}

int is_prime(unsigned long n)
{
	unsigned long i;

	if(n <= 1) return 0;
	for(i = 2; i*i <= n; i++)
		if(n % i == 0) return 0;
	return 1;
}

int main(int argc, char** argv)
{
	unsigned long num;
	int i;
	/*for(i=0;i<3;i++)
	first_arr[i] = second_arr[i];*/
	
	printf("Scalar product is: %d\n", scalar(first_arr, second_arr, sizeof(first_arr)));
	printf("Enter your number to check for primary: ");
	scanf("%lu", &num);
	printf("Number %s\n", is_prime(num) ? "is prime" : "is not prime");
	return 0;
}
