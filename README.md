# LZW-File-Compressor
Lempel-Ziv-Welch file compression program implemented using C. Compress and decompress files.

Running instructions:
1) Run the make file in /lzwLib
2) Move to the parent directory
3) To compile run: cc -o lzw lzw.c -LlzwLib -llzw -IlzwLib/include/ -Wall
4) Optional: Change input txt file to anything else
5) To Encode: ./lzw -encode
6) To DecodeL ./lzw -decode
7) To change min/max bits or path files, make the changes in lzw.c

Enjoy :)
