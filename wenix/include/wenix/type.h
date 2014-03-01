#ifndef _WENIX_TYPE_H
#define _WENIX_TYPE_H

typedef unsigned            uint32;
typedef unsigned            uint16;
typedef unsigned char       uint8;

#pragma pack(1)
/* 描述符类型定义 */
struct desc_t
{
    uint16 limit_low;
    uint16 base_low;
    uint8 base_middle;
    uint8 attr1;
    uint8 limit_high_attr2;
    uint8 base_high;
};

/* 门描述符类型定义 */
struct gate_t
{
    uint16 offset_low;
    uint16 selector;
    uint8 param_count;
    uint8 attr;
    uint16 offset_high;
};
#pragma pack()

#endif /* _WENIX_TYPE_H */