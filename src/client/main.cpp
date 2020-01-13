#include <iostream>
#include "ndnfileconfig.h"
#include "client.h"

int main(int argc, char * argv[]){
   for(int i=0;i<10;i++){
        
        std::cout<<NdnFileConfig::fileNumber<<std::endl;
        Client client(NdnFileConfig::prefix,NdnFileConfig::fileNameList[i],NdnFileConfig::downloadFilePath,NdnFileConfig::maxSeq);
        client.requestFile();
        NdnFileConfig::fileNumber++;
   }
    return 0;
}
