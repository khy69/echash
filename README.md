# ECHash

This is the source code for ECHash described in our paper presented in ACM SoCC'19. 
ECHash is tested on Ubuntu 14.04 and Red Hat Release 6.5 with GCC (version 4.8.4 - 4.9.2), and we take the Debian and Ubuntu environment as an example.


Preparation
----

These are the required libraries that users need to download separately.
Users can use apt-get to install the required libraries.

 - make & automake-1.14
 - yasm & nasm
 - libtool
 - boost libraries (libboost-all-dev)
 - libevent (libevent-dev)
`$ sudo apt-get install gcc g++ make cmake autogen autoconf automake yasm nasm libtool libboost-all-dev libevent-dev`

Users can install the following library manually: **Intel®-storage-acceleration-library (ISA-l)**.

    $ tar -zxvf isa-l-2.14.0.tar.gz
    $ cd isa-l-2.14.0
    $ sh autogen.sh
    $ ./configure; make; sudo make install

Users can use compile and run "*ec*" to confirm that ISA-l is successfully installed.

	$ gcc ec.cpp -o ec -lisal
	$./ec 1 2

## ECHash Installation

### **Memcached Servers**

Users can use apt-get to install Memcached servers, or by the source code of Memcached from *http://memcached.org*.

    `$ sudo apt-get install memcached`

-	For standalone setup, users can start and configure the initialization parameters (e.g. `-d -m`) of Memcached servers by `$ sh cls.sh` manually. Also users can configure **Init/Scale/Repair** Memcached IP/Ports in "*config.txt*" for testing.
-	For distributed setup, users can configure distinguished IP/Ports in each node.

### **ECHash Proxy**

	$ cd libmemcached-1.0.18
	$ sh configure; make; sudo make install
	$ export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH  #(if it ocuurs library path issue)


Workloads
----

Users can use YCSB to generate workloads **after executing "*ycsb_gen.sh*" and "*gen_workloads.cpp*"**.
For simplicity, users can also use the providing "*ycsb_set.txt*" and "*ycsb_test.txt*" as workloads (**8MB for READ ONLY**) to run ECHash and skip this workloads section.

1.Install *Maven-3.1.1* and configure the environment, and check the version by `mvn -version`.

	$ sudo tar -zxvf apache-maven-3.1.1-bin.tar.gz -C /usr/local
	$ cd /usr/local
	$ sudo ln -s  apache-maven-3.1.1 maven
	$ sudo vim /etc/profile.d/maven.sh
		#Add the following to maven.sh
			export M2_HOME=/usr/local/maven
			export PATH=${M2_HOME}/bin:${PATH}
	$ source /etc/profile.d/maven.sh

2.Install *java 8 update 151* and configure the environment, and check the version by `java -version`.

    $ sudo mkdir /usr/local/java
    $ sudo cp jdk-8u151-linux-x64.tar.gz /usr/local/java
    $ cd /usr/local/java
    $ sudo tar zxvf jdk-8u151-linux-x64.tar.gz
    $ sudo vim ~/.bashrc
    	#Add the following to .bashrc
    		export JAVA_HOME=/usr/local/java/jdk1.8.0_151 
    		export JRE_HOME=${JAVA_HOME}/jre  
    		export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib  
    		export PATH=${JAVA_HOME}/bin:$PATH
    $ source ~/.bashrc

3.Install *YCSB* (https://github.com/brianfrankcooper/YCSB.git), and build the Memcached binding.

	$ unzip YCSB-master.zip
	$ cd YCSB-master
	$ mvn -pl com.yahoo.ycsb:memcached-binding -am clean package

4.Use the **basic** parameter in "*workloads_test*" to generate the raw workloads.

-	Write/Read ratios (e.g. readproportion=1.0).
-	The rough value length (e.g. fieldlength=256)
-	Operation times (e.g. operationcount=32768)


	copy "workload_test" to YCSB-master/workloads
	copy "ycsb_gen.sh" to YCSB-master`

Configure `YCSB_HOME=` (path to YCSB-master) in "*ycsb_gen.sh*" firstly and then run the script to generate the workloads.

	$ sh ycsb_gen.sh

5.Pre-treatment workloads, execute "*gen_workloads*" to generate "*ycsb_set.txt*" and "*ycsb_test.txt*". The **value length of objects can be configured in "*gen_workloads.cpp*"** (`#define LENGTH 256`), and the **key size of objects is configured around 20B.** 

	copy "ycsb_load.load" and "ycsb_run.run" from YCSB-master to the directory of "gen_workloads.cpp"
	$ gcc gen_workloads.cpp -o gen_workloads
	$ ./gen_workloads


Benchmarks
----

-	Users can run *Basic I/O* with multiple threads (`#define NTHREADS 16` in "*io.cpp*"), but other benchmarks with **single thread currently** (`#define NTHREADS 1` in "*scale.cpp*" and "*repair.cpp*").
-	Function Prototype: `void MRmemcached_init_addserver(ECHash pointer, Memcached server IP, Port, Ring ID);` 
-	Users can change the directory of workloads ("*ycsb_set.txt*" and "*ycsb_test.txt*") in "*para.txt*" by configuring `Workloads Path=` (path to workloads).
-	Users can **configure (N,K) coding in "*libmemcached-1.0.18/libmemcached-1.0/struct/ring.h*"** (e.g. (5,3) coding as default) and keep **N=RING_SIZE**, where N = the number of data and parity chunks, K = numbers of data chunks. Besides, users should make sure that **the number of rings** of the number memcached servers **initialized in "*cls.sh*"** is consistent with the RING_SIZE of "*ring.h*".
-	Users can accelerate testing process by redirecting progress tracking printings to a temporary file, e.g., by "> tmp.txt".
-	Users can **reset Memcached servers** by `sh cls.sh` before each testing.


Compile all source files.

	$ make

1.**Basic I/O Performance** 

    $ ./io > tmp.txt

2.**Scale-out/Scale-in Performance**, users should make sure that scale-in memcached servers are already in this hash ring.
./scale [out|in] S, S indicates the number of scale out/in nodes.

	$ ./scale out S > tmp.txt # or ./scale in S > tmp.txt

3.**Degraded Read Performance During Scaling**, users can install mysqlclient for C++ firstly, create MySQL database and tables, and put the workloads into MySQL before running ECHash.

4.**Node Repair Performance**, users should make sure that the repaired memcached servers are already in this hash ring. Note that we only simulate the repair process including degraded reads and objects migration.

	$ ./repair > tmp.txt

## Publication

Liangfeng Cheng, Yuchong Hu, and Patrick P. C. Lee.
**"Coupling Decentralized Key-Value Stores with Erasure Coding."**
Proceedings of the ACM Symposium on Cloud Computing 2019 (SoCC 2019), Santa Cruz, CA, USA, November 2019.
(AR: 39/157 = 24.8%)

## Contact

Please email to Yuchong Hu ([yuchonghu@hust.edu.cn](mailto:yuchonghu@hust.edu.cn)) if you have any questions.

## Our other works

Welcome to follow our other works!

1. FAST 2021: https://github.com/YuchongHu/ecwide
2. ICDCS 2021: https://github.com/YuchongHu/stripe-merge
3. SoCC 2019: https://github.com/YuchongHu/echash
4. INFOCOM 2018: https://github.com/YuchongHu/ncscale
5. TOS: https://github.com/YuchongHu/doubler

## io logs

queries_init...done

start benching using thread12
start benching using thread8
start benching using thread9
start benching using thread7
start benching using thread3
start benching using thread11
start benching using thread0
start benching using thread15
start benching using thread10
start benching using thread13
start benching using thread6
start benching using thread1
start benching using thread5
start benching using thread4
start benching using thread2
start benching using thread14

thread11 gets 65536 items in 8.03 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8164.11, tset=  2691.06,tinsert=  -nan, tget=  4091.78, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread7 gets 65536 items in 8.02 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8173.79, tset=  2677.48,tinsert=  -nan, tget=  4096.72, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread9 gets 65536 items in 8.08 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8107.64, tset=  2676.99,tinsert=  -nan, tget=  4067.79, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread14 gets 65536 items in 8.04 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8151.03, tset=  2742.49,tinsert=  -nan, tget=  4085.32, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread8 gets 65536 items in 8.20 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7996.47, tset=  2670.10,tinsert=  -nan, tget=  4010.71, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread10 gets 65536 items in 8.11 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8084.83, tset=  2674.12,tinsert=  -nan, tget=  4052.78, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread12 gets 65536 items in 8.27 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7923.19, tset=  2669.00,tinsert=  -nan, tget=  3971.68, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread13 gets 65536 items in 8.12 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 8068.41, tset=  2681.35,tinsert=  -nan, tget=  4043.86, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread3 gets 65536 items in 8.26 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7931.33, tset=  2667.35,tinsert=  -nan, tget=  3975.49, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread6 gets 65536 items in 8.23 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7965.17, tset=  2695.04,tinsert=  -nan, tget=  3991.93, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread2 gets 65536 items in 8.24 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7957.89, tset=  2722.95,tinsert=  -nan, tget=  3989.81, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec
queries_exec...done

queries_exec...done

queries_exec...done

queries_exec...done


thread15 gets 65536 items in 8.49 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7717.36, tset=  2682.78,tinsert=  -nan, tget=  3867.76, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec
queries_exec...done

queries_exec...done

queries_exec...done

queries_exec...done


thread0 gets 65536 items in 8.49 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7718.76, tset=  2638.10,tinsert=  -nan, tget=  3870.20, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread4 gets 65536 items in 8.44 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7764.11, tset=  2683.90,tinsert=  -nan, tget=  3891.90, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec
queries_exec...done

queries_exec...done

queries_exec...done


thread5 gets 65536 items in 8.49 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7714.94, tset=  2670.04,tinsert=  -nan, tget=  3867.85, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec

thread1 gets 65536 items in 8.70 sec 
#set = 32768, #insert = 0, #get = 32768, #update =0 
#miss = 0, #hits = 32768
hitratio = 1.0000
throughput = 7532.20, tset=  2704.32,tinsert=  -nan, tget=  3774.95, tupdate= -nan
Get_once=32768,Dget=0,Dget success=0,Dget failed=0,Dget not encode=0, time_dget=0.00 sec
queries_exec...done

queries_exec...done

queries_exec...done

queries_exec...done

queries_exec...done


total_sets = 524288
total_inserts = 0
total_gets = 524288
total_updates = 0
total_time = 8.70 sec
latency set = 372.58, latency insert= -nan,latency get= 251.52
total_throughput = 126971.24, tsets= 42947.08,tinserts= -nan,tgets= 63650.53,tupdates= -nan
total_hitratio = 1.0000

over