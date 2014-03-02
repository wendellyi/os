#include <wenix/type.h>
#include <wenix/const.h>
#include <wenix/irq.h>
#include <wenix/system.h>

extern struct gate_t idt[NR_IDT];

static void set_gate(uint8 vector, uint8 type, uint8 dpl, void(* handler) (void))
{
    struct gate_t* gate = &idt[vector];
    uint32 base = (uint32)(long)handler;

    gate->offset_low    = base & 0xffff;
    gate->selector      = SELECTOR_KERNEL_CS;
    gate->param_count   = 0;
    gate->attr          = type | (dpl << 5);
    gate->offset_high   = (base >> 16) & 0xffff;
}

static void set_sys_gate(uint8 vector, void (* handler)(void))
{
    set_gate(vector, DA_386INTGATE, DPL_USER, handler);
}

static void set_trap_gate(uint8 vector, void (* handler)(void))
{
    set_gate(vector, DA_386INTGATE, DPL_KERNEL, handler);
}

void int_init(void)
{
    init_8259a();

    set_trap_gate(0, divide_error);
    set_trap_gate(1, debug);
    set_trap_gate(2, nmi);

    set_sys_gate(3, breakpoint);
    set_sys_gate(4, overflow);
    set_sys_gate(5, bounds);
    
    set_trap_gate(6, invalid_op);
	set_trap_gate(7, device_not_available);
	set_trap_gate(8, double_fault);
	set_trap_gate(9, coprocessor_segment_overrun);
	set_trap_gate(10, invalid_tss);
	set_trap_gate(11, segment_not_present);
	set_trap_gate(12, stack_segment);
	set_trap_gate(13, general_protection);
	set_trap_gate(14, page_fault);
	set_trap_gate(16, coprocessor_error);

    set_trap_gate(INT_VECTOR_IRQ0 + 0, hwint00);
    set_trap_gate(INT_VECTOR_IRQ0 + 1, hwint01);
    set_trap_gate(INT_VECTOR_IRQ0 + 2, hwint02);
    set_trap_gate(INT_VECTOR_IRQ0 + 3, hwint03);
    set_trap_gate(INT_VECTOR_IRQ0 + 4, hwint04);
    set_trap_gate(INT_VECTOR_IRQ0 + 5, hwint05);
    set_trap_gate(INT_VECTOR_IRQ0 + 6, hwint06);
    set_trap_gate(INT_VECTOR_IRQ0 + 7, hwint07);

    set_trap_gate(INT_VECTOR_IRQ8 + 0, hwint08);
    set_trap_gate(INT_VECTOR_IRQ8 + 1, hwint09);
    set_trap_gate(INT_VECTOR_IRQ8 + 2, hwint10);
    set_trap_gate(INT_VECTOR_IRQ8 + 3, hwint11);
    set_trap_gate(INT_VECTOR_IRQ8 + 4, hwint12);
    set_trap_gate(INT_VECTOR_IRQ8 + 5, hwint13);
    set_trap_gate(INT_VECTOR_IRQ8 + 6, hwint14);
    set_trap_gate(INT_VECTOR_IRQ8 + 7, hwint15);
}