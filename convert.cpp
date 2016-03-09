#include <cstdio>
#include <cstdlib>

#define MAXMEM 4096

unsigned char mem[MAXMEM];

int main(int argc, char** argv){

	FILE* fr = fopen("mem.txt", "r");
	int i, j;
	fscanf(fr, "page %x:", &i);

	while (true)
	{
		for (j = 0 ; j < 32 ; j++)
		{
			int _mem = 0;
			fscanf(fr, "%x", &_mem);
			mem[(i << 5) + j] = (char)(_mem);
		}
		if (fscanf(fr, "\npage %x:", &i) == EOF)
			break;
	}

	for (i = 1 ; i < argc ; i++)
	{
		unsigned char* pde = mem + 544;
		int vaddr = 0;
		sscanf(argv[i], "%x", &vaddr);

		if (((unsigned int)(pde[vaddr >> 10]) << 4) == 0x7f0)
		{
			printf("pde invalid!!\n");
			continue;
		}

		printf("pte_index = %02x pte_addr = %03x\n",
			vaddr >> 10, (unsigned int)(pde[vaddr >> 10]) << 4);
		unsigned char* pte = mem + ((unsigned int)(pde[vaddr >> 10]) << 4);

		if (((unsigned int)(pte[(vaddr >> 5) & 31]) << 4) == 0x7f0)
		{
			printf("pte invalid!!\n");
			continue;
		}

		printf("page_index = %02x page_addr = %03x\n",
			(vaddr>>5) & 31, (unsigned int)(pte[(vaddr >> 5) & 31]) << 4);
		unsigned char* page = mem + ((unsigned int)(pte[(vaddr >> 5) & 31]) << 4);
		printf("addr=0x%08x value=0x%02x\n", vaddr, page[vaddr & 31]);
	}

}

