import cs50

# get the height of the pyramid from the user
# use a forever loop to ensure that the number of rows is between 1 and 8 only
while True:
    pyramidHeight = cs50.get_int("Height: ")
    if pyramidHeight in range(1,9):
        break

# print the rows
for row in range(1, pyramidHeight + 1):
    # print the columns
    for column in range(2 * row):

        # print spaces at the begining of each row
        if column == 0:
            for spaces in range(pyramidHeight - row):
                print(" ", end="")

        # print the bricks of the pyramid
        print("#", end="")

        # print the space between the 2 pyramids
        if column == row - 1:
            print("  ", end="")
    # end the row    
    print()