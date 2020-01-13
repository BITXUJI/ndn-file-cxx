#pragma once 
#include <string>

// interest name:/Rpi-1/lab1028/info/about/10Mfile.txt/ABCD/1
namespace NdnFileConfig{

	//prefix for routing
	static const std::string prefix="/Rpi-1/lab1038/info/about";

	//fileNameList 
    static const std::string fileNameList[10]={"10Mfile1.txt","10Mfile2.txt","10Mfile3.txt","10Mfile4.txt","10Mfile5.txt","10Mfile6.txt","10Mfile7.txt","10Mfile8.txt","10Mfile9.txt","10Mfile10.txt"};
	
	//placeHolder
	static const std::string placeHolder ="ABCD";

	//path for downloaded file at client side 
    static const std::string downloadFilePath="/home/ndn/ndn-file-cxx/download/";

	//path for uploading file at server side
	static const std::string uploadFilePath="/home/ndn/ndn-file-cxx/upload/";

	//max sequence number of each interest to be sent;
    static const u_int64_t maxSeq=10240; //10MB

	//max segment size of each data
	static const u_int64_t maxSegmentSize = 1024; //1KB

	static int fileNumber =1;

}
