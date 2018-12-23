
/*
0	reg[1] = 123;                 //
1	reg[1] = reg[1] & 456;        //
2	reg[1] = (reg[1] == 72);      // if (123 & 456 == 72)
3	reg[4] = reg[1] + reg[4];     // {}
4	reg[4] = 0;                   // else { while(true) {} }                    reg[1] = 0;
5	reg[1] = 0;                   // //                                         while (reg[1] != reg[0]) {
6	reg[5] = reg[1] | 65536;      // while (true) {                               reg[5] = reg[1] | 65536;
7	reg[1] = 8586263;             //                                              reg[1] = 8586263;
8	reg[2] = reg[5] & 255;        //                                              while (true) {
9	reg[1] = reg[1] + reg[2];     //                                                reg[1] += reg[5] & 255;
10	reg[1] = reg[1] & 16777215;   //   unsigned overflow                            reg[1] may overflow
11	reg[1] = reg[1] * 65899;      //                                                reg[1] *= 65899;
12	reg[1] = reg[1] & 16777215;   //   unsigned overflow                            reg[1] may overflow
13	reg[2] = (256 > reg[5]);      //   //
14	reg[4] = reg[2] + reg[4];     //   if (reg[5] >= 256)                           if (reg[5] >= 256)
15	reg[4] = reg[4] + 1;          //   //                                           {
16	reg[4] = 27;                  //   {
17	reg[2] = 0;                   //     reg[2] = 0;                                  reg[2] = 0;
18	reg[3] = reg[2] + 1;          //     //
19	reg[3] = reg[3] * 256;        //     //
20	reg[3] = (reg[3] > reg[5]);   //     if (reg[5] >= (reg[2] + 1) * 256)            while (reg[5] >= (reg[2] + 1) * 256)
21	reg[4] = reg[3] + reg[4];     //     {                                            {
22	reg[4] = reg[4] + 1;          //       reg[2]++;                                    reg[2]++;
23	reg[4] = 25;                  //       goto 18;                                   }
24	reg[2] = reg[2] + 1;          //     } else {
25	reg[4] = 17;                  //       reg[5] = reg[2];                           reg[5] = reg[2];
26	reg[5] = reg[2];              //       goto 8;                                  } else break;
27	reg[4] = 7;                   //     }                                        }
28	reg[2] = (reg[1] == reg[0]);  //   } else if (reg[1] == reg[0]) return;     }
29	reg[4] = reg[2] + reg[4];     //   //
30	reg[4] = 5;                   // }
*/

/*
Lines 17 through 26 set reg[5] to the least n such that
    reg[5] < (n + 1) * 256.
Now 256 < (1 + 1) * 256, 257 < (1 + 1) * 256, 511 < (1 + 1) * 256, 512 < (2 + 1) * 256.
Thus n = reg[5] / 256.

reg[1] = 0;
while (reg[1] != reg[0]) {
  reg[5] = reg[1] | 65536;
  reg[1] = 8586263;
  while (true) {
    reg[1] += reg[5] & 255;
    reg[1] reg[1] & 16777215;
    reg[1] *= 65899;
    reg[1] reg[1] & 16777215;
    if (reg[5] >= 256)
    {
      reg[5] = reg[5] / 256;
    }
    else break;
  }
}
*/
