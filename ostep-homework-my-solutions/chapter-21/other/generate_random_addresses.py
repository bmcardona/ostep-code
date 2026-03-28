import random

with open('random_addresses.txt', 'w') as f:
    for _ in range(1000):
        print(random.randint(1, 100), file=f)
