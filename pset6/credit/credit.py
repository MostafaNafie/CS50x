import cs50

# get the credit card number
creditCardNumber = cs50.get_float("Number: ")

# initialize a variable for the sum of every other digit starting from the last digit
sumLast = 0
# initialize a variable for the sum of every other digit starting from the second to last digit
sumSecondLast = 0
# check whether the current digit is the last or second to last
isLast = True
# initialize a variable to ckeck the first 2 digits of the credit card number to get its type -VISA, AMERICAN EXPRESS, etc..-
firstDigits = creditCardNumber
# number of digits
digitsCount = 0

# a while loop which runs while the creditCardNumber not equal zero
# if creditCardNumber equals any number other than zero it returns true
while creditCardNumber:
    # check if this is the turn to get last digit
    if isLast:
        # get the last digit by using MOD10
        sumLast += creditCardNumber % 10
        # Change the turn to the second to last digit
        isLast = False
    else:
        # this will run in the second to last digit turn
        # get the second to last digit and multiply it by 2
        currentNumber = 2 * (creditCardNumber % 10)
        # loop through the digits of the result
        while currentNumber:
            sumSecondLast += currentNumber % 10
            # move back one digit by dividing by 10
            currentNumber //= 10
        # Change the turn to the last digit
        isLast = True

    # move back one digit by dividing by 10
    creditCardNumber //= 10
    digitsCount += 1

# when the while loop finishes add the two sums
total = sumLast + sumSecondLast
# check the type of visa if it's valid
if total % 10 == 0:
    # get the first 2 digits of the credit card number
    # if I want the first digit only change the 100 to 10
    while firstDigits >= 100:
        firstDigits = firstDigits // 10
            
    if ((firstDigits == 34 or firstDigits == 37) and (digitsCount == 15)):
        print("AMEX")
    elif ((firstDigits == 51 or firstDigits == 52 or firstDigits == 53 or firstDigits == 54 or firstDigits == 55) and (digitsCount == 16)):
        print("MASTERCARD")
    elif (firstDigits//10 == 4 and (digitsCount == 13 or digitsCount == 16)):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")

