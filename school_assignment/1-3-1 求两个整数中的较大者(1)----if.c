#include "stdio.h"
/*
题目描述
从键盘上输入以空格分隔的两个整数a和b，输出这两个数中较大的那个。
输入
输入两个整数a和b，以空格分隔。
输出
max=大值
样例输入
5 11
样例输出
max=11
*/
int main() {int a,b;
  scanf("%d %d", &a , &b);
  if (a >= b) {
    printf("max=%d", a);
  }
  else{
//    print("max=%d", b);
      printf("max=%d", b);
  }
  return 0;
}
