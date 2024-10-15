# <font color="F7A004">**FALL2024 Linux Operating System Project1**</font>
此為NCU 2024年 Linux Opreating system之專案作業。
課程之目的是教導讀者在Linux系統中建立自定義之System call，其作用是將傳入的 virtual address 轉換為 physical address 後回傳。
需有Liunx PGtable及作業系統的相關背景知識才叫好理解。

## Members
113522004 余聖言




## 題目
[Project1](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html)


## Reference
1. [linux内核那些事之struct page](https://www.bilibili.com/read/cv19509580/)
2. ChatGPT(幫助程式潤飾及知識科普)
3. [Linux 核心設計: 記憶體管理](https://hackmd.io/@sysprog/linux-memory?type=view)
4.
5.
6.
7.

## Enviroment
Linux kernel version: 5.15.137
Distribution: Ubuntu 22.04.3
gcc: 11.4.0
gdb: 12.1

## Kernel Space Code (New System Call)
### my_get_physical_addresses.c

```
config PGTABLE_LEVELS
	int
	default 5 if X86_5LEVEL
	default 4 if X86_64
	default 3 if X86_PAE
	default 2
```
這段程式碼是 Linux 核心的配置選項之一，與PGTABLE_LEVELS有關。PGtable level決定了虛擬記憶體地址映射到實體記憶體地址的層數。根據不同的架構，頁表層級數可能不同。在 X86_64 架構下，如果 沒有啟用 PAE (Physical Address Extension)，頁表層級應該是五層，分別是：

1. PGD (Page Global Directory)
2. P4D (Page 4th-level Directory)
3. PUD (Page Upper Directory)
4. PMD (Page Middle Directory)
5. PTE (Page Table Entry)
![123](https://hackmd.io/_uploads/r13Z6oikye.png)



---

[https://elixir.bootlin.com/linux/v5.15.137/source/arch/x86/include/asm/pgtable.h](https://)
pgd_offset(), p4d_offset() 等 functions 定義在上述 header file，是 Linux 核心用來遍歷Page table level的函數。通過這些函數，結合當前Process的 mm->pgd（頁表的起始位置），可以查找某個虛擬地址是否已經被分配對應的實體頁框（page frame）。

#### my_get_physical_addresses.c 程式碼
```cpp=
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/pgtable.h>


SYSCALL_DEFINE1(my_get_physical_addresses, void *, virt_addr)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn_addr = 0;
	unsigned long virt_offset = 0;
	unsigned long phy_addr = 0;   //存儲最終計算出的實體位址
	unsigned long vaddr = (unsigned long)virt_addr; //將傳入的虛擬地址 )virt_addr 轉換為無號整數型態

	pgd = pgd_offset(current->mm, vaddr);
	if (pgd_none(*pgd))
		return 0;
	
	p4d = p4d_offset(pgd, vaddr);
	if (p4d_none(*p4d))
		return 0;

	pud = pud_offset(p4d, vaddr);
	if (pud_none(*pud))
		return 0;

	pmd = pmd_offset(pud, vaddr);
	if (pmd_none(*pmd))
		return 0;

	pte = pte_offset_kernel(pmd, vaddr);
	if (pte_none(*pte))
		return 0;

	pfn_addr = pte_val(*pte) & PTE_PFN_MASK & PAGE_MASK;
	virt_offset = vaddr & ~PAGE_MASK;
	phy_addr = pfn_addr | virt_offset;
	
	
	return phy_addr;
}
```



## 新增 system call 與編譯核心過程
[about how to add a system call](https://hackmd.io/aist49C9R46-vaBIlP3LDA?view)
[Adding A System Call To The Linux Kernel](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)





## User Space Code
下面為原始程式碼:
[Link to question1.c](https://github.com/oracle30002/Linux-operating-system-Project1/blob/main/Project1/question1.c)
[Link to question2.c](https://github.com/oracle30002/Linux-operating-system-Project1/blob/main/Project1/question2.c)




## 編譯及運行
完成Kernel編譯後，重啟系統並使用新Kernel。並對question進行編譯及運行。
#### 編譯question程式:
`gcc -o question1 question1.c`

#### 運行question程式
`./question1`




## Ouput of ./question1








