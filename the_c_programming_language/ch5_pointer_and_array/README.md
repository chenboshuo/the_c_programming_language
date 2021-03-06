# 第五章 指针与数组

## 5.1 指针与地址
机器有一系列连续编号或编址单元,这些存储单元可以单个进行操纵,也可以连续成组的方式进行操纵.通常情况下,机器的一个字节可以存放一个char类型数据,两个相邻字节可以存放一个short类型数据,4个相邻字节可以存放一个long类型数据.指针能够存放一个地址或者一组存储单元(通常是2个或4个字节).

一元运算符`&`可用于取一个对象的地址,因此,下列语句:
```cpp
p = &c
```
将把c的地址赋给p,称p为指向p的指针. 地址运算符只用于内存中的对象,即变量与数组元素, 它不能作用于表达式,常量或者`register`类型的变量.

一元运算符`*`是间接寻址或间接引用运算符当它作用于指针时,将访问指针所指的对象.

一元运算符`*`与`&`的优先级比算数运算优先级高,因此,赋值语句:
```cpp
*p += 1
```
将*p的对象的值取出并+1,它等同于
```cpp
++*p
// 或者
(* p)++
```
语句`*(p)++`括号必不可少,否则表达式会对p加一而不是指向的对象

## 5.2 指针与函数参数
由于C中是以传值得方式将参数传给调用函数,因此,被调用的函数不能直接修改主函数中变量的值.例如,函数定义为以下形式:
```cpp
void swap(int x, int y) {
  int temp;

  temp = x;
  x = y;
  y = temp;
}
```
则下列语句无法达到目的
```cpp
swap(a, b);
```
这是因为上述函数不会影响到调用它的例程中参数a,b的值,仅仅交换了副本的值;

实现目标可以使主调程序指向所要交换的变量的指针传递给被调用函数,即
```cpp
swap(&a, &b);
```
由于一元运算符`&`用来取地址,这样`&a`就是一个指向变量a的指针.`swap`函数所有参数声明为指针,并且通过指针间接访问他们的操作数.
```cpp
void swap(int *px, int *py) {
  int temp;

  temp = * px;
  * px = * py;
  * py = temp;
}
```

来看这样一个例子:

函数getint接受自由格式的输入,并执行转换,将输入的字符流分解成整数,每次调用得到一个整数.getint返回得到的整数,并且,在到达输入结尾时要返回文件结束的标记.这些值必须通过不同方式返回.EOF可以用任何值表示,也可以用一个输入值表示

## 5.3 指针与数组

`int a[10]` 定义了一个长度为10的由10个对象组成的集合,这十个对象存储在相邻内存区域中,`a[i]`表示第i个元素,若pa声明为
```cpp
int *pa;
```
那么他是一个指向整型对象的指针,那么,赋值语句
```cpp
pa = &a[0];
```
则可以指向数组a的第0个元素,也就是说,pa的值为数组元素`a[0]`的地址.
```cpp
x = *pa;
```
把`a[0]`的内容复制到变量x中.

执行语句
```cpp
pa = &a[0]
```
后, pa与a具有相同的值,因为数组名所代表的是该数组第一个元素的地址,所以上面语句也可以写成
```cpp
pa = a;
```
在计算数组`a[i]`的值时,c语言实际上先将其转化为`*(a+i)`的形式.然后再进行求值.

指针是一个变量,`p++`是合法的,数组名不是变量,`a++`语句是非法的

当把数组名传递给一个函数时,实际传递的是第一个元素的地址. 调用函数中,该参数是一个局部变量, 因此,数组名参数必须是一个指针, 也就是一个存储地址值得变量.
``` cpp
int strlen(char *s){
  int n;

  for (n = 0; *s != '\0'; s++) {
    n++;
  }
  return n;
}
```
-   [代码](https://github.com/chenboshuo/c_learning/blob/f33b5a8daa156bb99621d6b63a421a2adb6d3f0d/the_c_programming_language/ch5_pointer_and_array/strlen.c)
执行s++运算并不会影响调用者的字符串,它仅对该指针在strlen的副本中进行自增运算.

在函数定义中,形式参数
``` cpp
char s[]
```
和
```cpp
char *s;
```
是等价的. 我们通常更习惯后一种形式,因为他比前者更直观表明这个参数是个指针.如果把数组名传给函数,函数可以根据情况判定是按照数组处理还是按照指针处理,然后根据相应的方法操作参数. 为了直观且恰当地表示主函数,函数中甚至可以同时使用数组和指针两种方法.

也可以将指向子数组起始位置的指针传给函数,这样,就将数组的一部分传给函数,例如
```cpp
f(&a[2])
```
与
```cpp
f(a+2)
```
都将把起始于`a[2]`的子数组的地址传给f,对于函数来说,它并不关心所引用的是否是一个更大数组的部分元素.

如果确信相应的元素存在,也可以通过下标访问第一个元素之前的元素.`p[-1]`在语法上是合法的,当然, 引用数组边外的对象是非法的.

## 5.4 地址算术运算

C语言中地址运算符是一致且有规律地, 将指针,数组和地址运算符集成在一起是该语言的一大优点.我们来看一个不完善的存储分配程序.

程序由两个函数组成, 第一个函数alloc(n)返回一个指向n个连续字符存储单元的指针,alloc函数的调用者可利用该指针存放字符序列,第二个函数释放存储空间,以便以后重用. 之所以说两个函数是不完善的, 是因为对afree函数调用次序必然与调用alloc的次序相反. 换句话说,alloc与afree 是以栈的方式(后进先出)进行存储空间的管理. 标准库中提供类似的功能malloc和free,他们没有这些限制.

最容易实现的方法是让alloc函数对一个大字符数组allocbuf中的空间进行分配.该数组是alloc和afree的私有数组. 由于alloc和afree处理的对象是指针而不是数组下标,因此,其他函数无需知道该数组的名字,这样,可以在包含alloc和afree的源文件中将该数组声明为static类型,使他对外不可见. 实际实现时,该数组甚至可以没有名字, 它可以通过调用malloc函数或者向操作系统申请一个无名存储块的指针获得.

allocbuf中的空间使用状况也是我们需要了解的信息.我们使用指针allocp指向allocbuf中的下一个空闲单元.当调用alloc申请n个字符的存储空间时,alloc检查allocbuf中有没有足够的剩余空间,则alloc返回allocp的当前值(即空闲位置开始位置),然后将alloc指向下一个空闲区域.如果空闲不够,则alloc返回0. 如果p在allocbuf的边界之内,则afree(p)仅仅只是将alloc的值设为p

-[代码](https://github.com/chenboshuo/c_learning/blob/6bf92f5a21317654905cd144c85738f33c52074b/the_c_programming_language/ch5_pointer_and_array/memorry-allocation.c)

C语言保证0永远不是有效的地址,因此返回值为0可以表示发生了异常事件;

指针与整数之间不能相互转换,但0是唯一例外,0可以赋值给指针,指针也可以和0比较.程序中经常用`NULL`代替0,这样清晰的说明0是指针的一个特殊值.`NULL`定义在`<stddef.h>`中.

指针的减法是有意义的:如果p和q指向相同数组中的元素,且`p<q`那么`q-p`就是位于p和q之间元素的数目

于是可以写出strlen的[另一个版本](https://github.com/chenboshuo/c_learning/commit/a03558fbdb90460c7f09d1304a720860b5c7721f)

```cpp
int strlen(char *s){
  char * p = s;

  for (;*p != '\0'; p++) {
    ;
  }
  return p - s;
}

int main(int argc, char const *argv[]) {
  printf("%d\n", strlen("abc"));
  return 0;
}
```

指针的算术运算具有一致性:
如果处理的数据类型是比字符型占据更多空间的浮点型,p是一个指向下一个浮点数的地址

有效指针运算包括:
-   指针同整数之间的加法或减法运算
-   指向相同数组中元素的两个指针的减法或比较运算
-   将指针赋值0或者与指针0之间的比较运算

其他运算是非法的,例如:
-   两个指针之间的加法,乘法,除法, 移位或屏蔽运算
-   指针同float或double之间的加法运算
-   不经过强制类型转换而直接将指向一种类型对象的指针赋值给另一种类型对象的指针运算(两个指针之一是`void*` 类型除外)

# 5.5 字符指针与函数
字符串常量是一个字符数组,例如:
```cpp
"I am a student"
```
在字符串的内部表示中,字符串与'\0'结尾,所以,程序可以通过检查空字符找到字符串的结尾.字符串常量占据的存储单元数因此比双引号中的字符数大1.

字符串常量的最常见用法也许是作为函数的参数:
```cpp
printf("hello world");
```
当类似这样的一个字符串出现在程序中时,实际上是通过字符指针访问该字符串的.也就是说,字符串常量可以通过一个指向其第一个元素的指针访问.
```cpp
char *pmessage;
pmessage = "now is the time"
```
语句将把一个指向该字符数组的指针赋给pmessage. 该过程并没有涉及到指针的操作. C语言没有提供将字符串作为一个整体进行处理的运算符.

下面两个定义有很大差别:
```cpp
char amessage[] = "now is the time" // 定义一个数组
char *pmessage = "now is the time" // 定义一个指针
```
上述声明中,amessage是一个仅仅足够存放初始化字符串以及空字符串'\0'的一维数组. 数组中的单个字符可以修改,但amessage始终指向同一个存储位置.
另一方面,pmessage是一个指针,其初值指向一个字符串常量,之后它可以被修改指向其他的地址,但如果修改字符串的内容,结果是没有定义的
```
pmessage ------> now is the time\0
amessage:now is the time
```

为了进一步研究指针和数组的其他问题,下面以标准库中两个有用的函数为例来研究他们的不同版本.

第一个函数`strcpy(s,t)`把指针t指向的字符串复制到指针s的位置,如果用`s=t`实现,只是拷贝了指针,并没有复制字符. 为了进行字符串的复制,这里使用了一个循环语句.
[第一个版本](https://github.com/chenboshuo/c_learning/blob/34da608f6d28931d8df5b4554db40497302bef3c/the_c_programming_language/ch5_pointer_and_array/strcpy.c)如下
```cpp
void strcpy(char *s, char *t) {
  int i;

  i = 0;
  while ((s[i] = t[i]) != '\0') {
    i++;
  }
}
```
为了进行[比较](https://github.com/chenboshuo/c_learning/commit/7edfbfaff27b1c1e7ec0db52749cd64507e72694),下面用[指针的方法实现](https://github.com/chenboshuo/c_learning/blob/7edfbfaff27b1c1e7ec0db52749cd64507e72694/the_c_programming_language/ch5_pointer_and_array/strcpy.c):
```cpp
void strcpy(char *s, char *t) {
  while ((*s = *t) != '\0') {
    s++;
    t++;
  }
}
```
因为函数时通过值来传递的, 所以strcpy可以往任何方式使用s和t.

经验丰富的程序员更喜欢[以下形式](https://github.com/chenboshuo/c_learning/blob/57acf1378cdcedaa0955bcac361a46f4ebc88d5e/the_c_programming_language/ch5_pointer_and_array/strcpy.c)
```cpp
void strcpy(char *s, char *t) {
  while ((*s++ = *t++) != '\0') {
    ;
  }
}
```
-[对比](https://github.com/chenboshuo/c_learning/commit/57acf1378cdcedaa0955bcac361a46f4ebc88d5e)
`*t++`的值是执行自增运算前t指向的字符,后缀++表示在读取该字符后才改变t的值.
进一步精炼[程序](https://github.com/chenboshuo/c_learning/commit/e8062648236ba406f22d9e566faadc1a2e5b06de)
```cpp
void strcpy(char *s, char *t) {
  while (*s++ != *t++) {
    ;
  }
}
```
标准库`<string.h>`中提供的函数`strcpy`把目标字符串作为函数值返回;

下面我们研究字符串比较函数`strcmp(s, t)`比较字符串.该函数比较字符串s,t并根据s字典顺序小于,等于或大于t的结果分别返回负整数,0或正整数. 该返回值是s和t由前向后逐个字符比较时遇到的第一个不相等字符处字符的差值.
```cpp
int strcmp(char *s, char *t){
  int i;

  for (i = 0; s[i] == t[i]; i++) {
    if (s[i] == '\0') {
      return 0;
    }
  }
  return s[i] - t[i];
}
```
-   [code](https://github.com/chenboshuo/c_learning/blob/036373e4984027c7654e2236d997a22f94e7b306/the_c_programming_language/ch5_pointer_and_array/strcmp.c)

下面用指针的方法实现
```cpp
int strcmp(char *s, char *t){
  for ( ; *s == *t; s++, t++) {
    if (*s == '\0') {
      return 0;
    }
  }
  return * s - * t;
}
```
-   [代码](https://github.com/chenboshuo/c_learning/blob/73c04ed9a22c99efaf2603f69e48c4292cac9907/the_c_programming_language/ch5_pointer_and_array/strcmp.c) [对比](https://github.com/chenboshuo/c_learning/commit/73c04ed9a22c99efaf2603f69e48c4292cac9907)

下列表达式
```cpp
*--p
```
在读取指针之前先对p执行自减运算, 事实上, 下面两个表达式:
```cpp
*p++ = val;
val = *--p
```
是进出栈的标准方法.

头文件`<string.h>`包含本节课提到的函数声明, 还包括标准库中其他一些字符串处理函数的声明

## 5.6 指针数组与指向指针的指针
指针也是变量,所以他们也可以像其他变量一样存储在数组中.

该程序按照字母顺序对由文本组成的集合进行排序

我们引用指针数组来处理这种问题. 如果待排序的文本首行首尾相连的存储在一个长字符串中, 那么每个文本可以通过指向他的第一个字符的指针来访问. 这些指针本身可以存放在数组中. 这样,将指向两个文本行的指针传递给函数`strcmp`就可以实现两个文本行的比较. 当交换次序颠倒的两个文本行时,实际上交换的是指针数组中与这两个文本行相对应的指针,而不是文本行本身.

这种方法消除了因移动文本本身所带来的复杂存储管理和巨大的开销这两个孪生问题

排序包括以下三个步骤
1. 读取所有输入行
2. 对文本进行排序
3. 按次序打印文本行

通常情况下,最好将程序划分成若干个问题的自然划分 相一致的函数, 通过主函数控制其他函数执行.

输入输出函数必须收集和保留每个文本行中的字符, 并建立一个指向这些文本行的数组.他同时还必须统计输入的行数, 因为在排序和打印时要用到这一信息.由于输入函数只能处理有限数目的输入行,所以在输入行过多而超过限定的最大行数时,函数返回某个表示非法行数的值,比如-1.

输出函数只要按照指针数组的次序依次打印这些文本即可

-   [对swap稍微改动](https://github.com/chenboshuo/c_learning/commit/5f7a2691a248dda1834e0a864e7c85466b2088d3)
-   [完整代码](./sort-text.c)

## 5.7 多维数组
C语言提供了类似矩阵的多为数组, 但他们实际上并不像指针数组用的那样广泛. 我们考虑一个日期转化问题

把某年某月某日这种日期表示形式转换为某年中第几天的表示形式. 例如3.1是非闰年的第60 天, 是闰年的61 天. 在这里, 我们定义两个函数进行日期转换,同时用到一张记录每月天数的表, 对于闰年与非闰年来说, 每个月天数不同, 所以把这些天数放到二维数组中比较判断2月有多少天更容易.

-   [代码](https://github.com/chenboshuo/c_learning/blob/eed2663724721f6185f689a3cf6f1ed7dedf438b/the_c_programming_language/ch5_pointer_and_array/day.c)

```cpp
static char daytab[2][13] ={
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
}
```

如果将二维数组作为参数传递给数组, 那么在函数声明中必须指明数组的列数. 数组行数没有太大关系, 函数调用时传递的是一个指针, 它指向由行向量组成的一维数组, 其中每个行向量都是由13个整型元素的一维数组. 如果将`daytab`当作参数传给函数`f`,可以这样写:
```cpp
f(int daytab[2][13]){}
f(int daytab[][13]){}
```
因为行数无关紧要,所以可以写成
```cpp
f(int (*datab)[13])
```
这种声明表示参数是一个指针, 它指向具有13个整型元素的一维数组. 因为方括号[]的优先级高于*,所以上述声明必须用圆括号,如果去掉圆括号,声明变成
```cpp
int *daytab[13];
```
这相当于声明了一个数组, 该数组有13个元素, 其中每个元素都是一个指向整型对象的指针.

一般来说,除数组第一维(下标)可以不指定大小以外,其余个维必须指定大小.

## 5.8 指针数组的初始化

考虑这样一个问题, 编写一个函数math_name(n), 它返回一个指向n和月名字的字符串的指针.这是static类型数组的一种理想应用. mouth_name函数包括一个私有的字符串数组, 当它被调用时,返回一个指向正确元素的指针

-[代码](https://github.com/chenboshuo/c_learning/blob/d3737a0a5e24d51856c4bf6f06ca2b4452d7e1be/the_c_programming_language/ch5_pointer_and_array/mouth_name.c)

```cpp
char * mouth_name(int n){
  static char * name[] = {
    "illeagal mouth",
    "January", "Febuary", "March",
    "April", "May", "June",
    "July", "August", "September",
    "October", "November", "Deceber"
  };
}
```

name 是个一维数组,数组的元素为一位指针. name的初始化通过一个字符串列表实现,列表中每一个字符串赋值给数组相应位置的元素. 第i个字符串中所有字符存储在存储器中的某个位置,指向它的指针存在`name[i]`中. 上述声明没有指定name长度, 因此,编译器编译时对初值个数进行统计,并将这一准确数字填入数组长度.

## 5.9 指针与多维数组
假如有这两个定义
```cpp
int a[10][20];
int *b[10];
```
那么从语法上讲,`a[3][4]`和`b[3][4]`都是对数组的合法引用.但是a是一个真正的二维数组, 它分配了200个int长的存储空间, 并且通过常规的矩阵下标计算公式`20*row + col`得到`a[row][col]`的位置. 但是,对b来说,它只是分配了10 个指针,并没有对他们初始化, 他们的初始化必须通过显示方法进行, 比如静态初始化或者通过代码初始化. 假定b的每一个元素都指向20个元素的数组,那么编译器就要分配200个`int`类型长度的存储空间以及10个指针的存储空间. 指针数组的一个优点在于, 数组的每一行长度可以不同.

```cpp
char *name = {"illegal mounth", "Jan", "Feb"};
```
```
name:|___|
     |___+--> illegal mounth\0
     |___+--> JAN\0
     |___+--> Feb\0
```

```cpp
char aname[][15] = {"illegal mounth", "Jan", "Feb"};
```
```
aname:
0                15             30
illegal mounth\0 Jan\0          Feb\0
```

## 5.10 命令行参数
支持C的环境中, 可以在程序开始执行时将命令行参数传递给程序. 调用主函数main时, 它带有两个参数. 第一个参数(习惯上成为`argc`, 用于参数计数)的值表示运行程序时命令行参数的数目; 第二个参数(称为`argv`, 用于参数参数向量)是一个指向字符串数组的指针, 其中每一个字符串对应一个参数. 我们通常用多级指针来处理这些字符串.

最简单的例子是`echo`, 它将命令行参数回显在屏幕上的一行中, 其中命令行中命令行各参数之间用空格隔开.
```
echo hello, world
```
输出
```
hello, world
```
按照C语言的约定,`argv[0]`的值是启动启动该程序的程序名,因此`argc`的值至少为1. 上面这个例子`argc`值为3. 另外ANSI标准要求`argv[argc]`的值必须为一空指针.
```
argv:
     |___+----> +___+--->echo\0
                |___+--->hello,\0
                |___+--->world\0
                |_0_|
```
[echo 的第一个版本](https://github.com/chenboshuo/c_learning/blob/dc00385ad78bd1ebd5710239e7a861843fbcfb2c/the_c_programming_language/ch5_pointer_and_array/print_argv.c)将argv看成一个字符指针数组.
因为argv是一个指向char类型的指针,所以可以通过指针而非数组下标进行自增运算.
[echo 的第二个版本](https://github.com/chenboshuo/c_learning/commit/0fe34dd9ecacf5eb05fae043102a85db313fb4b3)在对argv进行自增运算,argc做自减运算实现的, argv是指向指针的指针.

也可以将print写成[这种形式](https://github.com/chenboshuo/c_learning/commit/c8f44b8c49c8cac2e18cc8ae1e46699aac4cb75e)
```cpp
printf((argc > 1)? "%s ": "%s", *++argv);
```
说明printf的格式化参数也可以是表达式.

我们来看第二个例子.在这个例子中, 我们将增强[4.1中](https://github.com/chenboshuo/c_learning/blob/87c0559dc668498a2850ad7eb28f46bb98270e56/the_c_programming_language/ch4_function_and_program_structure/print_lines_containing_strings.c)模式查找的功能.
下面我们仿照UNIX程序grep来改写查找程序,通过命令行的一个参数指定匹配模式.
-   [code](https://github.com/chenboshuo/c_learning/commit/4f19515fea6adec771c89b73d1c3081c80828047)
```cpp
strstr(line, argv[1]) != NULL
```

标准库函数`strstr(s,t)`返回一个指针,该指针指向字符串t在s中第一次出现的位置,如果没有出现s则返回NULL,该函数声明在`<string.h>`中

为了进一步解释指针结构, 我们来改进模式查找程序.
假定允许程序带两个参数, 其中一个表示"打印除匹配模式之外的所有行", 另一个参数表示"每个文本行前面加上相应行号"

UNIX系统中C语言有一个公共约定: 以负号开头的参数表示一个可选标志或参数. 假定用 `-x`(表示除...之外)打印所有与模式不匹配的文本行,用`-n`(代表行号),那么下面的命令

~~现在看出起名字的艺术了吧,原文里起名时find,多简洁~~

```
print_lines_containing_strings -x -n
```
将打印所有与模式不匹配的行, 并在每个打印行前面加上行号.

可选参数可以以任意次序出现, 同时, 程序的其余部分应该与命令行参数数目无关. 此外, 如果可选参数能够组合使用, 将会给使用者带来更大方便,如
```
print_lines_containing_strings -nx 模式
```
-   [代码](https://github.com/chenboshuo/c_learning/commit/4797e73db17caa374d3526549883867e4bb2fa59)

-   [还有个小错误](https://github.com/chenboshuo/c_learning/commit/0d2ce3453012d66299800abc47b74d3e209c3867)

## 5.11 指向函数的指针

在C语言中, 函数不是变量, 但可以定义指向函数的指针. 这种类型的指针可以被赋值, 存放在数组中, 传递给函数或者作为函数的返回值等等, 接下来修改本章之前的排序函数, 在给定可选参数`-n`的情况下,函数将按照数值大小而非字典顺序进行排序.

排序通常包括三部分: 判断任何对象之间次序的比较操作, 颠倒对象次序的交换操作, 一个用于比较和交换对象直到所有对象都按正确次序排列的排序算法. 由于排序算法与比较, 交换操作无关, 因此, 通过在排序算法中调用不同的比较和交换函数, 便可实现按照不同标准排序. 这就是我们新版本排序所采用的方法.

我们在前面讲过, 函数`strcmp` 按字典排序比较两个输入行. 在这里, 我们还需要一个以数值为基础来比较两个输入行, 并返回与`strcmp`一样结果的`numcmp`. 指向恰当函数的指针将被传给`qsort`函数. 在这里, 参数的出错处理并不是问题的重点.

-   [相关修改](https://github.com/chenboshuo/c_learning/commit/11cd8859a9c7ab90a650469ce25d6566662f69a1)

## 5.12 复杂声明

C语言常常因为声明的语法问题受到人们批评, 特别是涉及到函数指针的语法.
C语言的语法力图使声明和使用相一致.
对于简单的情况, C语言的做法是很有效的, 但是, 如果情况比较复杂, 则很容易让人混淆,
原因在于,C语言的声明不能从左至右阅读, 使用了太多圆括号, 我们来看下面所示的两个声明:
```cpp
int *f(); // f:是一个函数,它返回一个指向int型的指针
```
以及
```cpp
int (* pf)(); // pf是指向函数的指针,该函数返回一个int类型的对象
```
他们之间的差别说明:`*`是一个前缀运算符,优先级低于`()`, 所以声明中必须使用圆括号保证正确的结合顺序.

创建复杂声明有几个比较好的方法,
一种比较好的方法是,通过`typedef`通过简单的步骤合成.
这里介绍另一种方法,通过两个程序使用这种方法:
一个程序用于将正确的C语言声明转化为文字描述, 另一个程序完成相反的转换.
