#include <stdio.h>
#include <cs50.h>

void checkCreditCard(long creditCardNumber)
{
    // initialize a variable for the sum of every other digit starting from the last digit
    int sumLast = 0;
    // initialize a variable for the sum of every other digit starting from the second to last digit
    int sumSecondLast = 0;
    // to check whether the current digit is the last or second to last
    bool last = true;
    // initialize a variable to ckeck the first 2 digits of the credit card number to get its type -VISA, AMERICAN EXPRESS, etc..-
    long firstDigits  = creditCardNumber;
    // number of digits
    int digitsCount = 0;
    
    // a while loop which runs while the creditCardNumber not equal zero
    // if creditCardNumber equals any number other than zero it returns true
    while(creditCardNumber)
    {
        // check if this is the turn to get last digit
        if (last)
        {
            // get the last digit by using MOD10
            sumLast += creditCardNumber % 10;
            // Change the turn to the second to last digit
            last = false;
        } else 
        {
            // this will run in the second to last digit turn
            // get the second to last digit and multiply it by 2
            long currentNumber = 2 * (creditCardNumber % 10);
            // loop through the digits of the result
            while(currentNumber)
            {
                sumSecondLast += currentNumber % 10;
                // move back one digit by dividing by 10
                currentNumber /= 10;
            }
            // Change the turn to the last digit
            last = true; 
        }
        // move back one digit by dividing by 10
        creditCardNumber /= 10;
        digitsCount ++;
    }
    // when the while loop finishes add the two sums
    int total = sumLast + sumSecondLast;
    // check the type of visa if it's valid
    if (total % 10 == 0)
    { 
        // get the first 2 digits of the credit card number
        // if I want the first digit only change the 100 to 10
         while(firstDigits >= 100)
        {
            firstDigits = firstDigits / 10;
        }
        
        if ((firstDigits == 34 || firstDigits == 37) && (digitsCount == 15))
        {
            printf("AMEX\n");
        } else if ((firstDigits == 51 || firstDigits == 52 || firstDigits == 53 || firstDigits == 54 || firstDigits == 55) && (digitsCount == 16))
        {
            printf("MASTERCARD\n");
        }else if (firstDigits/10 == 4 && (digitsCount == 13 || digitsCount == 16))
        {
            printf("VISA\n");
        }else
        {
            printf("INVALID\n");
        }
    } else
        {
            printf("INVALID\n");
        }
    
}

int main(void)
{
    // get the credit card number from the user
    long creditCardNumber = get_long("Number: ");
    // call the function to check the credit card number
    checkCreditCard(creditCardNumber);
    
}
