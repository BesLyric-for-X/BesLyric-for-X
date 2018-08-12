#ifndef GLOBAL_H
#define GLOBAL_H

    //在 MSVC 编译器编译下显示乱码时，引用该头文件

    //MSVC 编译的话，为了防止乱码，需要 改编译配置
    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
    #endif

#endif // GLOBAL_H
