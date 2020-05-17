/*
  Custom Boot Splash BMP
  Copyright (C) 2018, Princess of Sleeping
  Copyright (C) 2020, teakhanirons
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// ur0:tai/boot_splash.bmp needs to be 24 bit RGB

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/io/stat.h>
#include <psp2kern/display.h>
#include <psp2kern/kernel/dmac.h> 

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){

	SceKernelAllocMemBlockKernelOpt optp;
	SceDisplayFrameBuf fb;
	SceIoStat stat;

	void *fb_addr = NULL;
	void *bmp_addr = NULL;

	int stat_ret, uid, rgbuid, fd;

	stat_ret = ksceIoGetstat("ur0:tai/boot_splash.bmp", &stat);

	// 24 bit RGB BMP + 54 byte header = 0x17E836 size
	if((stat_ret < 0) || ((uint32_t)stat.st_size != 0x17E836) || (SCE_SO_ISDIR(stat.st_mode) != 0)) return SCE_KERNEL_START_SUCCESS;

	ksceDebugPrintf("size okke!\n");

	optp.size = 0x58;
	optp.attr = 2;
	optp.paddr = 0x1C000000;

	if((uid = ksceKernelAllocMemBlock("SceDisplay", 0x6020D006, 0x200000, &optp)) < 0) return SCE_KERNEL_START_SUCCESS;

	ksceDebugPrintf("disple okke!\n");

	if((rgbuid = ksceKernelAllocMemBlock("rgb", 0x1050D006, 0x200000, NULL)) < 0) return SCE_KERNEL_START_SUCCESS;

	ksceDebugPrintf("rgb okke!\n");

	ksceKernelGetMemBlockBase(uid, (void**)&fb_addr);
	ksceKernelGetMemBlockBase(rgbuid, (void**)&bmp_addr);

	ksceDmacMemset(fb_addr, 0xFF, 0x1FE000); // alpha will be FF, this is fallback
	ksceDebugPrintf("memset okke!\n");

	fd = ksceIoOpen("ur0:tai/boot_splash.bmp", SCE_O_RDONLY, 0);
	ksceIoLseek(fd, 54,	SCE_SEEK_SET); // 54 byte bmp header
	ksceIoRead(fd, bmp_addr, 0x17E7E2); // 0x17E836 - 54 = 0x17E7E2
	ksceIoClose(fd);
    ksceDebugPrintf("bmp read okke!\n");

    for(int i = 544 - 1; i >= 0; i--) {
    	for(int j = 0; j < 960; j++) {
    		// A B G R
        	*(uint32_t *)(fb_addr + ((i * 960) + j) * 4) =
            	((((char *)bmp_addr)[((i * 960) + j) * 3 + 2]) <<  0) |
    	        ((((char *)bmp_addr)[((i * 960) + j) * 3 + 1]) <<  8) |
        	    ((((char *)bmp_addr)[((i * 960) + j) * 3 + 0]) << 16) |
            	0xFF000000;
		}
    }

	ksceDebugPrintf("display okke!\n");

	ksceKernelFreeMemBlock(rgbuid);

	ksceKernelCpuDcacheAndL2WritebackInvalidateRange(fb_addr, 0x1FE000);

	fb.size        = sizeof(fb);
	fb.base        = fb_addr;
	fb.pitch       = 960;
	fb.pixelformat = 0;
	fb.width       = 960;
	fb.height      = 544;

	ksceDisplaySetFrameBuf(&fb, 1);

	ksceDisplayWaitVblankStart();

	ksceKernelFreeMemBlock(uid);

	ksceKernelDelayThread(3 * 1000 * 1000);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args){
	return SCE_KERNEL_STOP_SUCCESS;
}
