# echash第二周2

**Title:** **DistCache: Provable Load Balancing for Large-Scale Storage Systems with Distributed Caching**

**Source:** **SOCC'19**

**Authors:** Liangfeng Cheng,Yuchong Hu,Patrick P. C. Lee

---

**Summary**

- 论文解决的问题：

  在decentralized kv存储系统中，使用consistent hashing来组织数据，并使用多副本容错，引入纠删码机制能够降低存储开销，但也带来两大挑战。

  第一个挑战：在一个hash ring上扩容时，增加或删除节点会导致一些object移动到新的节点上，此时原来的条带会发生改变，需要重新计算校验块，引起较大开销。

  第二个挑战：当进行扩容时，object进行移动，无法被直接读取，需要degraded read，使用纠删码解码出要读的object；而此时由于object在移动，可能无法完整地获取同一个stripe上k个chunks，导致无法解码。

  节点修复：在解决上述两个问题时，允许fragment-chunk，一个chunk会分为多个片段存储在多个节点上，所以当一个节点损坏时，可能会造成多个chunk的损坏，但也仅仅是每个chunk中的一小部分。此时如果通过解码出整个chunk来恢复object，开销较大。

  

- 解决问题的方法：

  为了解决challenge1，引入sub-chunk，即允许同一个chunk中的object不在同一个节点上，这样的话就要在metadata中进行记录。而且即使进行扩容，data chunk在逻辑上内容也不会改变，所以不需要更新条带上的校验码。

  解决challenge2，则使用多个hash rings，将` (n,k)`纠删码中的共n个chunks分别放在n个hash rings上，此时在一个hash ring上扩容，便只会影响条带中的一个chunk，可以保证k个hash ring上的k个chunks 完整。

  fragment-repair：当一个object损坏后，不需要使用同一个stripe中需要的整个chunk来修复，根据该object的offset和length，找到对应需要的objects来重构即可，论文中认为parity是一个object，不进行分割。

- **实验**

  分别在local和cloud进行实验，比较了Vanilla（不使用纠删码）、ccMemcached（使用纠删码，single hash ring）、echash在不同读写比例下的throughput和latency。由于Vanilla不需要计算parity，性能会更好，但差距较小。
  
  replication有很大的存储冗余，在文章解决的几个问题上，只比较了ccMemcached和echash。scaling、degraded read中后者比前者性能好很多，scale-in不像scale-out那么好。node pair比前者差一些。

---

**Strengthens**  

- 允许fragment chunk，使得一个chunk可以存储在多个节点上，不仅在扩容后不需要进行parity update，也破除了chunk和node的一一对应关系。
- 将一个stripe上的n个chunks分布在n个hash ring上，减少了scaling对stripe的影响，很大程度上增加了degraded read的成功率。

---

**Weaknesses**  

- 将parity视为object，无法分开，影响了node repair的性能。
