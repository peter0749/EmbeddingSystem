Lab 7: Linux Exception

404410030 資工三 鄭光宇

### 實驗目的

學習如何使用 GDB server 遠端 debug

### 實驗步驟

使用跳線的網路線將 Pi 與 PC 直接連線

根據自己的網路環境，設定 Pi 上的
`/etc/network/interfaces` 檔案

回到 PC ，使用前幾個 Lab 編譯好的 Cross-Compiler (需要支援 C++) ，編譯 gdb server

進入 GDB tarball ，進行編譯

```
export CC=arm-linux-gnueabihf-gcc
export CXX=arm-linux-gnueabihf-g++
export CFLAGS=-static
export CXXFLAGS=-static
./configure --host=arm-linux-gnueabihf --target=arm-linux-gnueabihf
make -j2
```
會產生 gdbserver 執行檔，把他複製到 Pi 上就可以執行 gdbserver

寫一個測試程式

```
#include <stdio.h>

int main(void) {
	int i=0;
	int j=0;
	
	for (i=0; i<100; ++i) {
		j = j + 1;
	}
	
	printf("j = %d\n", j);
	return 0;
}
```

使用上述程式碼，用不同編譯設定，編譯出四個執行檔，並在 Pi 上測試、Debug。

```
arm-linux-gnueabihf-gcc -static test.c -o test_static.exe
arm-linux-gnueabihf-gcc test.c -o test.exe
arm-linux-gnueabihf-gcc -static -g test.c -o test_static_g.exe
arm-linux-gnueabihf-gcc -g test.c -o test_g.exe
```

上機測試

使用方法：

先在 Pi 上開啟 gdbserver

```
./gdbserver 169.254.210.125:8787 執行檔名稱
```

![](https://i.imgur.com/mwgf3Np.jpg)
![](https://i.imgur.com/KzrpbP8.jpg)
![](https://i.imgur.com/nvtSsv6.jpg)
![](https://i.imgur.com/fHoiP9f.jpg)
![](https://i.imgur.com/hQJRp2q.jpg)
![](https://i.imgur.com/6UPqOUb.jpg)
![](https://i.imgur.com/Ypxh3PZ.jpg)
![](https://i.imgur.com/vb5HGCq.jpg)
![](https://i.imgur.com/lZGKf7F.jpg)
![](https://i.imgur.com/hlb2R4W.jpg)
![](https://i.imgur.com/CExdDnq.png)
![](https://i.imgur.com/o54NWPl.png)
![](https://i.imgur.com/OIgdStr.png)
![](https://i.imgur.com/NW2VSM8.png)
![](https://i.imgur.com/V4phYJn.png)
![](https://i.imgur.com/pjkAHk2.png)
![](https://i.imgur.com/0TWAx4V.png)
![](https://i.imgur.com/s3nC50d.png)
![](https://i.imgur.com/8F4lv3g.png)


### Q&A

Q：請問 test.exe、test_g.exe、test_static.exe、 test_static_g.exe 在Pi上執行的結果如何? 它們之 間的差別是什麼?

A：沒有下 `-static` 參數的執行檔都無法執行（test.exe, test_g.exe），因為 `Pi` 上沒有相對應的 `shared library`。而沒有下 `-g` 的 `test_static.exe` 無法看到相對應程式碼片段，因為裡面沒有 symbol 的資訊。

能夠正常 debug 的檔案只有 `test_static_g.exe`。

之前有遇到 gdb 很奇怪認不到 cpu 型號的問題，後來參考網路上修改 gdb 原始碼的方式解決。

如果有如下錯誤：

```
Remote ‘g’ packet reply is too long: 
```

修改 `gdb_tarball/gdb/remote.c` 如下

```
/* 刪除這段 
if (buf_len > 2 * rsa->sizeof_g_packet)
error (_(“Remote ‘g’ packet reply is too long: %s”), rs->buf);
*/
/* 新增這段 */
if (buf_len > 2 * rsa->sizeof_g_packet) {
    rsa->sizeof_g_packet = buf_len ;
    for (i = 0; i < gdbarch_num_regs (gdbarch); i++) {
        if (rsa->regs->pnum == -1)
            continue;
        if (rsa->regs->offset >= rsa->sizeof_g_packet)
            rsa->regs->in_g_packet = 0;
        else  
            rsa->regs->in_g_packet = 1;
    }     
}
```

來源：[https://www.cnblogs.com/hustdc/p/8146117.html](https://www.cnblogs.com/hustdc/p/8146117.html)