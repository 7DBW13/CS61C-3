import random
import sys
print('v2.0 raw')
string = '0 '
hexdic = '1234567890abcdef'
for i in range(100):
  for j in range(8):
    string += hexdic[random.randint(0, 15)]
  string += ' '

print(string)
