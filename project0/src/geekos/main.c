/*
 * GeekOS C code entry point
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2004, Iulian Neamtiu <neamtiu@cs.umd.edu>
 * $Revision: 1.51 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/bootinfo.h>
#include <geekos/string.h>
#include <geekos/screen.h>
#include <geekos/mem.h>
#include <geekos/crc32.h>
#include <geekos/tss.h>
#include <geekos/int.h>
#include <geekos/kthread.h>
#include <geekos/trap.h>
#include <geekos/timer.h>
#include <geekos/keyboard.h>


void project0()
{
    Keycode keycode;

    Print("Press Ctrl + d to exit!\n");
    while (1)
    {
        if(Read_Key(&keycode))
        {
            if((keycode & KEY_SPECIAL_FLAG) || (keycode & KEY_RELEASE_FLAG))    //只输出非特殊字符
            {
                int asciiCode = keycode & 0xff; //获取低8位Ascii码

                if((keycode & KEY_CTRL_FLAG) && asciiCode == 'd')
                {
                    Print("Exit project0\n");
                    Exit(1);
                }
                else
                {
                    Print("%c", asciiCode);
                }
                
            }
        }
    }
    
}

/*
 * Kernel C code entry point.
 * Initializes kernel subsystems, mounts filesystems,
 * and spawns init process.
 */
void Main(struct Boot_Info* bootInfo)
{
    Init_BSS();
    Init_Screen();
    Init_Mem(bootInfo);
    Init_CRC32();
    Init_TSS();
    Init_Interrupts();
    Init_Scheduler();
    Init_Traps();
    Init_Timer();
    Init_Keyboard();


    Set_Current_Attr(ATTRIB(BLACK, GREEN|BRIGHT));
    Print("Welcome to GeekOS!\n");
    Set_Current_Attr(ATTRIB(BLACK, GRAY));


    //TODO("Start a kernel thread to echo pressed keys and print counts");
    struct kernel_Thread *thread;
    thread = Start_Kernel_Thread(&project0, 0, PRIORITY_NORMAL, false);

    /* Now this thread is done. */
    Exit(0);
}









