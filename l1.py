import os
import re

print 'pattern:'
pattern = raw_input()
if len(pattern) == 0:
    pattern = '.*.cpp'
if pattern[0] != '.':
    pattern = '.'+pattern
    
print 'result file:'
res_file = raw_input()
if len(res_file) == 0:
    res_file = 'D:\\sp\\file.txt'

found_files = []
for dir_, subdirs, files in os.walk('D:\\sp'):
    for f in files:
        if re.search(pattern, f):
            found_files.append(f)
            
print found_files
    
with open(res_file, 'a') as f:
    f.write('search files {}\n'.format(pattern))
    for i in found_files:
        f.write(i + '\n')
    
