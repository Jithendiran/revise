Usually I/O operations are done through kernel. Kernel space is a separation between user-space and device. Because of this separation programming become easier and yields performance benefit, Though some applications like large amount of data transfer to and from device performance would improve if communications are happened through user space directly

There are many drawbacks in user pace direct IO like overwhelming operations like setting up page alignment, cannot perform async operations and so

Key implementation to perform direct IO is with function `get_user_pages`
This function will return list of pointers to user space buffer, which is locked into memory. To operate on user space buffer directly, kernel space must turn each user space `struct page` to kernel virtual address with `kmap`.  

Once IO operations are completed, user space pages must be released. Before release if any content of the buffer is changed need to set the dirty flag of the page, this will help in writing back. Finally pages must free from page cache 

## Asynchronous IO

2.6 kernel is capable of _asynchronous IO_. Allows user space to initiate IO without waiting for previous IO to complete. This async IO is optional, many driver don't require async IO

`file_operations` have 3 method to support async IO

- `aio_read` this is like regular read
- `aio_write` this like regular write
- `aio_fsync` This is only for file system, not interested here

aio_read and aio_write initiate a request that may or may not complete by the time of this functions returned.

In async operations kernel can create "synchronous IO control blocks" which mean some async operations must execute in sync way. To find is this operation is sync form of async or not by using `int is_sync_kiocb`

If driver is able to do async IO operation. There are two things 
1. Remembering the operation informations like accessing the user-space buffer
2. return -EIOCBQUEUED (means operations is not yet completed and it's final status will be posted later)
`aio_complete` this function will inform kernel by driver about the completion of the operation


[Ch:15;pg:435]