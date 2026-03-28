# Here, we will mimic the "80-20" workload from the book (see pages 234 and 235). 
    # - Here, 80% of the references are made to 20% of the pages (the “hot” pages); the remaining 20% of the references are made to the re- maining 80% of the pages (the “cold” pages). In our workload, there are a total 100 unique pages again

import random

addresses = []

for _ in range(1000):
    # Here, 80% of the references are made from the numbers 0 to 19 (inclusive).
    if _ % 10 in [2, 3, 4, 5, 6, 7, 8, 9]:
        addresses.append(random.randint(0, 19))
    # And the other 20% of the references are made from the numbers 20 to 100 (inclusive).
    else:
        addresses.append(random.randint(20, 100))

random.shuffle(addresses)

with open('addresses_with_locality.txt', 'w') as f:
    for addr in addresses:
        print(addr, file=f)
