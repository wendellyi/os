#ifndef _INCLUDE_WENIX_SCHED_H_
#define _INCLUDE_WENIX_SCHED_H_

struct tss_struct
{
	uint32	back_link;	            /* 16 high bits zero */
	uint32	esp0;
	uint32	ss0;		            /* 16 high bits zero */
	uint32	esp1;
	uint32	ss1;		            /* 16 high bits zero */
	uint32	esp2;
	uint32	ss2;		            /* 16 high bits zero */
	uint32	cr3;
	uint32	eip;
	uint32	eflags;
	uint32	eax, ecx, edx, ebx;
	uint32	esp;
	uint32	ebp;
	uint32	esi;
	uint32	edi;
	uint32	es;		                /* 16 high bits zero */
	uint32	cs;		                /* 16 high bits zero */
	uint32	ss;		                /* 16 high bits zero */
	uint32	ds;		                /* 16 high bits zero */
	uint32	fs;		                /* 16 high bits zero */
	uint32	gs;		                /* 16 high bits zero */
	uint32	ldt;		            /* 16 high bits zero */
	uint32	trace_bitmap;	        /* bits: trace 0, bitmap 16-31 */	
};

struct task_struct
{
    struct desc_t ldt[3];
    struct tss_struct tss;
};

#endif /* _INCLUDE_WENIX_SCHED_H_ */