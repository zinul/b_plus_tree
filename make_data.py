import random
import string
import sys

def random_name(size):
    char_lists = string.ascii_lowercase
    return ''.join(random.choice(char_lists) for _ in range(size))


def random_ip():
    int_ip = random.randint(1, 2**32)
    temp=int_ip
    address = ''
    for _ in range(4):
        ip = temp % (2**8)
        temp >>= 8
        address = address+str(ip)+'.'
    return [address[0:-1],int_ip]

for i in range(int(sys.argv[1])):
    print(random_ip()[0], end='\t')
    print(random_name(random.randint(4,10))+".com",end='\t')
    print(random_ip()[1])
    
