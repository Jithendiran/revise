Allocation of DMA buffer at low level

Device which transfer data using ISA or PCI system bus uses physical address, so whenever data is bigger than one page that must occupy continuous page. Some Sbus uses virtual address but for portability we can't count that

DMA buffer can allocate at run time or boot time. Driver should take care of suitable memory allocation for DMA, not all zone are suitable(Some system's high memory may not work for DMA)

Most modern devices can handle 32-bit address, means that normal memory allocation works fine for them. Some PCI devices, however cannot to work with 32 bit address and ISA devices are limited to 24-bit

For device with that kind of limitation, memory should be allocated in DMA zone. It is done by adding flag `GFP_DMA` to `kmalloc` or `get_free_pages`. When this flag is present only memory can addressed with 24-bit. Alternatively, generic DMA layer is user to allocate buffer