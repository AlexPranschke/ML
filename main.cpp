#include <iostream>
#include "data.h"
#include "dataHander.h"

int main() {
    unsigned int num = 3333333;
    char* ptr = reinterpret_cast<char*>(&num);

    dataHandler* dh = new dataHandler();
    dh->ReadFeatureVector("../t10k-images.idx3-ubyte");
    dh->ReadFeatureLabels("../t10k-labels.idx1-ubyte");
    dh->SplitData();
    dh->CountClasses();

    return 0;
}
