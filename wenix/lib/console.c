/*
 * wenix/lib/console.c
 */
 
#include <wenix/system.h>
#include <wenix/const.h>
#include <string.h>

void print_string(const char * str)
{
    print_color_string(str, CHAR_COLOR_DEFAULT);
}

static void clear_row(int row)
{
    int i;
    
    set_display_position(row*160);
    
    for (i = 0; i < 80; i++)
        print_char(' ', CHAR_COLOR_DEFAULT);
}

/* 屏幕向上滚动一行 */
static int roll_screen(int position)
{
    int rows = position / 160;
    int i;
    
    for (i = 0; i < rows-1; i++)
        vgacpy((void *)(long)(i*160), (void *)(long)((i+1)*160), 160);
        
    clear_row(rows-1);
    vgacpy((void *)(long)((rows-1)*160), (void *)(long)(rows*160), 160);
    clear_row(rows-1);

    position -= 160;
    if (0 > position)
        return 0;
    else
        return position;
}

void print_color_string(const char * str, uint8 color)
{
    int i, len;
    int position;
    int rows;

    len = strlen(str);
    
    for (i = 0; i < len; i++)
    {
        position = get_display_position();

        if ('\n' == str[i])
        {
            rows = position / 160;
            position = (rows+1)*160;
        }
        else
        {
            print_char(str[i], color);
            position += 2;
        }
        
        set_display_position(position);
        
        if (3840 <= position)
        {
            position = roll_screen(position);
            set_display_position(position);
        }
    }  
}
