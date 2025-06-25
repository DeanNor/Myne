
#include "process.hpp"
#include "saver.hpp"

int main()
{
    for (int x = 0; x < 100000; x++)
    {
        Process* p = new Process;
        Saver os("ofile.txt");

        p->name = "HI";
        p->position.children = 90;
        p->test = 1;

        os.save_process(p);

        delete p;
    }

    return 0;
}
