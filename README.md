# LZW-File-Compressor
Lempel–Ziv–Welch (LZW) is a universal lossless data compression algorithm created by Abraham Lempel, Jacob Ziv, and Terry Welch.

This compression program is implemented using C. It can be utilized to compress and decompress files.


Running instructions:
1) Run the make file in ```lzwLib```
2) Move back to the root project directory
3) To compile run: `cc -o lzw lzw.c -LlzwLib -llzw -IlzwLib/include/ -Wall`
4) Optional: Change input txt file to anything else
5) To Encode: `./lzw -encode`
6) To DecodeL `./lzw -decode`
7) To change min/max bits or path files, make the changes in lzw.c

Enjoy :)
