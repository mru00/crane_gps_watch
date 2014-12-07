#include <iostream>
#include <fstream>

#include <vector>
#include <list>


using namespace std;

int main(int argc, char** argv) {

    list<vector<unsigned char> > buffers;

    for (int i = 1; i < argc-2; i++) {
        const char * fn = argv[i];
        ifstream f;
        f.open(fn, ios::binary);
        buffers.emplace_back(0xbf, 0);
        f.read( (char*) & buffers.back()[0], buffers.back().size());
        unsigned char cs_orig = buffers.back()[0xbc];
        cout 
          << "loading " << fn
          << hex
          << " cs=" << (int) cs_orig
          << endl;
    }


    for (int begin = 0; begin < 5; begin ++) {
        for (int end = 0xb0; end < 0xbf; end++) {

            unsigned char iv = 0;

                bool success = true;
                int nmatch = 0;

                for (auto buffer: buffers) {
                    unsigned char cs_orig = buffer[0xbc];
                    unsigned char cs = iv;

                    for (int i = begin; i < end; i ++) {
                        cs ^= buffer[i];
                    }

                    if (cs != cs_orig) {
                        success = false;
                    }
                    else {
                        nmatch ++;
                    }
                }

                if (success) {
                    cout 
                      << dec
                      << " begin=" << begin
                      << " end=" << end
                      << " nmatch=" << nmatch
                      << hex
                      << " iv=" << (int) iv
                      << endl;
                }
        }
    }

    return 0;
}
