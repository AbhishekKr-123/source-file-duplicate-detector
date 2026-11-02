#include <stdio.h>
#include <math.h>

int main() {
    int num;

    printf("Enter Number: ");
    scanf("%d", &num);

    int original = num;
    int temp = num;
    int count = 0;

    // Count the number of digits
    while (temp > 0) {
        count++;
        temp /= 10;
    }

    // Calculate Armstrong sum
    temp = num;
    int sum = 0;

    while (temp > 0) {
        int digit = temp % 10;
      sum += (int)round(pow(digit, count));
        temp /= 10;
    }

    // Display result
    printf("Count = %d\n", count);
    printf("Sum = %d\n", sum);

    if (sum == original)
        printf("%d is an Armstrong Number.\n", original);
    else
        printf("%d is NOT an Armstrong Number.\n", original);

    return 0;
}