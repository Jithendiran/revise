
DMA is the hardware controller mechanism that allows devices to transfer their I/O data directly to and from main memory without involving system processor
DMA increase through put to and from device so performance are boosted 

## DMA Data transfer overview

_For simplification only input transfer is described_

Data transfer can done in two ways: either software ask for data(read) or the hardware push the data asynchronously 

### Software ask

1. Process calls read
2. Driver allocate DMA buffer and instruct the hardware to transfer the data into buffer and put processor to sleep
3. Hardware write data to the DMA and raise an interrupt when it's done.
4. Interrupt handler receive's the data, acknowledge the interrupt and awake the process
5. Process will read the data

### Hardware push

Hardware will push the data to DMA buffer when ever hardware got data, even nobody is reading them, in this case driver should maintain a DMA buffer so that subsequent read call will return all the accumulated data to the user space

1. Hardware raise interrupt to announce about new data arrival
2. Interrupt handler allocate buffer and tells hardware where to transfer the data
3. Device writes data to the buffer and raise another interrupt when done
4. Handler dispatches new data and wakes any relevant process

So in both ways DMA handling relies on interrupt handling. It is possible to implement DMA in polling but it is a waste of performance benefit

DMA requires device drivers to allocate one or more special buffer suitable for DMA

[[DMA_buffer_allocation]]

[CH:15; pg:440]