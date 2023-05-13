# README

## 迭代1

本来不想写readme的，只因一开始的构思就是写一个带block的vector模拟队列

但是发现双端队列在前面插入变成O(n)了

但是发现好像可以变成O(1)

但是发现这样就需要多一点构思

所以就写了个readme

现在为了便于pushfront

本来的构思是二维数组

## 迭代2

现在把arr[0]作为一个缓冲，头就放在这个里面

push_front从前往后一次是O($\sqrt n$),因为要把后面的往后移

放满了之后，单次O(n),但平摊下来还是O($\sqrt n$)

但是怎么O(1)呢？从下往上插入，每次插入是O(1)

我可以记录每一个block是正插还是反插

那么放满了之后，我只要改变$\sqrt n$个指针的值，平摊下来也是O(1)

结构复杂导致的是读取上变得困难

每个block有一些自己的量，如正反和个数

使得读取头和尾很简单

随机访问的时候，通过一些计算，应该也可以O（1）

具体的，因为这个结构有三个部分，头，尾，身体

身体是满的

那么

1：判断在不在头里面

2：判断在不在身体里面，如果在，那么减去头的大小，先定位块，再定位具体元素

3：在尾，定位

---

## 迭代3

但是发现这样的话,insert 还是O（n），不符合分块的思想：平衡所有操作，所以还是要让insert变成O（$\sqrt{n}$,那么随机访问只能O($\sqrt{n}$)

如果不insert，其实和刚才一样

但是要insert，就把满列后移，在空列上加

但这样可以以$\sqrt{n}$次操作花费O(n)

但是如果在列中间加，就先$\sqrt{n}$把它插入这个满列，然后多出的一个插入下一列开头

也就是这次加上了每一列的个数不同

---

注意到还可以维护一个前缀和数组，这样可以用二分查找的方法快速做一个加减，实现$log\sqrt{n}=logn$时间完成一个迭代器加减,虽然比
原本的O（1）慢一点，不过也很接近了

而且这个二分查找大概率还能重用，在随机访问时可以快速定位到在哪一块，然后可以O(logn)的时间完成随机访问

需要在每次插入后重做前缀和，也就是使得在头部插入变成O($\sqrt{n}$)，不符合要求

---

## 迭代4

但是注意到在结尾维护前缀和是O（1）

并且由于只有头和尾插入，考虑维护两个数组，一个是从头插进来的，一个是从尾插进来的，这样类型就一样，我命名为U形数组，真正做到双端（之前好像已经有类似的做法了）

随机插入的时候虽然也要维护前缀和，但是符合$\sqrt{n}$

**注**：为什么不能O（1）随机访问，因为每个block中元素个仍然不同，只因有insert操作

也就是现在具有的数据有：

~~~
deque{
    ** arr_tail,arr_head
    * size_tail,size_head,sum_tail,sum_head
    size_q,tag,
    block_size_head,block_size_tail,
    head_block,tail_block
}
iterator{
    deque;
    cur
}
~~~

由于数据复杂性，iterator难以之间指向具体数据，于是iterator完全只是一个下标的封装，反而更加简单

---

进度：还需要完成：insert，erase，pop,两个队列之间转换，常迭代器

---

finish

---

2023.5.13update to github

文件说明：

项目提供了deque.hpp的框架，要求实现功能

无pdf需求文件

只有根目录下deque.hpp为自己手写

测试说明

test下每个文件夹有code.cpp

把code.cpp复制到根目录后编译运行code.cpp即可

其余为项目提供的依赖文件或测试内存泄漏工具等