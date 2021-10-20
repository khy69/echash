# echash project

## preorities

知网：通过学位文章前一部分学习基础知识

书：只学习前两章

memcashed：通过学位文章前一部分学习基础知识

nosql

ppt **show your english level**

**不要影响到平时学习**

从项目入手，提高系统工程能力，到达一定程度（疑问、看到差不多了）->去华科（确定一个学长带我）

之后再说算法优化的事

## words

Parity: 校验码

stripe:条带

Chunk:a set of objects

throughput=1/latency

metadata：a description of data(object)



![](/Users/khy/Desktop/Screen Shot 2021-10-09 at 10.54.11.png)

## goal

根据文章理解（重构）代码细节，找到疑问，尽量找可能改进的点

## question

Ask **some characters** about graduate recommendation

Yes

## erasure code

### mds code

![Screen Shot 2021-10-04 at 08.18.33](/Users/khy/Library/Application Support/typora-user-images/Screen Shot 2021-10-04 at 08.18.33.png)

### Optimization

internet

disk

### tasks

degrader read:customer request from some node which is in crash and have to get from **other nodes** including decoding

keep on file:convert multiple replicas to erasure code

## first week

呵呵 不回我 确实干的有点少了

## Second week

strip:a set of data and parity disk

Method:add too much band width?

find some points to enhance integerating with experiments!

Key point:implement of **FragEC**

Echash:写入慢、scale-in优势不明显

## Third week

### memcached

no **fault-tolerant**!

probability of mapping object to node is **equal**

#### memory application

Slab->several chunks

### Method to read thesis

- Force your brain to scan quickly(understand firstly!)
- highlight less but more useful

### TODO:memcached

how to read?i'm more on details?



## how to read a thesis

### title        1                                           every sentence

### abstract  1                                            repeat

### intro                          citied article        if i were him

### method    excel         high level             how to do/subsequent

### exp            graphic.    high level 			citied article		

### present

### conclusion 1

### second time

- read paragraph by paragraph
- Conbination->engineering->system detail, creation->research->method?
- find what you are **fuzzy**
- experimental part->result(detail-> repeat?)

## how to perform well as a advisee?

- easy to **communicate with(clear own (dis)adventages)**,**expose yourself more** to your advisor!

- Try **hard** and have **enthusiasm** on project

- try to **solve problem(not push after process)** and **ask help from teacher in time(dont struggle with yourself)**,as early as possible!!!!

- never be afraid to make mistakes, **talk as much as possible**

- ability to **summary\ppt\talking(strong and easy)**

- **english/programming/responsible**

- clear **question/goal/arrange/opinion/target of project**

- **Report process initiatively** about your question and result

- understand **work of teacher**

- show you will to **study as a phd sometime**

- Be more **active\positive** to **contact with teacher**

- never **lie**

- 1.你不能把他当老师。2.不能把自己当学生

- be **solid\self-motivated(seek fame!)**

- **programme your career!**

- ask **specific questions(leave some options for him)!**

- 交流时一定要先把**context（背景信息）**准确无误地表达清楚。比如要交代清楚一个算法，首先要讲这个算法要**解决什么问题**，这个算法的**输入输出是什么**，这个**算法的框架**是什么。然后再讲算法的细节是什么（这部分可能导师已经没功夫听了，可能听了也记不住）。失败的交流方式是讲了几十分钟算法的细节（比如某个变量的改动，某个公式的推导，或者对某些数据的操作），而没有交代清楚必要的context（为什么要做这件事？已知信息是什么？要得到什么？）

- 每个公式的**变量**什么含义都讲清楚

- 找个**靠谱的学长学姐**带很重要

- **不一定所有的实验结果都要一下子汇报完。结果好的时候可以适当留一些**，避免下周遇到了困难，没结果可以汇报。这个trick有很多时候都很有用，可以平滑项目的研究过程.

- 培养**critical thinking**很重要，有很多美国厉害的教授都重视这个。怎么培养？可以从正反面思考开始做起。在面对一篇文章的结论时，要经常考虑**他在什么情况下不成立**。

- 对使用的**每个工具**都要非常非常熟练，精益求精。比如编程时经常需要用到各种库，可能其他专业需要用到各种软件。我认识的一些工程师对每个库的来龙去脉，底层实现都非常清晰，这样遇到困难时才能很快确定问题。刨根究底是一项非常重要的工程技术

- **Always be specific**。比如发言的时候一定要**带上条件**，在XX的情况下，对XX数据的用XX手段观察到了XX现象，而不是泛泛而谈结论。这点在小组讨论和写论文的时候都有用。

- 做一手好PPT也很加分。有人总结的好，好的PPT是**字少且大**。在保证两个前提的情况下，字越少越好。前提1: PPT是**self-contained**，不需要讲解人们就能明白这页PPT在讲啥。前提2: PPT**页与页之间的关系非常清晰，逻辑清楚**。

- **会哭的孩子有奶吃**

- **Claim my needing**! 

- 答辩或面试最容易犯的三个大问题：1) 答非所问，抓不到问题核心重点和提问人目的，经常自说自话；2) 回避问题，有意无意的漏掉或躲闪一些问题，尤其是问题成块的时候；3) 作茧自缚，引入毫无证据或第三方来源的口水话来侧面证实，导致问题回答后漏洞越来越多，被提问者抓住小辫子往死里打。

- 前者踏踏实实，后者锲而不舍；比能力更重要的是，是每个人的动力

- 因随波逐浪而做出的局部最优的选择，也许多年以后回头看正是歧路的开始。而有些道路在错失机会后，是没可能再踏上的。因为时机不再，路径也发生了天翻地覆的变化。

  并不是条条大路通罗马，进不了风口的猪也只能回猪圈而已。

- 把什么都当作一根根救命稻草，卑微地守望着眼前的麦田，最后却收获了一条黄金之路，浩浩荡荡，向前望好像通到了天上，回头看每步都是一颗微小的稻草而已。

- 有不懂得会不会**主动问**老师？

- weekly report->学术写作能力

## thesis

Question: apply erasure coding to decentralized key-value stores(dereplication, lower cost),but some disadvantages:

- Scaling, parity updates->fragmented erasure coding
- degraded reads->multiple hash rings 

### Disadvantage?

