extern struct desc_t gdt[NR_GDT];

/* type 0x82为ldt 0x89为tss */
void set_ldttss_desc(int n, void * addr, uint8 type)
{
    struct desc_t * desc = &gdt[n];
    uint32 addr_value = (uint32)addr;
    
    desc->limit_low = 4095 & 0xffff
    desc->base_low = addr_value & 0xffff;
    desc->base_middle = (addr_value >> 16) & 0xff;
    desc->attr1 = type;
    desc->limit_high_attr2 = ((4095 >> 16) & 0x0f)
    desc->base_high = (addr_value >> 24) & 0xff;
}