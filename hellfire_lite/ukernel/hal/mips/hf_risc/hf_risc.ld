ENTRY(_entry)

/* specify HF-RISC memory areas */

MEMORY 
{
ram		: ORIGIN = 0x40000000, LENGTH = 0x20000	/* free RAM area*/
}

/* define a global symbol _stack  */

_stack = 0x4001fffc;

/* now define the output sections  */

SECTIONS 
{
	. = 0;

	.text :
	{
		_text = .;
		*(.text)
	        *(.text.*)
		_etext = .;
	} > ram

	.rodata :
	{
		_rodata = .;
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
	} > ram

	.data :
	{
		_data = .;
		*(.data)
		*(.data.*)
		. = ALIGN(16); 
		_gp = . + 0x7ff0; 
	} > ram

        .got : { 
                *(.got) 
                *(.got.*) 
        } > ram

	_edata = .;

	.bss :
	{
		__bss_start = .;
		*(.sbss) 
		*(.sbss.*) 
		*(.bss) 
		*(.bss.*)
		. = ALIGN(4);
		__bss_end = . ;
	} > ram

}
	_end = .;

