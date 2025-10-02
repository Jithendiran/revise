// gcc rect.c -lm -o /tmp/rect.out
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <math.h> // Needed for sin, cos, M_PI
int fb_width;
int fb_height;
int fb_bpp;
int fb_bytes;
char *fbdata;

// Bresenham's Line Drawing
void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0); 
    
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    
    int err = dx + dy, e2; // error value

    while (1) {
        // Draw pixel at (x0, y0)
        int offset = (y0 * fb_width + x0) * fb_bytes;
        fbdata[offset + 0] = 255; // Blue
        fbdata[offset + 1] = 255; // Green
        fbdata[offset + 2] = 255; // Red
        fbdata[offset + 3] = 0;   // Alpha (or padding)

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_line_at_angle(int x0, int y0, float angle_deg, int length) {
    float angle_rad = angle_deg * (M_PI / 180.0);
    int x1 = x0 + (int)(cos(angle_rad) * length);
    int y1 = y0 - (int)(sin(angle_rad) * length); // upward line
    printf("Line (%d,%d) (%d,%d)", x0, y0, x1, y1);
    draw_line(x0, y0, x1, y1);
}

void draw_rect(int posx0, int posx1, int posy0, int posy1) {
/*
hint: width x height =  (number of columns) x  (number of rows)
The points given are diagonal of rectangle
x0,y0               x0,y1



x1,y0               x1,y1
*/

int start_row = posy0 * fb_width * fb_bytes;
int end_row = posy1 * fb_width * fb_bytes;

int start_col = start_row + posx0;
int end_col = end_row + posx1;


}

// fill

// draw at angle

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
    int fb_data_size = fb_width * fb_height * fb_bytes;
    fbdata = mmap(0, fb_data_size,
                        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    // blank the entire screen
    memset(fbdata, 0, fb_data_size);

    draw_line(100, 100, 150, 100); // Start : 1024400, end : 1536400
    // Start : 1024400, end : 1034640
    
    // draw_line_at_angle(100, 300, 30.0, 20); // 30° upward-right line
    // draw_line_at_angle(200, 300, 150.0, 100); // 150° upward-left
    draw_line_at_angle(100, 100, 0, 50);   // Horizontal right
    draw_line_at_angle(100, 100, 5, 50);   // Slightly angled line
    draw_line_at_angle(100, 100, 10, 50);  // More angled line
    draw_line_at_angle(100, 100, 15, 50);  // Even steeper
    draw_line_at_angle(100, 100, 20, 50);  // Steeper still

    // cleanup
    munmap(fbdata, fb_data_size);
    close(fbfd);
}
