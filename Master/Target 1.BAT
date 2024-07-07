SET PATH=D:\MDK5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files (x86)\VMware\VMware Workstation\bin\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NGX;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;D:\MATLAB\runtime\win64;D:\MATLAB\bin;D:\MATLAB\polyspace\bin;D:\opencv\opencv\build\x64\vc14\bin;D:\OpenCV 1.0\bin;D:\Fiddler
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_it.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_usart.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\misc.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_tim.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\core_cm3.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\startup_stm32f10x_hd._ia"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\main.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\uart.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmLink" --Via ".\Output\Uart_Interrupt.lnp"
"D:\MDK5\ARM\ARMCC\Bin\fromelf.exe" ".\Output\Uart_Interrupt.axf" --i32combined --output ".\Output\Uart_Interrupt.hex"
