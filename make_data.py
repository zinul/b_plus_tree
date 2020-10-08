import random
import string
import sys

def random_name(size):
    char_lists = string.ascii_lowercase
    return ''.join(random.choice(char_lists) for _ in range(size))


def random_ip():
    int_ip = random.randint(1, 2**32)
    address = ''
    for _ in range(4):
        ip = int_ip % (2**8)
        int_ip >>= 8
        address = address+str(ip)+'.'
    return address[0:-1]

for i in range(int(sys.argv[1])):
    print(random_ip(), end='\t')
    print(random_name(random.randint(3,10))+".com")
