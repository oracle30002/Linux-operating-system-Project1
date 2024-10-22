#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/pgtable.h>


SYSCALL_DEFINE1(get_my_physical_addr, void *, virt_addr)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn_addr = 0;
	unsigned long virt_offset = 0;
	unsigned long phy_addr = 0;
	unsigned long vaddr = (unsigned long)virt_addr;

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
