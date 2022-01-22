#include <zephyr.h>
extern uint32_t img_buff_7[1280 * 1024];
#define YCBCR422_WHITE  0x80ff80ff
#define YCBCR422_YELLOW 0x00e194e1
#define YCBCR422_CYAN   0xabb200b2
#define YCBCR422_GREEN  0x2b951595
#define YCBCR422_PURPLE 0xd469e969
#define YCBCR422_RED    0x544cff4c
#define YCBCR422_BLUE   0xff1d6f1d
#define YCBCR422_BLACK  0x80108010
#define RGB_WHITE  0x00ffffff
#define RGB_YELLOW 0x0000ffff
#define RGB_CYAN   0x00ffff00
#define RGB_GREEN  0x0000ff00
#define RGB_PURPLE 0x00ff00ff
#define RGB_RED    0x000000ff
#define RGB_BLUE   0x00ff0000
#define RGB_BLACK  0x00000000
void draw_pattern(int h_active, int v_active);
void draw_color(int h_active, int v_active, int color);
void draw_color2(int h_active, int v_active, int color);
void draw_image(int h_active, int v_active);
