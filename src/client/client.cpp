#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cassert>

#include <ndn-cxx/interest.hpp>
#include "ndnfileconfig.h"

#include "client.h"


#define MAX_CACHE static_cast<u_int64_t>(10*1024) //10MB

Client::Client(const std::string & prefix) : 
    m_prefix(prefix)
{
}

Client::Client(const std::string & prefix, const std::string & name, const std::string & path, uint64_t maxSeq) :
    m_prefix(prefix),
    m_fileName(name),
    m_downloadPath(path),
    m_maxSeq(maxSeq)
{
    if(m_downloadPath.back() != '/'){
        m_downloadPath.append("/");
    }

}

void Client::requestFileList(){

        ndn::Name interestName(m_prefix);
        sendInterest(interestName.append("fileList"));
        std::cout<<m_prefix<<std::endl;
        m_face.processEvents();
 
}

void Client::requestFile(){
  
        ndn::Name interestName(m_prefix);
        //xuji modified to add placeHolder.
        sendInterest(interestName.append(m_fileName).append(NdnFileConfig::placeHolder).appendNumber(0));

        while(!m_done){
            m_face.processEvents();
            //usleep(100);
        }
 
}

void Client::sendInterest(ndn::Name & interestName){
    ndn::Interest interest(interestName);
    interest.setCanBePrefix(false);
    interest.setMustBeFresh(true);
    interest.setInterestLifetime(ndn::time::milliseconds(2000));
    interest.setNonce(std::rand());

    try {
        m_face.expressInterest(interest,
        std::bind(&Client::onData, this, _2), 
        std::bind(&Client::onNack, this), 
        std::bind(&Client::onTimeOut, this, _1));

        // std::cout << "send interest: " << interest.getName() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

void Client::onData(const ndn::Data & data){
    ndn::Name dataName = data.getName();
    // std::cout << "receive data: " << dataName << std::endl;
    if(dataName.at(-1).toUri() == "fileList"){
        const ndn::Block & content = data.getContent();
        std::string fileListInfor(reinterpret_cast<const char*>(content.value()), content.value_size());
        std::cout<<"filelist retrieved: "<<std::endl
                 <<fileListInfor<<std::endl;
        
    }
    else{
        auto dataPtr = data.shared_from_this();
        const auto receiveSeq = static_cast<u_int64_t>(dataName.at(-1).toNumber());
        // std::cout << receiveSeq << std::endl;
        m_dataCache.insert({receiveSeq, dataPtr});

        

        if((receiveSeq+1) == m_maxSeq || (receiveSeq+1) % MAX_CACHE == 0){
            writeToFile();
        }

        if((receiveSeq+1) == m_maxSeq){
          
            m_done = true;
            std::cout <<"Last segment received." << std::endl;
        }
        else{
            ndn::Name interestName(m_prefix);
            //xuji modified to add placeholder
            sendInterest(interestName.append(m_fileName).append(NdnFileConfig::placeHolder).appendNumber(receiveSeq+1));
        }
    }
}

void Client::onNack(){
    std::cerr << " receive Nack " << std::endl;
}

void Client::onTimeOut(const ndn::Interest & interest){
    const auto receiveSeq = static_cast<u_int64_t>(interest.getName().at(-1).toNumber());
    std::cerr << "Time out: " << interest.getName() << "Seq = " << receiveSeq << std::endl;

    // retransmission
    ndn::Name interestName(m_prefix);
    //xuji modified to add placeholder
    sendInterest(interestName.append(m_fileName).append(NdnFileConfig::placeHolder).appendNumber(receiveSeq+1));
}

void Client::writeToFile(){
    std::string filePath(m_downloadPath); 
    filePath.append(m_fileName);
    std::ofstream fout;

    if(m_dataCache.find(0) != m_dataCache.end()){
        fout.open(filePath, std::ios::out | std::ios::trunc);  
    }
    else{
        fout.open(filePath, std::ios::out | std::ios::app);
    }
    assert(fout.is_open());

    for(auto &item : m_dataCache){
        const ndn::Block & content = item.second->getContent();
        for (size_t i = 0; i < content.value_size(); ++i) {
            fout << (content.value())[i];
        }
    }

    fout.clear();
    fout.close();

    m_dataCache.clear();
}