#include <iostream>
#include "server.h"
#include "ndnfileconfig.h"

int main(int argc, char * argv[]){
    Server server(NdnFileConfig::prefix, NdnFileConfig::maxSegmentSize, NdnFileConfig::uploadFilePath);
    server.run();
    return 0;
}