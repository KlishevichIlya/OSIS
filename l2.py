#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys

if __name__ == '__main__':
    
        largest_size = 0
        for file in os.listdir("."):
            if os.path.getsize(file) > largest_size:
                largest_size = os.path.getsize(file)
                largest_file = file
        print 'largest file: {} size: {} kB'.format(
                        largest_file, largest_size/1024)
