#include <stdlib.h>

extern void RunMountingTests(void);
extern void RunFunctionalTests(void);

int main(int argc, char** argv) {
    RunMountingTests();
    RunFunctionalTests();
        
    return (EXIT_SUCCESS);
}





