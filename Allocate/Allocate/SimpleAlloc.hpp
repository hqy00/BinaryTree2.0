#include "alloc.h"


typedef DefaultAllocTemplate<0, 0> alloc;  
  
//封装成为SimpleAlloc,缺省使用二级空间配置器
template<class T, class Alloc = alloc>  
class SimpleAlloc   
{  
public:  
    static T* Allocate(size_t n)  
    {  
        return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));  
    }  

    static T* Allocate(void)  
    {  
        return (T*)Alloc::Allocate(sizeof (T));  
    }

    static void Deallocate(T *p, size_t n)  
    {  
        if (0 != n) Alloc::Deallocate(p, n * sizeof (T));  
    }

    static void Deallocate(T *p)  
    {  
        Alloc::Deallocate(p, sizeof (T));  
    }  
};  