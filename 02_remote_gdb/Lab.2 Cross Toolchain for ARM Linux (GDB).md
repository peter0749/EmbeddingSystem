學號：404410030

姓名：鄭光宇

Email：jengku@gmail.com

### Lab.2 Cross Toolchain for ARM Linux (GDB)

#### 實驗目的：

藉由實際從原始碼編譯一次 gdb ，了解 gdb 的編譯步驟，讓我們之後如果要修改、加入 gdb 程式碼時，可以更有相關經驗、概念。並利用這次實驗練習 remote debugging ，了解 remote debugging 的流程與基本原理。

#### 實驗步驟：

實驗分成兩大步驟：編譯 gdb 、使用 gdb, gdbserver 實現 remote debugging。

接續 Lab.1 ，我們要將 gdb 編譯進上次我們在 Lab.1 編譯好的 toolchains ，詳細步驟如下：

我們寫一個 bash script ，下載原始碼、編譯 gdb 到 toolchains 中，每個步驟都有附上註解。

```bash
#!/bin/bash

sudo apt-get install texinfo libncurses5-dev libx11-dev # 安裝相關套件

### 下載、解壓 gdb
wget ftp://ftp.gnu.org/gnu/gdb/gdb-8.1.tar.gz
tar zxvf gdb-8.1.tar.gz

### 建立用來編譯 gdb 的臨時目錄
rm -rf build_gdb # 清除舊的臨時目錄
mkdir build_gdb
cd build_gdb

### 在臨時目錄中，編譯、設定 gdb ，並指定 target 名稱，與安裝路徑
### 安裝路徑可以利用執行這份 bash script 時，以環境變數 TOOL_CHAIN_PATH 傳入
### 例如： TOOL_CHAIN_PATH=/home/peter/toolchains ./install_gdb.sh
../gdb-8.1/configure --prefix=${TOOL_CHAIN_PATH} \
                     --target="arm-linux-gnueabihf" \
                     --enable-tui

### 以五個 jobs 加速 gdb 編譯
make -j5
### 安裝到設定 gdb 時指定的 prefix
make install
```
編譯好 gdb 後，我們先在 raspberry pi 上，安裝 gdbserver ，用來完成 remote debugging：

```
sudo apt-get install gdbserver
```

之後，我們將 raspberry pi 的網域設定到與 host 相同網域，之後輸入 `ifconfig` 指令查詢 raspberry pi 的 IP。

![](https://i.imgur.com/9nRglgW.png)

將 IP 抄下來。

我們在 host 上編譯實驗要用的 test.exe：

![](https://i.imgur.com/626Wv5y.png)

在 raspberry pi (target) 上開啟 gdbserver，IP 為之前 ifconfig 上看到的那一組，port 隨意指定，指定要執行 `test.exe` ，並記住這個 port 號。被除錯的 test.exe 會停在程式剛執行的地方：

![](https://i.imgur.com/c08cLvO.png)

在 host 端，開啟 gdb (用 `-tui` 模式開啟)：

![](https://i.imgur.com/7P7uRU4.png)
![](https://i.imgur.com/zl359XV.jpg)

輸入 `b 10` ，在第十行設定斷點 (breakpoint)：

![](https://i.imgur.com/CbEKiz3.jpg)

輸入 `target remote 你的IP:你的port`，開始 remote debug：

![](https://i.imgur.com/G90CcQN.png)

輸入 `continue`，遠端的 `test.exe` 會從 gdbserver 幫我們自動設的斷點繼續執行到我們設定的斷點 (第十行)：

![](https://i.imgur.com/7K0b47K.jpg)

接下來就可以練習 remote debugging 了。

試著使用 step：
![](https://i.imgur.com/LL9xS6B.png)
![](https://i.imgur.com/TNC6SXo.png)

其他，用 gdb 印出記憶體、register 等資訊：

![](https://i.imgur.com/ClVIqwC.png)
![](https://i.imgur.com/CYSNXh0.png)

#### 問題與討論：

*What is “GDB server”?*

一個用來做遠端除錯的程式，我們會在 target 端開啟 gdbserver ，去控制 target 上要被除錯的程式，透過 TCP 或者 Serial Port 與 host 溝通。使用 gdbserver 時， target 端不需要實際安裝 gdb，而且 gdbserver 遠比 gdb 小，因此可以快速地 porting，很快達成遠端 debug。使用 gdbserver 時，我們需要一份已經編譯好的可執行檔在 target 端上，而 host 端需要同一份可執行檔，與它的 source code。

*What is “GDB stub”?*

GDB stub 是一個用來監測、控制要被 debug 的程式的程式，會和要被 debug 的程式 link 在一起，藉由接收 Remote Serial Protocol 與 host 通訊，將 host 的指令，利用 gdb stub 去控制要被除錯的程式。