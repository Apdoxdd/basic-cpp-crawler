# Web Crawler
## Basic Web Crawler (C+23) implemeted on top of libcurl and libxml2

### What it does
    It starts from a base URL seed in the first line of the 'data_base.txt' file, crawls into it, gets all the links embedded in the
    received document, appends those links the end of the data base, after so it crawls into the next link in line
    and so on.

### What it needs to work
    1) OpenSSL on the system for libcurl to start
    2) A seed url in the first row of the 'data_base.txt' file


THIS IS PROOF OF CONCEPT ONLY NOT INTEDED FOR ANY ACTUAL USE
