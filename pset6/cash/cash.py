import cs50

# get change owed
while True:
    cash = cs50.get_float("Change owed: ")
    if cash >= 0:
        break

# convert the change into cents
cents = cash * 100
# initilize the number of coins
coins = 0

# add number of coins of quarters, then get the remainder
coins += cents // 25
cents %= 25
# add number of coins of dimes, then get the remainder
coins += cents // 10
cents %= 10
# add number of coins of nickles, then get the remainder
coins += cents // 5
cents %= 5
# add number of coins of pennies
coins += cents

# print the number of coins
print(int(coins))