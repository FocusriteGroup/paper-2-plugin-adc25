#include "TestTools.h"

#include <MVMFilter.h>

int main()
{
    TestTools::stdioTest<MVMFilter, 128>({});
    return 1;
}