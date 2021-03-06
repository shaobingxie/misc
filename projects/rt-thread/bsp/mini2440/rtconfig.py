# panel options
# 'PNL_A70','PNL_N35', 'PNL_T35'
RT_USING_LCD_TYPE = 'PNL_T35'

# toolchains options
ARCH     = 'arm'
CPU      = 's3c24x0'
TextBase = '0x30000000'

CROSS_TOOL 	= 'gcc'

if  CROSS_TOOL == 'gcc':
	PLATFORM 	= 'gcc'
	#EXEC_PATH 	= 'E:/Program Files/CodeSourcery/Sourcery G++ Lite/bin'
	EXEC_PATH = '/home/lixianjing/CodeSourcery/Sourcery_G++_Lite/bin'
elif CROSS_TOOL == 'keil':
	PLATFORM 	= 'armcc'
	EXEC_PATH 	= 'E:/Keil'
BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    #PREFIX = 'arm-none-eabi-'
    PREFIX = 'arm-none-linux-gnueabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'axf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -mcpu=arm920t'
    CFLAGS = DEVICE
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp' + ' -DTEXT_BASE=' + TextBase
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread_mini2440.map,-cref,-u,_start -T mini2440_ram.ld' + ' -Ttext ' + TextBase
    LFLAGS += ' -nostdlib'
    CFLAGS += ' -nostdinc -nostdlib'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --device DARMSS9'
    CFLAGS = DEVICE + ' --apcs=interwork --diag_suppress=870'
    AFLAGS = DEVICE
    LFLAGS = DEVICE + ' --strict --info sizes --info totals --info unused --info veneers --list rtthread-mini2440.map --ro-base 0x30000000 --entry Entry_Point --first Entry_Point'

    CFLAGS += ' -I"' + EXEC_PATH + '/ARM/RV31/INC"'
    LFLAGS += ' --libpath "' + EXEC_PATH + '/ARM/RV31/LIB"'

    EXEC_PATH += '/arm/bin40/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'

elif PLATFORM == 'iar':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'

    CFLAGS = ''
    AFLAGS = ''
    LFLAGS = ''
