# echash第二周

## 2021.10.17 亢恒越

**Title:** Coupling Decentralized Key-Value Stores with Erasure Coding

**Source:**socc19

**Authors:**Liangfeng Cheng,Yuchong Hu,Patrick P. C. Lee

## introduction

​		在decentralized kv存储系统中，使用consistent hashing来组织数据，并使用多副本容错，引入纠删码机制能够降低存储开销，但也带来两大挑战。

​		第一个挑战：在一个hash ring上扩容时，增加或删除节点会导致一些object移动到新的节点上，此时原来的条带会发生改变，需要重新计算校验块，引起较大开销。

![Screen Shot 2021-10-17 at 10.02.28](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 10.02.28.png)

​		第二个挑战：当进行扩容时，object进行移动，无法被直接读取，需要degraded read，使用纠删码解码出要读的object；而此时由于object在移动，可能无法完整地获取同一个stripe上k个chunks，导致无法解码。

![Screen Shot 2021-10-17 at 14.45.25](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 14.45.25.png)

​		论文分别使用fragmented chunks和multipe hash rings来解决这两个问题。

## Background

### consistent hashing

​		常见的hashing，使用hash函数算出object的位置，该函数通常与桶的数量有关，所以当扩容时，很多object的位置都要改变，开销较大。

![v2-52800890b024d1d4ea79390893eddacb_b](https://pic4.zhimg.com/v2-52800890b024d1d4ea79390893eddacb_b.jpg)

​		如图，4个object，模2分桶的结果。

![v2-b7ab0fdfa0c1bbc57606267b6321ac83_b](https://pic4.zhimg.com/v2-b7ab0fdfa0c1bbc57606267b6321ac83_b.jpg)

​		加桶1个后，所有object的位置都发生了改变。在存储系统中，扩容会经常发生，这样的开销是不可接受的。

​		为了减少object的移动，在逻辑上增加虚拟桶，此时计算hash函数时使用虚拟桶和实际桶的总数，当object被分到虚拟桶时，就向后查找最近的实际桶，放在其中。

![v2-1d6406cc600626658b334f809953285f_b](https://pic4.zhimg.com/v2-1d6406cc600626658b334f809953285f_b.jpg)

​		如图，模7分桶，0、1、2、4、5为虚拟桶，3、6为实际桶。

![v2-049489c502ed0d7470c1cb9fee9358b0_b](https://pic1.zhimg.com/v2-049489c502ed0d7470c1cb9fee9358b0_b.jpg)

​		增加一个桶后，只有在其后面的一个桶中的object可能发生位置变化，因为这个物理桶的增加，只影响一段区域，如图，加入4号桶，只影响(3,6]，使得这一区域中虚拟桶的object位置发生变化。这样的设计能够大大减少开销。

​		在这样的设计下，加桶时可以直接加，只影响一个桶中的object；减少桶时，只需要在该桶的下一个（物理）桶中提前备份要去掉桶中的内容即可。在图中加入去掉3号桶，则2、3会进入号桶。

![v2-121ee964ec9c42c93b89dbb94497fd04_b](https://pic1.zhimg.com/v2-121ee964ec9c42c93b89dbb94497fd04_b.jpg)

​		还有一个问题，如果要去掉6号桶，而其后面无桶，所以将6号桶与0号桶（虚拟桶）连接在一起，其后继桶便为3号桶。这样就形成一个hash ring。

![Screen Shot 2021-10-17 at 14.43.15](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 14.43.15.png)

​		如图为论文文中的模型，既有物理节点也有虚拟节点，按照顺时针方向，object会被放入距离其最近的一个节点。

### erasure coding

#### self-coding

​		将一个object分为多个小块，将每个小块作为数据块进行编码，将产生的n个块放在n个节点上。

#### cross-coding

​		将多个object联合在一起，作为一个有固定大小的data chunk，以此为数据块进行编码，也将n个chunk放在n个节点上。

后者的优点：

- 现实生活中的kv存储数据主要是以object的形式存在。
- 在consistent hashing中，可以直接获得object的位置，而不需要查阅metadata，更好地实现了“**去中心化**”。

### motivation

​		为了解决challenge1，引入sub-chunk，即允许同一个chunk中的object不在同一个节点上，这样的话就要在metadata中进行记录。而且即使进行扩容，data chunk在逻辑上内容也不会改变，所以不需要更新条带上的校验码。

![Screen Shot 2021-10-17 at 14.59.59](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 14.59.59.png)

​		解决challenge2，则使用多个hash rings，将` (n,k)`纠删码中的共n个chunks分别放在n个hash rings上，此时在一个hash ring上扩容，便只会影响条带中的一个chunk，可以保证k个hash ring上的k个chunks 完整。

![Screen Shot 2021-10-17 at 15.14.21](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 15.14.21.png)

### FragEC

![Screen Shot 2021-10-17 at 15.31.27](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 15.31.27.png)

#### object index

​	每个object对应一个64bit的bucket，其中保存的信息如下：

![Screen Shot 2021-10-17 at 15.33.35](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 15.33.35.png)

​		图中chunk index应为56-63bits，编码使用RS码。

#### chunk index

​		每个chunk和stripe都有唯一的32位ID，每个chunk对应一个OIRList，该list维护了chunk中objects的组织顺序以及通过其可获得object index，即使一个chunk中的object移动到其他的节点上，该表也不变，去除了data chunk和node之间的关系，从而实现了fragment；stripe ID对应记录该stripe上data和parity chunks的ID。

## ECHash

​		除了解决了前面所提到的两个挑战之外，论文还设计了fragment-repair，提高了节点修复的效率。

![Screen Shot 2021-10-17 at 15.39.39](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 15.39.39.png)

​		如图为系统的架构，多个client用于和用户进行交互，proxy负责返回用户请求的结果和分发object的工作，并存储metadata；每个hash ring中使用多个server的内存来存储object；database和proxy backup分别作为data和metadata的冗余备份。

​		buffer的数量与hash ring的数量相等并一一对应，写入数据时先写入buffer，当k个buffer都达到设定的data chunk大小时，分发到k个hash rings上；同时计算好` n-k`个parity chunks，分发到不同的n-k个hash ring上 。

### scaling

![Screen Shot 2021-10-17 at 15.50.32](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 15.50.32.png)

​		新加入节点之后，部分object发生迁移，object index中的部分数据也发生改变，但仍属于原来的data chunk，对应的chunk index不会发生改变，也就不需要更新校验码。

### degraded reads

![](/Users/khy/Desktop/Screen Shot 2021-10-17 at 15.53.15.png)

​		如图，加入新节点后，假设object a此时正在移动，而用户请求读取a，便可通过proxy中保存的metadata，找到解码需要的chunks并构建，利用纠删码解码出a。

​		ccmemcacched:无法解码，从database取，延迟大。

​		此处系统有一定的**限制**，即最多允许` n-k`个hash rings同时扩容，否则无法保证一个条带上的k个chunks在同一时间都完整，从而解码出需要降级读的object。当然，如果degraded read失败，可以从mysql中读取。

### node repair

​		因为允许sub-chunk的存在，所以一个chunk会分为多个片段存储在多个节点上，所以当一个节点损坏时，可能会造成chunk的损坏。

![Screen Shot 2021-10-17 at 16.03.01](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 16.03.01.png)

​		如图a3损坏后，根据纠删码的原理，使用整个data2、data3和parity来恢复出a3，开销较大。

![Screen Shot 2021-10-17 at 16.05.56](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-17 at 16.05.56.png)

​		在echash中实现了fragment-repair，当一个object损坏后，不需要使用其他chunk的整体来修复，只使用根据该object的offset和length，找到对应需要的objects来重构即可。a3对应着b2、b3、c3，再加上parity即可修复出a3。

## question

- degraded read类似于node repair，可以使用相同的方式来解码出object，而不是解码出整个chunk。
- node repair时echash比ccMemcached差，是由于parity没有允许fragment。允许fragment后，一个node fail，会涉及到多个stripe，修复也需要多个stripe上的objects；在同样的条件下，ccMemcached涉及的stripe会更少，但需要reconstruct整个chunk。
- 允许parity fragment后，为了维护consistency，two-phase commit protocol需要更多的servers与proxy进行communication，有更大的开销。

## future work

- 论文中，将一个parity认为是一个object，不可被分割为fragmets，所以需要获取整个parity。若将parity也看作一个chunk，则修复时parity也可以只取其中的fragments，进而提高性能。
- degraded writes：如果预先确定的server失效，无法写入，可以重定向到另一个sever临时储存，并保留对应的metadata，恢复后再迁移。
- consistency：在纠删码更新方面，有一个data chunk更新，就要更新所有的parity chunks，使用two-phase commit protocol来保证。server中有一块buffer，用于临时存储parity chunk，当所有的servers都告知proxy存储正确，proxy才允许所有的servers更新parity，否则就丢弃这些parity。

## next week

​		本周已经配置好环境并跑通了代码，下周计划阅读代码，明确更多论文中的细节。

## 参考文献

Coupling Decentralized Key-Value Stores with Erasure Coding

[聊聊一致性哈希](https://zhuanlan.zhihu.com/p/24440059)



