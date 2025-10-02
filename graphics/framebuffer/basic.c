/*
 * Author: E.K. Jithendiran
 * Date: 30 Sep
 */

/*
Framebuffer
---------------
it's a very low-level way of doing graphics, and an even lower-level way of writing text
In modern system we don't have to use this much low level to written graphics, we can use libraries and tools which will draw the screen for us
But Getting a full graphical desktop environment working on an embedded system can be difficult eg: e-readers

Framebuffer devices usually appear as /dev/fbXXX. This is usually owned by root.
Don't use linux desktop to run the program, execute the program from linux consoles: ctrl+alt+F-key eg: ctrl+alt+F2


the framebuffer memory is organized into four-byte pixels, with the first three bytes storing the blue, green, and red colour levels (in that order), and the fourth being unused. The fourth byte is often referred to as the 'alpha' or 'transparency' channel(there are framebuffers with two-byte and even one-byte pixels)

most framebuffers are organized row-first

+---+---+---+---+
| 1 | 2 | 3 | 4 |
+---+---+---+---+
| 5 | 6 | 7 | 8 |
+---+---+---+---+
| 9 | 10| 11| 12|
+---+---+---+---+
widthxheight = 4 (number of columns) x 3 (number of rows)
*/
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>


int fb_width;
int fb_height;
int fb_bpp;
int fb_bytes;

// Function to set a single pixel with RGB values
void set_pixel(char *fbdata, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    /*
    fb_width = 1920 (screen width is 1920 pixels).
    fb_height = 1080 (screen height is 1080 pixels).
    fb_bytes = 4 bytes (R - 1, G - 2, B - 3, T - 4)

    avail range: 0 - 8294400 for (1920x1080)
    
    Now, let's calculate the byte index for the pixel at position (x = 100, y = 50)

    1. Calculate the number of pixels before this row
         (y) * fb_width = 50 * 1920 = 96000 pixels
    2. Add the horizontal position:
        (y * fb_width + x) = 96000 + 100 = 96100 pixels.
    3. Multiply by fb_bytes to get the byte offset:
        96100 * 4 = 384400 bytes.
    So, the pixel at (100, 50) will be located at the 384400 byte
    */
    int pixel_index = ((y)*fb_width + (x)) * fb_bytes;
    fbdata[pixel_index] = b;     // Blue
    fbdata[pixel_index + 1] = g; // Green
    fbdata[pixel_index + 2] = r; // Red
    fbdata[pixel_index + 3] = 0; // transparent
}

int main()
{
    int fbfd = open("/dev/fb0", O_RDWR);

    struct fb_var_screeninfo vinfo;

    ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo); // get info

    fb_width = vinfo.xres;
    fb_height = vinfo.yres;
    fb_bpp = vinfo.bits_per_pixel;
    fb_bytes = fb_bpp / 8;

    printf("Res: W: %d, H: %d, bits_per_inch: %d, count of single pixel : %d\n", fb_width, fb_height, fb_bpp, fb_bytes);
    // Res: W: 2560, H: 1440, bits_per_inch: 32, count of single pixel : 4

    int fb_data_size = fb_width * fb_height * fb_bytes;

    char *fbdata = mmap(0, fb_data_size,
                        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    // blank the entire screen
    memset(fbdata, 0, fb_data_size);

    // Draw some colored pixels
    set_pixel(fbdata, 100, 100, 255, 0, 0);   // Red pixel at (100, 100)
    set_pixel(fbdata, 200, 100, 0, 255, 0);   // Green pixel at (200, 100)
    set_pixel(fbdata, 300, 100, 0, 0, 255);   // Blue pixel at (300, 100)
    set_pixel(fbdata, 400, 100, 255, 255, 0); // Yellow pixel at (400, 100)
    set_pixel(fbdata, 500, 100, 255, 0, 255); // Magenta pixel at (500, 100)

    // cleanup
    munmap(fbdata, fb_data_size);
    close(fbfd);
}


/*
1. gcc basic.c -o /tmp/basic.out
2. ctrl+alt+f2 -> run this pgm
*/