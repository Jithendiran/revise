Setup
GPU with 2 GB VRAM
One or more color LCDs(1920 × 1080 resolution)
Each LCD is connected to the GPU via HDMI/DP/etc.

What is stored in the GPU’s VRAM?
* Framebuffers (pixel data for one or more displays)
* Textures for 3D rendering
* Shader programs
* Depth/stencil buffers
* Buffers for rendering pipelines
So the framebuffer is just one use of the VRAM.

What is a framebuffer?
A framebuffer is a block of memory that stores the color of every pixel currently being shown on a display.
If you have 
    A 1920 × 1080 screen (Full HD), 
    32 bits per pixel (common for full color + alpha)
    R - 8 bit
    G - 8 bit
    B - 8 bit
    alpha - 8 bit
    8 * 4 = 32 bit -> 4 bytes, each pixel info is 4 byte

    1920 × 1080 × 4 bytes = 8,294,400 × 4 = 33,177,600 bytes = 31.6 MB

    One screen at 1080p uses about 32 MB of VRAM for a single framebuffer.

Each active monitor typically has its own framebuffer in VRAM.
Monitor 1: 1920×1080  -> 32 MB
Monitor 2: 1920×1080  -> 32 MB
Total framebuffer usage: 64 MB, That’s still only a small fraction of your 2 GB VRAM (2048 MB).

The GPU driver (in the OS, like AMD/NVIDIA/Intel driver) and graphics APIs (like DRM/KMS on Linux, or DirectX/OpenGL/Vulkan on Windows) manage memory allocation inside VRAM.
- Framebuffers are just memory blocks allocated inside VRAM.
- You don't get to choose exact addresses (like 0x00100000); the driver does that.
- But conceptually, they occupy some region of that 0–2GB VRAM space.

| Memory region | Size   | Description                   |
| ------------- | ------ | ----------------------------- |
| 0x0000_0000   | 32 MB  | Framebuffer for Monitor 1     |
| 0x0200_0000   | 32 MB  | Framebuffer for Monitor 2     |
| 0x0400_0000   | 256 MB | Textures                      |
| 0x1400_0000   | 128 MB | Render target / depth buffers |
| ...           | ...    | More GPU-managed buffers      |

How does GPU know where to send each framebuffer?
The display engine inside the GPU:
- Knows which framebuffer is for which monitor.
- Continuously reads the framebuffer and sends the video signal to the correct output (HDMI, DP, etc).
- This is handled by the GPU's display pipeline (often called "CRTC" in Linux DRM).
You don’t need to manually manage this — the GPU driver sets it up.

On most modern systems with a discrete GPU, /dev/fb0 usually points to a framebuffer located in system RAM (main memory), not GPU VRAM. 
On some integrated GPUs or simple embedded devices, /dev/fb0 may map directly to GPU VRAM (which might just be a reserved region of system RAM).
It depends on how the framebuffer driver is implemented.

With GPU drivers (DRM/KMS):
Modern graphics stacks use Direct Rendering Manager (DRM) and Kernel Mode Setting (KMS).
These typically do NOT use /dev/fb0, but instead create devices like /dev/dri/card0.
The framebuffer in this context is managed by the GPU driver and stored in GPU VRAM.
/dev/fb0 might exist but is often a "stub" or fallback device.


How it works

1. **You call `write(fd, buffer, size)` on `/dev/fb0`**

   * Your process requests to write pixel data to the framebuffer device.

2. **The Linux kernel framebuffer driver intercepts the call**

   * The kernel receives the data from your user-space buffer.
   * **Data is copied from user space to kernel space** (this is a normal `write()` syscall behavior).

3. **Kernel copies data into framebuffer memory**

   * The framebuffer memory is **usually a memory-mapped region** representing the pixels.
   * Depending on your system:

     * If framebuffer is in **system RAM** (common), the kernel copies the data directly into that region.
     * If framebuffer is in **GPU VRAM**, then the kernel (or GPU driver) may have to **copy data from system RAM into VRAM**. This second copy depends on the hardware and driver implementation.
    data copied from user space to /dev/fb0

4. **The graphics hardware reads the framebuffer memory**

   * The GPU or display controller continuously scans the framebuffer memory.
   * It reads the pixel data to generate the video signals for the monitor.

5. **Screen updates with new pixel data**

   * The display shows the updated image based on the new framebuffer content.

--------------
When you mmap /dev/fb0, the kernel:

Maps the framebuffer memory region (which is either in system RAM or GPU VRAM, but accessible from CPU) directly into your process’s address space.

Why is mmap useful here?
No extra copying: Unlike write(), which copies your pixel data into kernel space, with mmap you operate directly on framebuffer memory.