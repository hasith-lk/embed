libpath="C:\\Users\\Hasith Perera\\.platformio\\packages\\framework-ststm8spl\\Libraries\\STM8S-SDCC-SPL\\src"
import glob

from pathlib import Path
for path in Path(libpath).glob("stm8s_uart4_*.c"):
    print(path)
