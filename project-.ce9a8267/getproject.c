#include <stdio.h>
#include <string.h>

int hasNineDigits(const char number[]);

int main()
{
    char number[20]; // 10 characters for 9 digits + null terminator
    int easy;
    int medium;
    int hard;
    int i;

    printf("Enter your 9-digit student number including leading 0's: ");
    scanf("%s", number);

    while(strlen(number) != 9 || !hasNineDigits(number)){
        printf("Invalid input. Please enter exactly 9 digits.\n");
        printf("Enter your 9-digit student number including leading 0's: ");
        scanf("%s", number);
    }

    // Store the required digits into variables
    easy = number[3] - '0'; 
    medium = (number[4] - '0') * 10 + (number[5] - '0'); 
    hard = number[6] - '0';

    easy = easy % 7 + 1;
    medium = medium%15 + 1;
    hard = hard % 6 + 1;
    printf("Based on your student number: ");

    for(i = 0;i<3;i++){
        printf("%c",number[i*3]);
        printf("%c",number[i*3+1]);
        printf("%c",number[i*3+2]);
        if(i!=2){
            printf("-");
        }
        else{
            printf("\n");
        }
    }

    printf("Your Problem set is as follows: \n");
    printf("    A%d\n",easy);
    printf("    B%d\n",medium);
    printf("    C%d\n",hard);
    return 0;

}

int hasNineDigits(const char number[])
{
    int rc = 1;
    for (int i = 0; rc==1 && i < 9; i++) {
        if (number[i] < '0' || number[i] > '9') {
            rc = 0;
        }
    }
    return rc;
}
